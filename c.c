#include "c.h"


void createAlarmInstance(int pid, int duration) {
    while (1) {
        pid = fork();
        if (pid == 0) {
            sleep(duration);
            printf("ALARM from pid %d \a\n", getpid());
            exit(0);
        } else {
            printf("New child: %d, pid");
        }
    }
}