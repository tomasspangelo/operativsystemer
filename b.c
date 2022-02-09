#include "b.h"

time_t parse_time(char time_string[]){
    struct tm t;
    strptime(time_string, "%Y-%m-%d %H:%M:%S", &t);
    return mktime(&t);
}

char *parse_seconds(char str[],time_t seconds) {
    struct tm* tm_info = localtime(&seconds);
    strftime(str, 20, "%Y-%m-%d %H:%M:%S", tm_info);
    return str;
}

void add_alarm(struct alarm *a, pid_t pid, char time_string[]) {
    struct alarm new_alarm = {.seconds = parse_time(time_string), .process_id = pid, .active = 1};
    a[pid] = new_alarm;

}

void remove_alarm(struct alarm *a, pid_t pid) {
    a[pid].active = 0;
}