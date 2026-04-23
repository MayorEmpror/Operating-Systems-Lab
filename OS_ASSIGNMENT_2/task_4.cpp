#define MAX_RUNWAYS 3
#define THRESHOLD 5

typedef enum {
    EMERGENCY = 5,
    LANDING = 4,
    TAKEOFF = 3,
    CARGO = 2
} FlightType;

typedef struct {
    int id;
    FlightType type;
    int priority;
    time_t arrivalTime;
    int fuelLow; // for landing priority boost
} Flight;

typedef struct Node {
    Flight flight;
    struct Node* next;
} Node;


Node* priorityQueue = NULL;

int runwayStatus[MAX_RUNWAYS]; // 0 = free, 1 = busy

pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t runwayMutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;

void enqueueFlight(Flight f) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->flight = f;
    newNode->next = NULL;

    if (!priorityQueue || f.priority > priorityQueue->flight.priority) {
        newNode->next = priorityQueue;
        priorityQueue = newNode;
    } else {
        Node* temp = priorityQueue;
        while (temp->next && temp->next->flight.priority >= f.priority)
            temp = temp->next;

        newNode->next = temp->next;
        temp->next = newNode;
    }
}

Flight dequeueFlight() {
    Node* temp = priorityQueue;
    Flight f = temp->flight;
    priorityQueue = temp->next;
    free(temp);
    return f;
}

void* flightGenerator(void* arg) {
    int id = 1;

    while (1) {
        Flight f;
        f.id = id++;
        f.type = rand() % 4 + 2; // CARGO to EMERGENCY
        f.arrivalTime = time(NULL);
        f.fuelLow = rand() % 2;

        // Base priority
        f.priority = f.type;

        // Fuel low boost
        if (f.type == LANDING && f.fuelLow)
            f.priority += 2;

        pthread_mutex_lock(&queueMutex);

        enqueueFlight(f);
        printf("Generated Flight %d (type=%d priority=%d)\n", f.id, f.type, f.priority);

        pthread_cond_signal(&queueCond);
        pthread_mutex_unlock(&queueMutex);

        sleep(1);
    }
}



void* runwayController(void* arg) {
    int runwayId = *(int*)arg;

    while (1) {
        pthread_mutex_lock(&queueMutex);

        while (priorityQueue == NULL)
            pthread_cond_wait(&queueCond, &queueMutex);

        Flight f = dequeueFlight();
        pthread_mutex_unlock(&queueMutex);

        // Lock runway
        pthread_mutex_lock(&runwayMutex);
        runwayStatus[runwayId] = 1;
        pthread_mutex_unlock(&runwayMutex);

        printf("Runway %d handling Flight %d (priority=%d)\n", runwayId, f.id, f.priority);

        sleep(2); // simulate operation

        pthread_mutex_lock(&runwayMutex);
        runwayStatus[runwayId] = 0;
        pthread_mutex_unlock(&runwayMutex);

        printf("Runway %d finished Flight %d\n", runwayId, f.id);
    }
}


void* emergencyMonitor(void* arg) {
    while (1) {
        pthread_mutex_lock(&queueMutex);

        Node* temp = priorityQueue;
        time_t now = time(NULL);

        while (temp) {
            // Dynamic priority increase
            if (difftime(now, temp->flight.arrivalTime) > THRESHOLD) {
                temp->flight.priority++;
            }

            // Emergency detection
            if (temp->flight.type == EMERGENCY) {
                printf("🚨 Emergency Flight Detected: %d\n", temp->flight.id);

                // Preemption logic (simulation)
                pthread_mutex_lock(&runwayMutex);
                for (int i = 0; i < MAX_RUNWAYS; i++) {
                    if (runwayStatus[i] == 1) {
                        printf("⚠ Preempting runway %d for emergency\n", i);
                        // In real system: safely interrupt
                        runwayStatus[i] = 0;
                        break;
                    }
                }
                pthread_mutex_unlock(&runwayMutex);
            }

            temp = temp->next;
        }

        pthread_mutex_unlock(&queueMutex);

        sleep(1);
    }
}

int main() {
    pthread_t generator, monitor;
    pthread_t runways[MAX_RUNWAYS];

    int ids[MAX_RUNWAYS];

    // Init runway status
    for (int i = 0; i < MAX_RUNWAYS; i++) {
        runwayStatus[i] = 0;
        ids[i] = i;
    }

    pthread_create(&generator, NULL, flightGenerator, NULL);
    pthread_create(&monitor, NULL, emergencyMonitor, NULL);

    for (int i = 0; i < MAX_RUNWAYS; i++) {
        pthread_create(&runways[i], NULL, runwayController, &ids[i]);
    }

    pthread_join(generator, NULL);
    pthread_join(monitor, NULL);

    for (int i = 0; i < MAX_RUNWAYS; i++)
        pthread_join(runways[i], NULL);

    return 0;
}

