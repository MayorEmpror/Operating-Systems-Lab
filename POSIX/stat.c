#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define num_Threads 3
#define size_arr 7


int global_array[] = {90, 81, 78, 95, 79, 72, 85};



void* calc_min(void* args){
 
    int local_min = global_array[0];

    for(int i = 0; i < size_arr; i++){
        if(local_min > global_array[i]){
            local_min = global_array[i];
        }
      
    }
    return (void*)(long)local_min;
}

void* calc_max(void* args){
    int local_max = global_array[0];
    for(int i = 0; i<size_arr; i++){
     if( local_max <  global_array[i]){
        local_max = global_array[i];
     }
    }
    return (void*)(long)local_max;
}

void* calc_sum(void * args){  
    int local_sum = 0;
    for(int i = 0; i<size_arr; i++){
        local_sum += global_array[i];
    }
    return (void*)(long)local_sum;
}


int main(){
    void* global_sum;
    void* global_max;
    void* global_min;
    float avg;
    pthread_t thread_min, thread_max, thread_sum;
    
        pthread_create(&thread_min, NULL, calc_min, NULL );
        pthread_create(&thread_max, NULL, calc_max, NULL );
        pthread_create(&thread_sum, NULL, calc_sum, NULL );

        pthread_join(thread_min, &global_min);
        pthread_join(thread_max, &global_max);
        pthread_join(thread_sum, &global_sum);

        avg = (int)(long)global_sum/ size_arr;
        printf("min of giving array %ld \n", (long)global_min);
        printf("max of giving array %ld \n", (long)global_max);
        printf("avg of given array %2.f  \n", avg);

    return 0;
}