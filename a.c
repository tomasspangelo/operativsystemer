#include "a.h"

void menySystem(){
    char time_str[20];
    printf("Welcome to the alarm clock! ");
    printf("It is currently %s\n", parse_seconds(time_str, time(NULL))); //fikse tidspunkt + dato for nå
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel), \"x\" (exit)\n");
    int status=1;
    struct alarm alarms[10];
    int index_stack[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int top = 9;
    while(status){
        char choice = getchar();
        switch(choice)
        {
            case 's': 
                printf("Schedule alarm at which date and time?: ");
                char time_string[20];
                getchar();
                scanf("%[^\n]%*c", time_string);
                pid_t pid = createAlarmInstance(time_string, alarms, index_stack, &top); //  lager child
                break;
            case 'l':
                 for (int i = 0; i < 10; i++)
                 {
                     bool active = true;
                     for (int j = 0; j <= top; j++)
                     {
                         if (index_stack[j] == i){
                             active = false;
                             break;
                         }
                     }
                     if (active)
                     {
                         char alarm_time[20];
                         parse_seconds(alarm_time, alarms[i].seconds);
                         printf("Alarm %d scheduled at %s\n", i, alarm_time);
                     }
                     
                     
                 }
                 
                 break;
            case 'c':
                 printf("Cancel which alarm?\n");
                 // killAlarm(indeks for alarm) //funksjon for å avslutte alarm
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
