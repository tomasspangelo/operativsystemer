#include <stdio.h>
#define _XOPEN_SOURCE
#include <time.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h> //fork()
#include <stdlib.h> //exit()
#include <signal.h> //kill()

struct alarm {
    time_t seconds;
    pid_t process_id;
    bool active;
};


time_t parse_time(char time_string[]);
void add_alarm(struct alarm *a, pid_t pid, int seconds, int *index_stack, int *top);
void remove_alarm(struct alarm *a, int index, int *index_stack, int *top);
char *parse_seconds(char str[],time_t seconds);

void menySystem();
pid_t createAlarmInstance(char *time_string, struct alarm *alarms, int *index_stack, int *top, int song);

void playSong(int song);
void playSounds();
int chooseSound();