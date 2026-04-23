#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <deque>
#include <ctime>

using namespace std;

#define MAX_WAIT 30

deque<Patient*> criticalQ, seriousQ, normalQ;

pthread_mutex_t queueLock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t patientAvailable = PTHREAD_COND_INITIALIZER;

void* patientThread(void* arg) {
    Patient* p = (Patient*)arg;

    pthread_mutex_lock(&queueLock);

    p->arrival = time(NULL);
    p->assigned = false;

    if (p->type == CRITICAL) criticalQ.push_back(p);
    else if (p->type == SERIOUS) seriousQ.push_back(p);
    else normalQ.push_back(p);

    pthread_cond_broadcast(&patientAvailable);

    while (!p->assigned) {
        pthread_cond_wait(&p->cond, &queueLock);
    }

    pthread_mutex_unlock(&queueLock);

    cout << "Patient " << p->id << " treated\n";
    return NULL;
}


struct Doctor {
    int id;
    DoctorType type;
    int normalCount;
};

Patient* getPatient(Doctor* doc) {
    // Rule 4: force serious after 3 normals
    if (doc->normalCount >= 3 && !seriousQ.empty()) {
        Patient* p = seriousQ.front();
        seriousQ.pop_front();
        doc->normalCount = 0;
        return p;
    }

    // Critical
    if (!criticalQ.empty()) {
        if (doc->type == JUNIOR) return NULL; // can't treat
        Patient* p = criticalQ.front();
        criticalQ.pop_front();
        return p;
    }

    // Serious
    if (!seriousQ.empty()) {
        Patient* p = seriousQ.front();
        seriousQ.pop_front();
        return p;
    }

    // Normal
    if (!normalQ.empty()) {
        Patient* p = normalQ.front();
        normalQ.pop_front();
        doc->normalCount++;
        return p;
    }

    return NULL;
}

void* doctorThread(void* arg) {
    Doctor* doc = (Doctor*)arg;

    while (true) {
        pthread_mutex_lock(&queueLock);

        Patient* p = NULL;

        while (!(p = getPatient(doc))) {
            pthread_cond_wait(&patientAvailable, &queueLock);
        }

        p->assigned = true;
        pthread_cond_signal(&p->cond);

        pthread_mutex_unlock(&queueLock);

        cout << "Doctor " << doc->id << " treating patient " << p->id << endl;

        sleep(2); // simulate treatment
    }

    return NULL;
}

void* monitorThread(void* arg) {
    while (true) {
        sleep(1);

        pthread_mutex_lock(&queueLock);

        time_t now = time(NULL);

        // Rule 5: 30 sec wait
        for (auto it = seriousQ.begin(); it != seriousQ.end(); ) {
            if (difftime(now, (*it)->arrival) > MAX_WAIT) {
                criticalQ.push_back(*it);
                it = seriousQ.erase(it);
            } else it++;
        }

        for (auto it = normalQ.begin(); it != normalQ.end(); ) {
            if (difftime(now, (*it)->arrival) > MAX_WAIT) {
                criticalQ.push_back(*it);
                it = normalQ.erase(it);
            } else it++;
        }

        // Rule 3: promote serious
        if (seriousQ.size() >= 5) {
            criticalQ.push_back(seriousQ.front());
            seriousQ.pop_front();
        }

        pthread_mutex_unlock(&queueLock);
    }
}

int main() {
    pthread_t docs[3];
    Doctor d[3] = {
        {1, SENIOR, 0},
        {2, JUNIOR, 0},
        {3, SENIOR, 0}
    };

    for (int i = 0; i < 3; i++)
        pthread_create(&docs[i], NULL, doctorThread, &d[i]);

    pthread_t monitor;
    pthread_create(&monitor, NULL, monitorThread, NULL);

    // Create patients
    for (int i = 0; i < 20; i++) {
        Patient* p = new Patient();
        p->id = i;
        p->type = (PatientType)(i % 3);
        pthread_cond_init(&p->cond, NULL);

        pthread_t t;
        pthread_create(&t, NULL, patientThread, p);

        sleep(1);
    }

    pthread_join(monitor, NULL);
    return 0;
}