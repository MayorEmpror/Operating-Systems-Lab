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

    mkfifo(FIFO,0666);

    printf("Enter two numbers: ");
    scanf("%d %d",&d.num1,&d.num2);

    printf("1.Sum\n2.Product\nEnter choice: ");
    scanf("%d",&d.choice);

    fd=open(FIFO,O_WRONLY);

    write(fd,&d,sizeof(d));

    close(fd);

    printf("Data sent\n");

    return 0;
}