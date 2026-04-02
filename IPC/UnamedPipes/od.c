#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd[2];
    pid_t pid;
    char write_msg[] = "Hello Students";
    char read_msg[50];

    pipe(fd);   // create pipe

    pid = fork();   // create child process

    if (pid > 0) {   // Parent process
        close(fd[0]);  // close reading end
        write(fd[1], write_msg, strlen(write_msg)+1);
        close(fd[1]);
    }
    else {   // Child process
        close(fd[1]);  // close writing end
        read(fd[0], read_msg, sizeof(read_msg));
        printf("Message received: %s\n", read_msg);
        close(fd[0]);
    }

    return 0;
}


