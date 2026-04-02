#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errorno.h>
#include <sys/Types.h>
#include <sys/Wait.h>
#include <sys/stat.h>


#define MAX_STS_LEN 100
#define FIFO_NAME "/tmp/com_channel"

int main(){
    int pid;
    int fd;
    int msg[MAX_STS_LEN];

    mkfifo(FIFO_NAME, msg, sizeof(msg));
    pid = fork();
    if(pid < 0){
            printf("Error while commiting fork\n");
            return 0;
    }
    if(pid == 0){
            fd = open(MAX_STS_LEN, )
    }
    return 0;
}

