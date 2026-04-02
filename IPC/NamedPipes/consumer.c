#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_FILE "/tmp/myfifo"

int main() {
    int fd;
    char buffer[BUFSIZ];
    ssize_t num_bytes;

    fd = open(FIFO_FILE, O_RDONLY); // Open the named pipe for reading (consumer)

    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    printf("Consumer: Waiting for messages...\n");

    while (1) { // Consumer loop
        num_bytes = read(fd, buffer, sizeof(buffer)); // Read from the named pipe

        if (num_bytes == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (num_bytes == 0) {
            // The writer has closed the pipe
            break;
        }

        printf("Consumer: Received Message: %.*s", (int)num_bytes, buffer);
    }

    close(fd); // Close the named pipe

    return 0;
}

