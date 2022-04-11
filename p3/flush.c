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
        for(int i = 0; i<size; ++i){

            if(!strcmp(argv[i],"<")){
                FILE *fp;
                char buff[255];

                fp = fopen(argv[i+1], "r");

                while (fscanf(fp, "%[^\n] ", buff) != EOF) {
                    printf(" %s\n", buff);
                }
                fclose(fp);  
            }  

            //Rederict standard output to the file
            else if(!strcmp(argv[i],">")){
               int fd;

                fd = open(argv[i+1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

                if (dup2(fd, 1) < 0) { perror("dup2"); exit(1); }

                 printf("Standard output now goes to \n");

                 size = size - 2;

                 close(fd);
            }    
        }

        if(size == 2 && strcmp(argv[0],"cd")){
            execl(argv[0],argv[0],argv[1], (char*) NULL);
        }
        else if(size == 1 && strcmp(argv[0],"cd")){
            execl(argv[0],argv[0], (char*) NULL);
        }
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