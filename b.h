#define _XOPEN_SOURCE
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct alarm {
    time_t seconds;
    pid_t process_id;
    bool active;

};

time_t parse_time(char time_string[]);
void add_alarm(struct alarm *a, int pid, char time_string[]);
void remove_alarm(struct alarm *a, int pid);
