#include <stdio.h>

int addbook(char* name, int id){
    FILE* file = fopen("db.txt", "a");  // use append mode

    if(file == NULL){
        printf("Error: file refused to open.\n");
        return 1;
    }

    fprintf(file, "Name : %s", name);
    fprintf(file, "ID   : %d", id);
    fprintf(file, "-----------------\n");

    fclose(file);

    return 0;
}