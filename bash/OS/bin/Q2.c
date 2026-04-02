#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

            int main(){
                pid_t P1_child = fork();
                if(P1_child < 0){ perror("fork failed...see -help");
                return 0;
                }
                if(P1_child == 0){
                    printf("child process :: P1_Child : %d /parrent_id, %d /self_id", getppid(), getpid());
                    exit(0);
                }
                if(P1_child > 0){
                    printf("Parent process :: P1_Child : %d /child_id, %d /self_id", getcid(), getpid());
                }
            }