#include "a.h"

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

void add_alarm(struct alarm *a, pid_t pid, int seconds, int *index_stack, int *top) {
    struct alarm new_alarm = {.seconds = seconds, .process_id = pid, .active = 1};
    a[index_stack[*top]] = new_alarm;
    (*top) --;

}

void remove_alarm(struct alarm *a, int index, int *index_stack, int *top) {
    a[index].active = 0;
    (*top) ++;
    index_stack[*top] = index;
}