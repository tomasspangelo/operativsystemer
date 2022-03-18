#include <sys/socket.h> //basic socket 
#include <pthread.h>
#include <sys/types.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h> //inet_pton
#include <fcntl.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netdb.h>
#include "worker.h"
#include "bbuffer.h"
#include "sem.h"

//constants
//not reserved
#define PORT 7229
#define THREADS 4

int main() {
    pthread_t threads[THREADS];
    int irets[THREADS];
    BNDBUF *bb = bb_init(THREADS);
    printf("%d\n", bb -> size);
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    printf("FY FAEN I HELVETE\n");
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");
    //zero out socket
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //reserve from all ports
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    printf("FY FAEN I HELVETE\n");
    //bind here
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
        }
    //open port and accept connections
    printf("FY FAEN I HELVETE\n");
    listen(sockfd,5);
    for (int i = 0; i < THREADS; i++) {
        irets[i] = pthread_create(&(threads[i]), NULL, &work, (void*) bb);
    }

    printf("FY FAEN I HELVETE\n");
    while (1) {
        printf("INSH ALLAH0\n");
        clilen = sizeof(cli_addr);
        //accept block until incoming connection arrives, another socket
        printf("INSH ALLAH\n");
        newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr,
        &clilen);
        printf("HELVETE\n");
        if (newsockfd < 0) error("ERROR on accept");
        bb_add(bb, newsockfd);
    }
}