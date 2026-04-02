#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#define FIFO "/tmp/myfifo"

struct data
{
    int num1;
    int num2;
    int choice;
};

int main()
{
    int fd;

    struct data d;

    fd=open(FIFO,O_RDONLY);

    read(fd,&d,sizeof(d));

    if(d.choice==1)
    printf("Result: %d\n",d.num1+d.num2);

    else if(d.choice==2)
    printf("Result: %d\n",d.num1*d.num2);

    else
    printf("Invalid choice\n");

    close(fd);

    return 0;
}