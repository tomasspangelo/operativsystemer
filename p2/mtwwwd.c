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

// Constants
#define PORT 7238 // Port number
#define THREADS 4 // Number of threads
#define MAXQUEUE 5 //Maximum queue of requests before rejecting

int main() {
    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    // Declare array for threads
    pthread_t threads[THREADS];
    int irets[THREADS];

    // Initialize bounded ring buffer
    BNDBUF *bb = bb_init(THREADS);

    // Decklare socket variables
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    
    // Create an endpoint for communication and store file descriptor
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) error("ERROR opening socket");

    //zero out socket
    bzero((char *) &serv_addr, sizeof(serv_addr));

    //reserve from all ports
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);
    
    //bind here
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
        }
    // Listen to port 
    listen(sockfd, MAXQUEUE);
    
    // Create threads
    for (int i = 0; i < THREADS; i++) {
        irets[i] = pthread_create(&(threads[i]), NULL, work, (void*) bb);
    }
   
    while (1) {
        clilen = sizeof(cli_addr);
        //accept block until incoming connection arrives, another socket
        newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr,
        &clilen);
        if (newsockfd < 0) error("ERROR on accept");

        // Add file descriptor of socket to bounded ring buffer
        bb_add(bb, newsockfd);
    }
}