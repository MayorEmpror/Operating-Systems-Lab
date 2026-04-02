#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main()
{
    int fd[2];
    int pid;

    char message[50];
    char buffer[50];

    pipe(fd);

    pid=fork();

    if(pid>0)
    {
        // Parent

        printf("Enter string (max 50): ");
        fgets(message,50,stdin);

        write(fd[1],message,strlen(message)+1);

        close(fd[1]);
    }

    else
    {
        // Child

        read(fd[0],buffer,sizeof(buffer));

        int len=strlen(buffer);

        printf("Reversed string: ");

        for(int i=len-2;i>=0;i--)
        printf("%c",buffer[i]);

        printf("\n");

        close(fd[0]);
    }

    return 0;
}