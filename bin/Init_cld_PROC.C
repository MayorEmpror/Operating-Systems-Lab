#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
      
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
       
        printf("Child Process: Listing current directory contents:\n");

       
        execlp("ls", "ls", NULL);

      
        perror("exec failed");
        exit(1);
    }
    else {
        
        wait(NULL); 
        printf("Parent Process: Child has terminated.\n");
    }

    return 0;
}