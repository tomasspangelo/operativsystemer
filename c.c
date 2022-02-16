#ifndef "a.h"
#include "a.h"
#endif
#ifndef "b.h"
#include "b.h"
#endif
#ifndef "c.h"
#include "c.h"
#endif


pid_t createAlarmInstance(char *time_string, struct alarm *alarms) {
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
            printf("New child");
            add_alarm(alarms, pid, seconds);
        }
        return pid;
    }
}