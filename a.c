#include "a.h"

void menySystem(){
     printf("Welcome to the alarm clock!");
    printf("It is currently\n"); //fikse tidspunkt + dato for nå
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)\n");
    int status=1;
    while(status){
        char choice = getchar();
        switch(choice){
            case 's':
                printf("Schedule alarm at which date and time?\n");
                break;
                //funksjon for å legge inn alarm
            case 'l':
                 //list comands
                 break;
            case 'c':
                 printf("Cancel which alarm?\n");
                 //funksjon for å avslutte alarm
                 break;
            case 'x':
                printf("Goodbye!");
                status= 0;
                break;
            case '\n': break; //Ignore enter
            default:
                printf("Not an option.\n");
                printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)\n");
        }
    }
}
