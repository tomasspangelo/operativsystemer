#ifndef WORKER_H
#define WORKER_H
#include <sys/socket.h> //basic socket 
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> //inet_pton
//#include <stgarg.h> //err_n_die trengerikke?
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include <string.h>
#include <sys/stat.h> // stat()

void *work(void *ptr);
void error(const char *msg);

#endif