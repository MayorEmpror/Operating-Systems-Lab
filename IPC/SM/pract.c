#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define file_par 'parrent_buffer.txt'// parrent to child
#define  file_child 'child_buffer.txt' // child to parrent 

typedef struct {
    char name[20];
    int quantity;
    float cost;
   
} product;


int main(){

    pid_t pid;
    int fd;
    product[];


    mkFIFO(file_par, 0777);
    mkFIFO(file_child, 0777);

    product inventory[3] = {
        {"Boxes", 50, 34.00},
        {"Boxes", 50, 34.00},
        {"Boxes", 50, 34.00}
    }



    pid_t pid = fork();

    if(pid == 0){
        //child process
        products inventory_child[3];
        int shipped[3], remaining[3];
        int parrent_to_child = open(file_par, O_RDONLY);
        read(parrent_to_child, inventory_child, sizeof(inventory_child))
        close(parrent_to_child)

        for(int i = 0; i < 3; i++){
            shipped[i] = inventory_child[i].quantity * 0.30 // ships 1/3 of the total products
            remaining[i] = inventory_child[i].quantity - shipped[i];
        }

        int child_to_parrent = open(file_child, O_WRONLY);
        write(child_to_parrent, shipped, sizeof(shipped) );
        write(child_to_parrent, remaining, sizeof(remaining));
        close(child_to_parrent);


    }else {
        int parrent_to_child = open(file_par, O_WRONLY);
        write(parrent_to_child, inventory, sizeof(inventory));
        close(parrent_to_child);

        int shipped, remaining;
        int child_to_parrent = open(file_child, O_RDONLY);
        read(child_to_parrent, shipped, sizeof(shipped));
        read(child_to_parrent, remaining, sizeof(remaining));
        close(child_to_parrent)

        float st_cost[3], shpmt_percent[3];
        for (int i = 0; i < 3; i++){
            st_cost[i] = inventory[i].cost * remaining[i];
            shpmt_percent[i] = shipped[i]/inventory[i].quantity * 100;
            printf("Merchandise storage cost : %f ", st_cost[i]);
            printf("Merchnadise shipment percentage: %f ", shpmt_percent[i]);
        }
        
    }


    return 0;
}


