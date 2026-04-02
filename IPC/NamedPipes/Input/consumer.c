#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#define FIFO_NAME "/tmp/myfifo"

int main(){

   int fd;
   int num;
   ssize_t num_bytes;

   mkfifo(FIFO_NAME, 0666);   // create FIFO

   fd = open(FIFO_NAME, O_RDONLY);

   if(fd == -1){
        perror("open");
        exit(1);
   }

   while(1){

        num_bytes = read(fd, &num, sizeof(num));

        if(num_bytes <= 0)
            break;

        printf("Consumer received: %d\n", num);
   }

   close(fd);

   return 0;
}