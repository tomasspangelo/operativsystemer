#include "worker.h"
#include "bbuffer.h"

//Maxmim buffer size for a worker thread
#define MAXREQ (1024)

/*
Write the response to the socket indicated by the file descriptor
sockfd.
*/
void response (void *message, int msglen, int sockfd)
{
    char *msg = (char*) message;

    // write until the entire message is written to sockfd.
    while (msglen > 0)
    {
         int len = write(sockfd, msg, msglen);
         if (len <= 0) return;
         msg += len;
         msglen -= len;
    }
}

/*
Function for responding with 404 error.
*/
void showerror(int sockfd, char *buffer){
    // Open 404 html file
    FILE *fp = fopen("webroot/404error.html", "r");
     // If error
    if (fp == NULL)
    {
        perror("Error while opening the file.\n");
        
        exit(EXIT_FAILURE);
    }

    // use stat() system call to get length of file
    struct stat st;
    stat("webroot/404error.html", &st);

    // Write header to socket
    sprintf(buffer, "HTTP/1.0 404 Not Found\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", "text/html", st.st_size);
    response(buffer, strlen(buffer), sockfd);

    // Write body to socket
    // Note: will take multiple iterations if filesize is larger than buffer
    long ret; 
    while ((ret = fread(buffer, 1, sizeof(buffer), fp)) > 0)  {
        response(buffer, ret, sockfd); 
    }

    // Close file
    fclose(fp);
}

/*
Function for processing a request
sockfd: file descriptor (for socket)
t: string containing filetype
filepath: string indicating path to requested file
buffer: pointer to start of buffer
*/
void process_request(int sockfd, char *filepath, char *t, char* buffer) {

    // Start of path
    char *start = "webroot";

    // Path
    char str[8+ strlen(filepath)];
    strcpy(str, start);

    if (strcmp(filepath, "/")) {
        strcat(str, filepath);
    }
    // If requested path is "/", respond with index.html
    else {
        strcat(str, "/index.html");
        t = ".html";
    }
        
    printf(str);
    printf("\n");

    //Open file
    FILE *fp = fopen(str, "r");

    // If error
    if (fp == NULL)
    {
        showerror(sockfd, buffer);
        return;
    }

    // Get correct media type for response header
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

    // use stat() system call to get length of file
    struct stat st;
    stat(str, &st);

    // Write header to socket
    sprintf(buffer, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", type, st.st_size);
    response(buffer, strlen(buffer), sockfd);

    // Write body to socket
    // Note: will take multiple iterations if filesize is larger than buffer
    long ret; 
    while ((ret = fread(buffer, 1, sizeof(buffer), fp)) > 0)  {
        response(buffer, ret, sockfd); 
    }

    // Close file
    fclose(fp);
}

void error(const char *msg) { perror(msg); exit(1); }

/*
"Main" function for a worker thread.
ptr: pointer to bounded ring buffer
*/
void *work(void *ptr){
    printf("hello thread\n");

    // Cast ptr to pointer to a bounded ring buffer
    BNDBUF *bb = (BNDBUF*) ptr;

    // Declare buffer for the worker thread
    char buffer[MAXREQ];
    while(1){
        // get file descriptor from bounded ring buffer
        int fd = bb_get(bb);

        // Erase content in buffer
        bzero(buffer,sizeof(buffer));

        // Read request (from file descriptor)
        int n = read(fd,buffer,sizeof(buffer)-1);

        // If error
        if (n < 0) error("ERROR reading from socket");
        
        // Find start and end of path in request
        const char *start_of_path = strchr(buffer, '/');
        const char *end_of_path = strchr(start_of_path, ' ');
        // Get the right amount of memory
        char path[end_of_path - start_of_path];
        // Copy the path into our memory 
        strncpy(path, start_of_path,  end_of_path - start_of_path);
        // Null terminators (because strncpy does not provide them)
        path[sizeof(path)] = 0;

        // Find start and end of filename extension
        const char *start_of_type = strchr(buffer, '.');
        const char *end_of_type = strchr(start_of_type, ' ');
        // Get the right amount of memory
        char t[end_of_type - start_of_type];
        // Copy the file extension into our memory 
        strncpy(t, start_of_type, end_of_type - start_of_type);
        // Null terminators (because strncpy does not provide them)
        t[sizeof(t)] = 0;

        // Print
        printf("%s\n", t);
        printf("%s\n", path, path);

        // Process request
        process_request(fd, path, t, buffer);

        // Close socket
        close (fd);

    }
}

