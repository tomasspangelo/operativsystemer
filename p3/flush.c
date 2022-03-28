#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "flush.h"


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
       
    pid = fork();
    if (pid == 0) {
        execl(argv[0], argv[1]);
        printf("Den gjorde det\n");
        exit(0);
    } 
    return pid;
    
}


int main(void) {
    char path[MAX_PATH];
    printFilepath(path);
    char str[MAX_PATH];
    scanf("%[^\n]%*c", str);
    printf("%s", str);

    char *found;
    
    printf("Original string: '%s'\n",str);

    printf("Does it work?\n");
    //system(str);

    char *tok =str, *end = str;
    int i = 0;
    while( tok != NULL ){
        strsep(&end, " ");
        found = tok;
        strcpy(argv[i], tok);
        printf("Her kommer det\n");
        printf("%s\n",argv[i]);
        tok = end;
        i++;
    }
    create_process();

    system(str);

   
        

    return(0);
}