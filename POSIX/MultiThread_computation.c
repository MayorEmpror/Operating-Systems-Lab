#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define size_Arr 10000000
#define num_Threads 10 

float arr_A[size_Arr];
float arr_B[size_Arr];
float arr_C[size_Arr];

void *initialize_array(void* args){
    int thread_id = *(int*)args;

    int chunk = size_Arr / num_Threads;
    int start = thread_id * chunk;
    int end = start + chunk;

    for(int i = start; i < end; i++){
        arr_A[i] = rand() % 1000;
        arr_B[i] = rand() % 1000;
        arr_C[i] = rand() % 1000;
    }
    return NULL;
}

void *sumation_series(void* args){
    int thread_id = *(int*)args;

    int chunk = size_Arr / num_Threads;
    int start = thread_id * chunk;
    int end = start + chunk;

    for(int i = start; i < end; i++){
        arr_C[i] = arr_A[i] + arr_B[i];
    }
    return NULL;
}

int main(){
    pthread_t threads_init[num_Threads];
    int pthread_ids_init[num_Threads];

    pthread_t threads_sumation[num_Threads];
    int pthread_ids_sumation[num_Threads];

    // Initialize arrays
    for(int i = 0; i < num_Threads; i++){
        pthread_ids_init[i] = i;
        pthread_create(&threads_init[i], NULL, initialize_array, &pthread_ids_init[i]);
    }

    for(int i = 0; i < num_Threads; i++){
        pthread_join(threads_init[i], NULL);
    }

    // Sum arrays
    for(int i = 0; i < num_Threads; i++){
        pthread_ids_sumation[i] = i;
        pthread_create(&threads_sumation[i], NULL, sumation_series, &pthread_ids_sumation[i]);
    }

    for(int i = 0; i < num_Threads; i++){
        pthread_join(threads_sumation[i], NULL);
    }

    printf("%f + %f = %f\n", arr_A[0], arr_B[0], arr_C[0]);
    printf("%f + %f = %f\n", arr_A[1], arr_B[1], arr_C[1]);
    printf("%f + %f = %f\n", arr_A[2], arr_B[2], arr_C[2]);

    return 0;
}