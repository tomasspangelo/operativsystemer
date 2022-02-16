#include <unistd.h> //fork()
#include <stdlib.h> //exit()
#pragma once

pid_t createAlarmInstance(char *time_string, struct alarm *alarms);