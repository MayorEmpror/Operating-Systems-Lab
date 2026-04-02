#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>

#define FIFO_NAME "/tmp/myfifo"

int main()
{
    int pid;
    int fd;
    int num;

    mkfifo(FIFO_NAME,0666);

    pid=fork();

    if(pid<0)
    {
        printf("Fork failed\n");
        return 1;
    }

    else if(pid==0)
    {
        // Child reader

        fd=open(FIFO_NAME,O_RDONLY);

        read(fd,&num,sizeof(num));

        printf("Child received number: %d\n",num);

        close(fd);
    }

    else
    {
        // Parent writer

        printf("Enter number: ");
        scanf("%d",&num);

        fd=open(FIFO_NAME,O_WRONLY);

        write(fd,&num,sizeof(num));

        close(fd);

        printf("Parent sent number\n");
    }

    return 0;
}