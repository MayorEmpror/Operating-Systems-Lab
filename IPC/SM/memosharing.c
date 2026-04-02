#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <string.h>


int main(){
    char *ptr;
    int sharedmemo_id = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    
    if(sharedmemo_id < 0){
         perror("shmget failed ");
         exit(1);
    }

    ptr = shmat(sharedmemo_id, NULL, 0);
    if(ptr == (char* ) -1){
        perror("shmat failed");
        exit(1);
    }
    pid_t process_id = fork();
    if(process_id < 0){
        perror("fork failed ");
        exit(1);
    }else if(process_id == 0){
        sleep(2);
        int len = strlen(ptr);
        char *reversed_string = malloc(strlen(ptr)+1);
        for (int i = 0; i< len; i++){
               reversed_string[i] = ptr[(len - 1) - i];
        }
        printf("reversed the string : %s", reversed_string);
    }else{
        strcpy(ptr, "Hello nigga");
        printf("parrent wrote to the shm");
    }
    shmdt(ptr);
    shmctl(sharedmemo_id, IPC_RMID, NULL);
    return 0;
}