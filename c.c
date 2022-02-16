#include "a.h"

pid_t createAlarmInstance(char *time_string, struct alarm *alarms, int *index_stack, int *top) {
    while (1) {
        pid_t pid;
        time_t seconds = parse_time(time_string);
        time_t seconds_delta = seconds - time(NULL);
        printf("Scheduling alarm in %ld seconds\n", seconds_delta);
        pid = fork();
        if (pid == 0) {
            sleep(seconds_delta);
            printf("RING! \a\n");
            exit(0);
        } else {
            add_alarm(alarms, pid, seconds, index_stack, top);
        }
        return pid;
    }
}