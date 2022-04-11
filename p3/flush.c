#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "flush.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>


#define MAX_PATH 200
int size;
char argv[MAX_PATH][MAX_PATH];
char path[MAX_PATH];

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
        char *clean_argv[size+1];
        int index = 0;
        for(int i = 0; i<size; i++){

            // looking for input character
            if(!strcmp(argv[i],"<")){
                ++i;
                int fd;
                fd = open(argv[i], O_RDONLY);
                dup2(fd, STDIN_FILENO); // duplicate stdin to input file
                close(fd);
                continue;  
            }  

            // looking for output character
            if(!strcmp(argv[i],">")){
                ++i;
                int fd;
                fd = creat(argv[i], 0644); // create a new file or rewrite an existing one
                dup2(fd, STDOUT_FILENO); // redirect stdout to file
                close(fd); 
                continue;
            } 
        clean_argv[index++] = argv[i];    
        }
        clean_argv[index] = NULL;
        execvp(clean_argv[0], clean_argv); 
        exit(0);
    } 

    //Gjør det i parrent så det er mulig å endre faktisk
     if(!strcmp(argv[0],"cd")){
            chdir(argv[1]);
            getcwd(path, MAX_PATH);
            //printFilepath(path);
    }
    //wait(0);
    //Parent wait
    w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
    //returns true if the child terminated normally,
    if (WIFEXITED(status)) {
        //antagligvis feil, dette er om det gikk bra med barn, men vi vil ha om selve kommand fungerte
        printf("Exit status[%s %s] = %d\n", argv[0], argv[1], WEXITSTATUS(status));
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
    //execl("/bin/ls","/bin/ls",  (char*) NULL);
    printFilepath(path);
    //chdir("..");
    //printFilepath(path);
    scanf("%[^\n]%*c", str);
    int flag=0;

    while(flag != EOF ){
    //printf("%s", str);


    char *found;
    
    //printf("Original string: '%s'\n",str);

    //printf("Does it work?\n");
    //system(str);

    char *tok =str, *end = str;
    size = 0;
    while( tok != NULL ){
        strsep(&end, " ");
        found = tok;
        strcpy(argv[size], tok);
        //printf("Her kommer det\n");
        //printf("%s\n",argv[i]);
        tok = end;
        size++;
    }
    create_process();

   
    printf("%s:\n", path);
    flag = scanf("%[^\n]%*c", str);

    //system(str);

    }
        

    return(0);
}