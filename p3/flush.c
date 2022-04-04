#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "flush.h"
#include <sys/types.h>
#include <sys/wait.h>


#define MAX_PATH 200
char argv[MAX_PATH][MAX_PATH];

// The entered
// text is split into command name and arguments. Arguments are separated from each other and from the command
// name by space (ASCII 0x20) or tab (0x09) characters.

void printFilepath( char *path ){
    getcwd(path, MAX_PATH);
    printf("%s:\n", path);
}

pid_t create_process() {
    pid_t pid;
    pid_t cpid, w;
    int status;
    pid = fork();


    if (pid == 0) {
        execl(argv[0],argv[0], argv[1], (char*) NULL);
        exit(0);
    } 
    //Parent wait
    w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
    //returns true if the child terminated normally,
    if (WIFEXITED(status)) {
        printf("Exit status[%s%s] = %d\n", argv[0], argv[1], WEXITSTATUS(status));
    //returns true if the child process was terminated by a signal.
    } else if (WIFSIGNALED(status)) {
        printf("killed by signal %d\n", WTERMSIG(status));
    //returns true if the child process was stopped by delivery of a signal
    } else if (WIFSTOPPED(status)) {
        printf("stopped by signal %d\n", WSTOPSIG(status));}
    return pid;
    
}


int main(void) {
    char str[MAX_PATH];
    char path[MAX_PATH];
    printFilepath(path);
    scanf("%[^\n]%*c", str);
    int flag=0;

    while(flag != EOF ){
    //printf("%s", str);


    char *found;
    
    //printf("Original string: '%s'\n",str);

    //printf("Does it work?\n");
    //system(str);

    char *tok =str, *end = str;
    int i = 0;
    while( tok != NULL ){
        strsep(&end, " ");
        found = tok;
        strcpy(argv[i], tok);
        //printf("Her kommer det\n");
        //printf("%s\n",argv[i]);
        tok = end;
        i++;
    }
    create_process();

    char path[MAX_PATH];
    printFilepath(path);
    flag = scanf("%[^\n]%*c", str);

    //system(str);

    }
        

    return(0);
}