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

//constants

//not reserved
#define PORT 7300

#define MAXREQ (4096*1024)

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];
//error function
void error(const char *msg) { perror(msg); exit(1); }
int main() {
    int sockfd, newsockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
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
    //open port and accept connections
    listen(sockfd,5);


    while (1) {
        clilen = sizeof(cli_addr);
        //accept block until incoming connection arrives, another socket
        newsockfd = accept (sockfd, (struct sockaddr *) &cli_addr,
        &clilen);
        if (newsockfd < 0) error("ERROR on accept");
        bzero(buffer,sizeof(buffer));
        n = read (newsockfd,buffer,sizeof(buffer)-1);

        if (n < 0) error("ERROR reading from socket");
        snprintf (body, sizeof (body),
            "<html>\n<body>\n"
            "<h1>Hello web browser</h1>\nYour request was\n"
            "<pre>%s</pre>\n"
            "</body>\n</html>\n", buffer);
        snprintf (msg, sizeof (msg),
            "HTTP/1.0 200 OK\n"
            "Content-Type: text/html\n"
            "Content-Length: %d\n\n%s", strlen (body), body);

        //Skrivel OK melding, og Hello
        

        n = write (newsockfd,msg,strlen(msg));
        if (n < 0) error("ERROR writing to socket");
        close (newsockfd);
        }
}