#include "worker.h"
#include "bbuffer.h"
#define MAXREQ (1024)

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

void showerror(int sockfd, char *buffer){
    FILE *fp = fopen("webroot/404error.html", "r");
    char ch;

    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        
        exit(EXIT_FAILURE);
    }
    struct stat st;
    stat("webroot/404error.html", &st);
    sprintf(buffer, "HTTP/1.0 404 Not Found\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", "text/html", st.st_size);
    response(buffer, strlen(buffer), sockfd);
    long ret; 
    while ((ret = fread(buffer, 1, sizeof(buffer), fp)) > 0)  {
        response(buffer, ret, sockfd); 
    }
    fclose(fp);
}

void process_request(int sockfd, char *filepath, char *t, char* buffer) {
    char *start = "webroot";
    char str[8+ strlen(filepath)];
    strcpy(str, start);
    if (strcmp(filepath, "/")) {
        strcat(str, filepath);
    }
    else {
        strcat(str, "/index.html");
        t = ".html";
    }
        
    printf(str);
    printf("\n");
    FILE *fp = fopen(str, "r");
    char ch;

    if (fp == NULL)
    {
        //perror("Error while opening the file.\n");
        showerror(sockfd, buffer);
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
    sprintf(buffer, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", type, st.st_size);
    response(buffer, strlen(buffer), sockfd);
    long ret; 
    while ((ret = fread(buffer, 1, sizeof(buffer), fp)) > 0)  {
        response(buffer, ret, sockfd); 
    }
    fclose(fp);
}

void error(const char *msg) { perror(msg); exit(1); }

void *work(void *ptr){
    printf("hello thread\n");
    BNDBUF *bb = (BNDBUF*) ptr;
    char buffer[MAXREQ];
    while(1){
        int fd = bb_get(bb);
        bzero(buffer,sizeof(buffer));
        int n = read(fd,buffer,sizeof(buffer)-1);
        
        if (n < 0) error("ERROR reading from socket");
        
        const char *start_of_path = strchr(buffer, '/');
        const char *end_of_path = strchr(start_of_path, ' ');
    
        /* Get the right amount of memory */
        char path[end_of_path - start_of_path];

         /*Copy the strings into our memory */
        strncpy(path, start_of_path,  end_of_path - start_of_path);

        /* Null terminators (because strncpy does not provide them) */
        path[sizeof(path)] = 0;

        const char *start_of_type = strchr(buffer, '.');
        const char *end_of_type = strchr(start_of_type, ' ');
        char t[end_of_type - start_of_type];
        strncpy(t, start_of_type, end_of_type - start_of_type);
        t[sizeof(t)] = 0;
        printf("%s\n", t);
    
        /*Print */

        printf("%s\n", path, path);
        process_request(fd, path, t, buffer);
    
        close (fd);

    }
}

