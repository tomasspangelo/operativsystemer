#include "mtwwwd.h"


// Constants
#define MAXQUEUE (5) //Maximum queue of requests before rejecting
//Maxmim buffer size for a worker thread
#define MAXBUF (1024)
#define HTTP_VERSION (1)
// HTTP_VERSION=0: HTTP/0.9 <--- Will not send a header, only html file
// HTTP_VERSION=1: HTTP/1.0 <---- If you want header + body (jpg/pdf/json/ico/html)
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
void responderror(int sockfd, char *buffer){
    char *page_404 = "<html>\n<body>\n<h1>404 NOT FOUND</h1>\n</body>\n</html>\n";
 
    // Write header to socket
    if (HTTP_VERSION){
        sprintf(buffer, "HTTP/1.0 404 Not Found\r\n Content-Type: text/html\r\n Content-Length: %ld\r\n\r\n", strlen(page_404));
        response(buffer, strlen(buffer), sockfd);
    }
    strcpy(buffer, page_404);
    response(buffer, strlen(buffer), sockfd);
}

/*
Function for processing a request
sockfd: file descriptor (for socket)
t: string containing filetype
filepath: string indicating path to requested file
buffer: pointer to start of buffer
start: start of path for webroot
*/
void process_request(int sockfd, char *filepath, char *t, char* buffer, char *start) {

    // Path
    char str[strlen(start)+ strlen(filepath)];
    strcpy(str, start);

    if (strcmp(filepath, "/")) {
        strcat(str, filepath);
    }
    // If requested path is "/", respond with index.html
    else {
        strcat(str, "/index.html");
        t = ".html";
    }

     if (!HTTP_VERSION && strcmp(t, ".html"))  {
        responderror(sockfd, buffer);
        return;
    }
        
    //Open file
    FILE *fp = fopen(str, "r");

    // If error
    if (fp == NULL)
    {
        responderror(sockfd, buffer);
        return;
    }

    // Get correct media type for response header
    // Assumes the request-path hat no . after root other then the filename
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
    else if (!strcmp(t, ".json")){
        type = "application/json";
    }
    else {
        responderror(sockfd, buffer);
        return;
    }

    // use stat() system call to get length of file
    struct stat st;
    stat(str, &st);

    // Write header to socket (if HTTP_VERSION==0)
    // Workes with curl --http0.9
    if(HTTP_VERSION){
        sprintf(buffer, "HTTP/1.0 200 OK\r\nContent-Type: %s\r\nContent-Length: %lld\r\n\r\n", type, st.st_size);
        response(buffer, strlen(buffer), sockfd);
    }
    
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

    WorkerArgument *wa = (WorkerArgument*) ptr;
    // Cast ptr to pointer to a bounded ring buffer
    BNDBUF *bb = wa->bb;
    char *start = wa->start;

    // Declare buffer for the worker thread
    char buffer[MAXBUF];
    while(1){
        // get file descriptor from bounded ring buffer
        int fd = bb_get(bb);

        // Read request (from file descriptor)
        int n = read(fd,buffer,sizeof(buffer)-1);

        // If error
        if (n < 0) {
            continue;
        };

        printf("%s\n", buffer);        
       
        // Find start and end of path in request
        const char *start_of_path = strchr(buffer, '/');
        const char *end_of_path = strchr(start_of_path, ' ');
        /*
        char *end_of_path;
        for (int i = 0; i<buffer - start_of_path; i++){
            if (isspace(start_of_path[i])){
                end_of_path = &start_of_path[i];
            }
        }*/
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

        // Process request
        process_request(fd, path, t, buffer, start);

        // Close socket
        close (fd);

    }
}

//Assumes home path is an absolute path, not a relative path
int main(int argc, char *argv[]) {

    if (argc < 5){
        printf("%s", "Too few arguments\n");
        return -1;
    }
    char *start = argv[1];
    int port = atoi(argv[2]);
    int numb_threads = atoi(argv[3]);
    int bufferslots = atoi(argv[4]);

    // ignore SIGPIPE
    signal(SIGPIPE, SIG_IGN);

    // Declare array for threads
    pthread_t threads[numb_threads];
    int irets[numb_threads];

    // Initialize bounded ring buffer
    BNDBUF *bb = bb_init(bufferslots);

    WorkerArgument wa = {.bb = bb, .start = start};

    // Declare socket variables
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
    serv_addr.sin_port = htons(port);
    
    //bind here
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        error("ERROR on binding");
        }
    // Listen to port 
    listen(sockfd, MAXQUEUE);
    
    // Create threads
    for (int i = 0; i < numb_threads; i++) {
        irets[i] = pthread_create(&(threads[i]), NULL, work, (void*) &wa);
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