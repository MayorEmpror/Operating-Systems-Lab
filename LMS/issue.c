#include <stdio.h>
#include "issue.h"
#include <string.h>

int search_book(int book_id){
    FILE *file = fopen("db.txt", "r");

    if(file == NULL){
        return 2;
    }

    char line[256];
    int id;

    while(fgets(line, sizeof(line), file) != NULL){

        // Match your exact format: "ID   : %d"
        if(sscanf(line, "ID   : %d", &id) == 1){
            if(id == book_id){
                fclose(file);
                return 1;  // found
            }
        }
    }

    fclose(file);
    return 0; // not found
}

int issue_book(char* bookname, int student_id, int book_id, char* student_name){

    int search_status = search_book(book_id);

    if(search_status == 0){
        return -1; // not found
    }
    else if(search_status == 2){
        return 2; // file error
    }

    FILE *file = fopen("Issuedbooks.txt", "a");

    if(file == NULL){
        return 2;
    }

    fprintf(file, "Student Name: %s", student_name);
    fprintf(file, "Student ID: %d\n", student_id);
    fprintf(file, "Book ID: %d\n", book_id);
    fprintf(file, "Book Name: %s", bookname);
    fprintf(file, "----------------------------------------\n");

    fclose(file);
    return 0;
}