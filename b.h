#define _XOPEN_SOURCE
#include <time.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdbool.h>

struct alarm {
    time_t seconds;
    pid_t process_id;
    bool active;
    int song;

};

time_t parse_time(char time_string[]);
void add_alarm(struct alarm *a, pid_t pid, int seconds); 
void remove_alarm(struct alarm *a, pid_t pid);
char *parse_seconds(char str[],time_t seconds);