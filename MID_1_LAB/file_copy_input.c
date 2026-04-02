#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

int copy_file_content(char* src, char* dest) {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        // Child → write to destination
        close(fd[1]);

        char buffer[200];
        int bytes = read(fd[0], buffer, 200);

        int out = open(dest, O_CREAT | O_WRONLY | O_TRUNC, 0666);
        write(out, buffer, bytes);

        close(out);
        close(fd[0]);
    } else {
        // Parent → read from source
        close(fd[0]);

        char buffer[200];
        int in = open(src, O_RDONLY);
        int bytes = read(in, buffer, 200);

        write(fd[1], buffer, bytes);

        close(in);
        close(fd[1]);
        wait(NULL);
    }
    return 0;
}

int main(int argc, char* argv[]) {
    copy_file_content(argv[1], argv[2]);
    printf("done\n");
    return 0;
}