#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>


int main(){
    int fd;
    char write_data[] = "hello world";
    char read_data[100];
    fd = open("myfile.txt", O_CREAT | O_WRONLY, 0664);
    if(fd < 0){
        perror("error while opening file");
        return 1;
    }
    write(fd, write_data, strlen(write_data));
    printf("data written");
    close(fd);


    fd = open("myfile.txt", O_RDONLY);
    if (fd < 0){
        perror("error while opening the file");
        return 1;
    }
    read(fd, read_data, sizeof(read_data));
    printf("data read: %s", read_data);
    close(fd);
    printf("done with the process");
}
