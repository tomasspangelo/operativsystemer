#ifndef "a.h"
#include "a.h"
#endif
#ifndef "b.h"
#include "b.h"
#endif
#ifndef "c.h"
#include "c.h"
#endif

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

void add_alarm(struct alarm *a, pid_t pid, int seconds) {
    struct alarm new_alarm = {.seconds = seconds, .process_id = pid, .active = 1};
    a[pid] = new_alarm;

}

void remove_alarm(struct alarm *a, pid_t pid) {
    a[pid].active = 0;
}