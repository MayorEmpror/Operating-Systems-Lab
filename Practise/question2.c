#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]){
    char* file_name =  argv[1];
    
    int fd[2];
    int fd_filesocket
    pid_t pid;

    char *write_buffer =argv[2];
    char *read_buffer
    
    pipe(fd)
    pid = fork();

    if(pid < 0){
        printf("fork request faild ");
        return 1;
    }
    else if(pid == 0){
        close(fd[0]);
        write(fd[1], write_buffer, strlen(write_buffer));
        fd_filesocket = open(file_name, O_CREAT | O_WRONLY, 0664);
        if(fd_filesocket < 0){
            perror("file refused to open");
            close(fd[1]);
            return 1;

        }
            write(fd_filesocket, write_buffer, strlen(write_buffer));
            close(fd_filesocket)
        close(fd[1]);

    }else {
        close(fd[0]);
        write(fd[1], read_buffer, sizeof(read_buffer));
        fd_filesocket = open(file_name, O_RDONLY);
        if (fd_filesocket < 0){
            perror("file refused to open");
            close(fd[0]);
            return 1;
        }
        read(fd_filesocket, read_buffer, sizeof(read_buffer))
        printf("content: %s", read_buffer);
        close(fd[1]);
    }
    return 0;
}