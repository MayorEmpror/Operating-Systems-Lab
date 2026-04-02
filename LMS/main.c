#include <stdio.h>
#include "lib.h"
#include "issue.h"

void print_menu(){
    printf("\n===== Library Menu =====\n");
    printf("1) Add Book\n");
    printf("2) Issue Book\n");
    printf("0) Exit\n");
    printf("Enter choice: ");
}

int main(){

    int choice;
     
    do {
        print_menu();
        scanf("%d", &choice);
        getchar(); // clear newline
        
        switch(choice){

            case 1: {

                char name_book[100];
                int id;
                printf("Enter book name: ");
                fgets(name_book, sizeof(name_book), stdin);
                printf("Enter book id: ");
                scanf("%d", &id);
                getchar();
                addbook(name_book, id);
                break;
            }

            case 2: {

                char book_name[100];
                char std_name[100];
                int id_book;
                int id_std;
                printf("Enter Student id: ");
                scanf("%d", &id_std);
                getchar();
                printf("Enter Student name: ");
                fgets(std_name, sizeof(std_name), stdin);
                printf("Enter Book id: ");
                scanf("%d", &id_book);
                getchar();
                printf("Enter Book name: ");
                fgets(book_name, sizeof(book_name), stdin);

                int status = issue_book(book_name, id_std, id_book, std_name);

                if(status == -1){
                    printf("Book not found!\n");
                }
                else if(status == 2){
                    printf("Database error!\n");
                }

                break;
            }

            case 0:
                printf("Exiting...\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } while(choice != 0);

    return 0;
}