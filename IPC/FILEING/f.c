#include <stdio.h>
#include <fcntl.h>      
#include <unistd.h>     
#include <string.h>     

int main() {

    int fd;  
    char write_data[] = "Hello Students! This is File Management in OS.\n";
    char read_data[100];

    
    // STEP 1: OPEN / CREATE FILE FOR WRITING
    
    fd = open("myfile.txt", O_CREAT | O_WRONLY, 0644);

    if (fd < 0) {
        printf("Error opening file for writing\n");
        return 1;
    }

    printf("File opened for writing.\n");

   
    // STEP 2: WRITE INTO FILE
   
    write(fd, write_data, strlen(write_data));
    printf("Data written successfully.\n");

    
    // STEP 3: CLOSE FILE AFTER WRITING
    
    close(fd);
    printf("File closed after writing.\n");



    
    // STEP 4: OPEN FILE AGAIN FOR READING
   
    fd = open("myfile.txt", O_RDONLY);

    if (fd < 0) {
        printf("Error opening file for reading\n");
        return 1;
    }

    printf("File opened for reading.\n");

    
    // STEP 5: READ FROM FILE
   
    read(fd, read_data, sizeof(read_data));
    printf("Data read from file:\n%s", read_data);

   
    // STEP 6: CLOSE FILE AFTER READING
   
    close(fd);
    printf("File closed after reading.\n");

    return 0;
}