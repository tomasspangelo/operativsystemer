#ifndef FLUSH_H
#define FLUSH_H
#include <unistd.h> //fork()
#include <stdlib.h> //exit()
#include <signal.h> //kill()

#define MAX_PATH 200

//Process struct with information about the process
struct Process {
    pid_t process_id;
    int active;
    char command[MAX_PATH];
};
#endif