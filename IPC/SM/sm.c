#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

int main() {
    int shmid;
    char *ptr;

    // Shared memory create (size 1024 bytes)
    shmid = shmget(IPC_PRIVATE, 1024, 0666 | IPC_CREAT);
    if (shmid < 0) {
        perror("shmget failed");
        exit(1);
    }

    // Attach shared memory
    ptr = (char*) shmat(shmid, NULL, 0);
    if (ptr == (char*) -1) {
        perror("shmat failed");
        exit(1);
    }

    pid_t pid = fork();

    if (pid == 0) {
        // Child Process
        sleep(1); // Wait for parent to write
        printf("Child read from shared memory: %s\n", ptr);
    } else {
        // Parent Process
        strcpy(ptr, "Hello from Parent Process!");
        printf("Parent wrote to shared memory.\n");
    }

    // Detach memory
    shmdt(ptr);

    // Delete shared memory
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
