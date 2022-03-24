#ifndef MTWWWD_H
#define MTWWWD_H
#include <sys/socket.h> //basic socket 
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> 
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h> // stat()
#include <pthread.h>

#include "bbuffer.h"
#include "sem.h"

typedef struct WorkerArgument {
    BNDBUF *bb;
    char *start;
} WorkerArgument;


void response (void *message, int msglen, int sockfd);
void showerror(int sockfd, char *buffer);
void process_request(int sockfd, char *filepath, char *t, char* buffer, char *start);

void *work(void *ptr);
void error(const char *msg);

#endif