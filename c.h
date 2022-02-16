#include <unistd.h> //fork()
#include <stdlib.h> //exit()

pid_t createAlarmInstance(char *time_string, struct alarm *alarms);