#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h>


#define NUM_THREADS 4
#define ARRAY_SIZE 10000

int global_array[ARRAY_SIZE];


void initialize_array(){
    for (int i = 0;  i<ARRAY_SIZE ;i++){
        global_array[i] = rand() % 1000;
    }
}

void *sum_array(void* args){ 
    int thread_id = *(int*)args;
    int start = thread_id * (ARRAY_SIZE/NUM_THREADS);
    int end = start + (ARRAY_SIZE/NUM_THREADS);
    int sum = 0;
    for(int i =start;i<end;i++){
            sum += global_array[i];
        }
    return (void*)(long)sum;
}



int main(){

    pthread_t pthread_threads[NUM_THREADS];
    int pthread_args[NUM_THREADS];
    void* pthread_results[NUM_THREADS];
    long total_sum = 0;

    initialize_array();
    for(int i = 0;i<NUM_THREADS; i++){
        pthread_args[i] = i;
        pthread_create(&pthread_threads[i], NULL,sum_array, (void*)&pthread_args[i]);
    }
    for(int i = 0;i<NUM_THREADS; i++){
       pthread_join(pthread_threads[i], &pthread_results[i]);
       total_sum += (long)pthread_results[i];
       printf("sum : %ld\n", total_sum);
    }

    return 0;
}