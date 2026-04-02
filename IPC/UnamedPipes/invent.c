#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/types.h>




int main(){

    int shmid;
    float* stock_price;

    shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    if(shmid < 0){
        perror("shared memo init failed");
        return 1;
    }
    stock_price = (float*) shmat(shmid, NULL, 0);
    if (stock_price == (float*) -1){
        perror("shared memo linkage failed");
        return 1;
    }
    pid_t pid = fork();
    if(pid ==0){
        // trader process
        sleep(1);
        if(*stock_price <= 100.0){
            printf("buy");
        }else printf("hold");

    }else{
        // producer 
      
        *stock_price = 101.00;
    }
    shmdt(stock_price);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}