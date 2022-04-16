#ifndef FLUSH_H
#define FLUSH_H
#include <unistd.h> //fork()
#include <stdlib.h> //exit()
#include <signal.h> //kill()
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_PATH 200

//Process struct with information about the process
struct Process {
    pid_t process_id;
    int active;
    char argv[MAX_PATH][MAX_PATH];
    int size;
};

void printFilepath( char *path );
void add_process(struct Process *p, pid_t pid, int *index_stack, int *top, char argv[MAX_PATH][MAX_PATH], int size);
struct Process remove_process(struct Process *p, pid_t pid, int *index_stack, int *top);
void create_process(struct Process *p, int *index_stack, int *top, char *command, int size, char argv[MAX_PATH][MAX_PATH]);

#endif