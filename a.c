#include "a.h"
#include "b.h"
#include "c.h"

void menySystem(){
    char time_str[20];
    printf("Welcome to the alarm clock! ");
    printf("It is currently %s\n", parse_seconds(time_str, time(NULL))); //fikse tidspunkt + dato for nå
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)\n");
    int status=1;
    struct alarm alarms[10];
    while(status){
        char choice = getchar();
        switch(choice)
        {
            case 's': 
                printf("Schedule alarm at which date and time?: ");
                char time_string[20];
                getchar();
                scanf("%[^\n]%*c", time_string);
                pid_t pid = 0; // bytt ut 0 med det som returneres fra fork (eventuelt kan logikken flyttes til add_alarm)
                add_alarm(alarms, pid, time_string);
                createAlarmInstance(pid, alarms[pid].seconds - time(NULL)); //  lager child
                printf("Scheduling alarm in %ld seconds\n", alarms[pid].seconds - time(NULL));
                break;
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
            case '\n':
                break; //Ignore enter
            default:
                printf("Not an option.\n");
        }
    }
}

int main() {
    menySystem();
}
