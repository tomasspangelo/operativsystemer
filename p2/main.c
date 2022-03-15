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


//constants

//not reserved
#define PORT 7227

#define MAXREQ (4096*1024)

char buffer[MAXREQ], body[MAXREQ], msg[MAXREQ];

void response (void *message, int msglen, int clientsocket)
{
    char *msg = (char*) message;

    while (msglen > 0)
    {
         int len = write(clientsocket, msg, msglen);
         if (len <= 0) return;
         msg += len;
         msglen -= len;
    }
}

void showerror(int sockfd){
    FILE *fp = fopen("webroot/404error.html", "r");
    char ch;

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        
        exit(EXIT_FAILURE);
    }
    struct stat st;
    stat("webroot/404error.html", &st);
    sprintf(msg, "HTTP/1.0 404 Not Found\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", "text/html", st.st_size);
    response(msg, strlen(msg), sockfd);
    long ret; 
    while ((ret = fread(body, 1, sizeof(body), fp)) > 0)  {
        response(body, ret, sockfd); 
    }
    fclose(fp);
}

void file_to_string(int sockfd, char *filepath, char *t) {
    char *start = "webroot";
    char str[8+ strlen(filepath)];
    strcpy(str, start);
    if (strcmp(filepath, "/")) {
        strcat(str, filepath);
    }
    else {
        strcat(str, "/index.html");
    }
        
    printf(str);
    printf("\n");
    FILE *fp = fopen(str, "r");
    char ch;

    if (fp == NULL)
    {
        //perror("Error while opening the file.\n");
        showerror(sockfd);
        return;
        //exit(EXIT_FAILURE);
    }
    char *type;
    if(!strcmp(t, ".html")){
        type = "text/html";
    }
    else if (!strcmp(t, ".jpg") || !strcmp(t, ".JPG")){
        type = "image/jpeg";
    }
    else if (!strcmp(t, ".ico")){
        type = "image/x-icon";
    }
    else if (!strcmp(t, ".pdf")) {
        type = "application/pdf";
    }
    else {
        type = "application/json";
    }
    struct stat st;
    stat(str, &st);
    sprintf(msg, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", type, st.st_size);
    response(msg, strlen(msg), sockfd);
    long ret; 
    while ((ret = fread(body, 1, sizeof(body), fp)) > 0)  {
        response(body, ret, sockfd); 
    }
    fclose(fp);
}


void parse(){
    
    const char *start_of_path = strchr(buffer, '/') +1;
    const char *end_of_path = strchr(start_of_path, ' ');
    


    /* Get the right amount of memory */
    char path[end_of_path - start_of_path];

    /*Copy the strings into our memory */
    strncpy(path, start_of_path,  end_of_path - start_of_path);
    
    /* Null terminators (because strncpy does not provide them) */
    path[sizeof(path)] = 0;
    /*Print */

   
}
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
        
         const char *start_of_path = strchr(buffer, '/');
        const char *end_of_path = strchr(start_of_path, ' ');
    


        /* Get the right amount of memory */
        char path[end_of_path - start_of_path];

         /*Copy the strings into our memory */
        strncpy(path, start_of_path,  end_of_path - start_of_path);
        const char *start_of_type = strchr(buffer, '.');
        const char *end_of_type = strchr(start_of_type, ' ');
        char t[end_of_type - start_of_type];
        strncpy(t, start_of_type, end_of_type - start_of_type);
        t[sizeof(t)] = 0;
        printf("%s\n", t);
    
        /* Null terminators (because strncpy does not provide them) */
        path[sizeof(path)] = 0;
        /*Print */

        printf("%s\n", path, path);
        file_to_string(newsockfd, path, t);

    

       // printf("%s\n", body, sizeof(body));
    
        
        /*snprintf (body, sizeof (body),
            "<html>\n<body>\n"
            "<h1>Hello web browser</h1>\nYour request was\n"
            "<pre>%s</pre>\n"
            "</body>\n</html>\n", buffer);
        
            snprintf (msg, sizeof (msg),
                "HTTP/1.0 200 OK\n"
                "Content-Type: text/html\n"
                "Content-Length: %ld\n\n%s", strlen (body), body);
            n = write (newsockfd,msg,strlen(msg));
        */
        //Skriver OK melding, og Hello
        

        
        if (n < 0) error("ERROR writing to socket");
        close (newsockfd);
        }
}