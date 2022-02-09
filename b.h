#define _XOPEN_SOURCE
#include <time.h>
#include <sys/types.h>
#include <stdio.h>

struct alarm {
    time_t seconds;
    pid_t process_id;

};

time_t parse_time(char time_string[]);
void add(struct alarm *a, int pid, char time_string[]);
