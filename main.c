#include "functions.h"
#include "sound.h"

void meny_system(){
    char time_str[20];
    printf("Welcome to the alarm clock! ");
    printf("It is currently %s\n", parse_seconds(time_str, time(NULL))); //fikse tidspunkt + dato for nå
    printf("Please enter \"s\" (schedule), \"l\" (list), \"c\" (cancel),  \"p\" (play songs), \"x\" (exit)\n");
    int status=1;
    struct Alarm alarms[10];
    for (int i; i < 10; i++){
        alarms[i].active = 0;
    }
    int index_stack[10] = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int top = 9;
    while(status){
        char choice = getchar();
        for (int i = 0; i < 10; i++) {
            if (alarms[i].active && alarms[i].seconds < time(NULL)) {
                waitpid(alarms[i].process_id, NULL, 0);
                remove_alarm(alarms, i, index_stack, &top);
            }
        }
        switch(choice)
        {
            case 's':
                if (top==-1)
                {
                    printf("Sorry, you have reached the maximum number of alarms.\n");
                    break;
                }
                
                printf("Schedule alarm at which date and time?: ");
                char time_string[20];
                getchar();
                scanf("%[^\n]%*c", time_string);
   
                int sound = choose_sound();
                if (parse_time(time_string) < time(NULL))
                {
                    printf("You cannot schedule an alarm in the past...\n");
                    break;
                }

                pid_t pid = create_alarm_instance(time_string, alarms, index_stack, &top, sound); //  lager child
                break;

            case 'l':
                 for (int i = 0; i < 10; i++)
                 {
                     if (alarms[i].active)
                     {
                         char alarm_time[20];
                         parse_seconds(alarm_time, alarms[i].seconds);
                         printf("Alarm %d scheduled at %s\n", i, alarm_time);
                     }
                 }
                 
                 break;

            case 'c':
                 printf("Cancel which alarm?\n");
                 getchar();
                 int index;
                 scanf("%d", &index);
                 if (index > 9 || !alarms[index].active)
                 {
                     printf("Invalid index.\n");
                     break;
                 }
                 kill(alarms[index].process_id, 9);
                 waitpid(alarms[index].process_id, NULL, 0);
                 remove_alarm(alarms, index, index_stack, &top);
                 break;

            case 'p':
                play_sounds();
                break;

            case 'x':
                printf("Goodbye!\n");
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
    meny_system();
}
