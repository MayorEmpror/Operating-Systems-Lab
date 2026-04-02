#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>


#define FIFO_FILE "/tmp/myfifo"

int main(){
    int fd;
    int input;
    ssize_t num_bytes;
    mkfifo(FIFO_FILE, O_WRONLY);
    fd = open(FIFO_FILE, O_WRONLY);

    if(fd == -1){
            perror("open");
            exit(EXIT_FAILURE);
        }
    while(1){
        printf("Producer : Enter a number or type 1 to end ");
        scanf("%d", &input);

        num_bytes = write(fd, &input, sizeof(input));
        
        if(num_bytes == -1){
            perror("write");
            exit(EXIT_FAILURE);
        }
        if (input == 1){
            break;
        }
    }
    close(fd);
    unlink(FIFO_FILE);
    return 0 ;
}
