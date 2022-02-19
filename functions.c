#include "functions.h"
#include "sound.h"

time_t parse_time(char time_string[]){
    struct tm t;
    t.tm_isdst = -1;
    strptime(time_string, "%Y-%m-%d %H:%M:%S", &t);
    return mktime(&t);
}

char *parse_seconds(char str[],time_t seconds) {
    struct tm* tm_info = localtime(&seconds);
    strftime(str, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    return str;
}

void add_alarm(struct Alarm *a, pid_t pid, int seconds, int *index_stack, int *top) {
    struct Alarm new_alarm = {.seconds = seconds, .process_id = pid, .active = 1};
    a[index_stack[*top]] = new_alarm;
    (*top) --;

}

void remove_alarm(struct Alarm *a, int index, int *index_stack, int *top) {
    a[index].active = 0;
    (*top) ++;
    index_stack[*top] = index;
}

pid_t create_alarm_instance(char *time_string, struct Alarm *alarms, int *index_stack, int *top,int song) {
    while (1) {
        pid_t pid;
        time_t seconds = parse_time(time_string);
        time_t seconds_delta = seconds - time(NULL);
        printf("Scheduling alarm in %ld seconds\n", seconds_delta);
        pid = fork();
        if (pid == 0) {
            sleep(seconds_delta);

            printf("Ring!\n");
            play_song(song);
            exit(0);
        } else {
            add_alarm(alarms, pid, seconds, index_stack, top);
        }
        return pid;
    }
}