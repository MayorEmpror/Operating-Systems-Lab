#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define PIPE1 "/tmp/pipe_p2c"  // parent to child
#define PIPE2 "/tmp/pipe_c2p"  // child to parent

typedef struct {
    char name[20];
    int quantity;
    float cost_per_unit;
} Product;

int main() {
    // Create named pipes
    mkfifo(PIPE1, 0666);
    mkfifo(PIPE2, 0666);

    Product inventory[3] = {
        {"Boxes",      100, 2.5},
        {"Containers",  50, 5.0},
        {"Packages",    80, 3.0}
    };

    pid_t pid = fork();

    if (pid == 0) {
        // ── CHILD ──
        Product products[3];
        int shipped[3], remaining[3];

        // Read from parent
        int fd1 = open(PIPE1, O_RDONLY);
        read(fd1, products, sizeof(products));
        close(fd1);

        // Process
        for (int i = 0; i < 3; i++) {
            shipped[i]   = products[i].quantity * 0.30;
            remaining[i] = products[i].quantity - shipped[i];
            printf("[Child] %-12s Shipped: %d  Remaining: %d\n",
                   products[i].name, shipped[i], remaining[i]);
        }

        // Send back to parent
        int fd2 = open(PIPE2, O_WRONLY);
        write(fd2, shipped,   sizeof(shipped));
        write(fd2, remaining, sizeof(remaining));
        close(fd2);

    } else {
        // ── PARENT ──
        int shipped[3], remaining[3];

        // Send inventory to child
        int fd1 = open(PIPE1, O_WRONLY);
        write(fd1, inventory, sizeof(inventory));
        close(fd1);

        // Read results from child 
        int fd2 = open(PIPE2, O_RDONLY);
        read(fd2, shipped,   sizeof(shipped));
        read(fd2, remaining, sizeof(remaining));
        close(fd2);

        // Display results
        printf("\n[Parent] Warehouse Report:\n");
        printf("%-12s %8s %9s %13s %12s\n",
               "Product", "Initial", "Remaining", "StorageCost", "Shipment%");
        printf("------------------------------------------------------------\n");

        for (int i = 0; i < 3; i++) {
            float cost    = inventory[i].cost_per_unit * remaining[i];
            float percent = ((float)shipped[i] / inventory[i].quantity) * 100;
            printf("%-12s %8d %9d %13.2f %11.2f%%\n",
                   inventory[i].name, inventory[i].quantity,
                   remaining[i], cost, percent);
        }

        // Cleanup
        unlink(PIPE1);
        unlink(PIPE2);
    }

    return 0;
}