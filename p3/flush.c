#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "flush.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_BACKGROUND 10
char path[MAX_PATH];

// The entered
// text is split into command name and arguments. Arguments are separated from each other and from the command
// name by space (ASCII 0x20) or tab (0x09) characters.

void printFilepath( char *path ){
    getcwd(path, MAX_PATH);
    printf("%s:\n", path);
}

void add_process(struct Process *p, pid_t pid, int *index_stack, int *top, char argv[MAX_PATH][MAX_PATH], int size) {
    struct Process new_process;
    for(int i = 0; i < size; i++) {
        strcpy(new_process.argv[i], argv[i]);
    }
    new_process.size = size;
    new_process.active = 1;
    new_process.process_id = pid;
    p[index_stack[*top]] = new_process;
    (*top) --;
}

struct Process remove_process(struct Process *p, pid_t pid, int *index_stack, int *top) {
    int index = -1;
    //printf("%d\n", MAX_BACKGROUND - *top);
    for (int i = 0; i < MAX_BACKGROUND - *top; i++){
        if (p[i].process_id == pid) {
            index = i;
            break;
        }
    }
    //printf("%d\n", index);
    p[index].active = 0;
    (*top) ++;
    index_stack[*top] = index;
    return p[index];
}

void create_process(struct Process *p, int *index_stack, int *top, char *command, int size, char argv[MAX_PATH][MAX_PATH]) {
    pid_t pid;
    pid_t cpid, w;
    int status;

    //Gjør det i parrent så det er mulig å endre faktisk
     if(!strcmp(argv[0],"cd")){
            chdir(argv[1]);
            getcwd(path, MAX_PATH);
            //printFilepath(path);
            return;
    }

    //list processes running
    if(!strcmp(argv[0], "jobs")){
        for (int i = 0; i < MAX_BACKGROUND; i++){
            if (p[i].active == 1) {
                printf("WTF\n");
                printf("pid: %i CMD: ", p[i].process_id);
                for (int j = 0; j < p[i].size; j++) {
                    if (j == p[i].size - 1) {
                        printf("%s\n", p[i].argv[j]);
                        continue;
                    }
                    printf("%s ", p[i].argv[j]);
                }
            }
        }
        return;
    }
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
            // If it is not a background process, add argument
            if (strcmp(argv[i], "&")){
                clean_argv[index++] = argv[i]; 
            }
  
        }
        clean_argv[index] = NULL;
        int stat = execvp(clean_argv[0], clean_argv); 
        exit(stat);
    }

    // If it is a background process, add to stack
    if (!strcmp(argv[size - 1], "&")){
        add_process(p, pid, index_stack, top, argv, size);
        return;
    } 

    //wait(0);
    //Parent wait
    w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
    //returns true if the child terminated normally,
    if (WIFEXITED(status)) {
             printf("Exit status[");
        for (int i = 0; i < size; i++)
        {
            if(i == size-1){
                 printf("%s", argv[i]);
            }
            else{
                 printf("%s ", argv[i]);
            }
        }

        printf("] = %d\n",WEXITSTATUS(status));
        //returns true if the child process was terminated by a signal.
        } 
    else if (WIFSIGNALED(status)) {
        printf("killed by signal %d\n", WTERMSIG(status));
        //returns true if the child process was stopped by delivery of a signal
        } 
    else if (WIFSTOPPED(status)) {
        printf("stopped by signal %d\n", WSTOPSIG(status));
        }
  
    return;
    
}


int main(void) {
 
    struct Process processes[MAX_BACKGROUND];
    int index_stack[MAX_BACKGROUND];
    for (int i = 0; i < MAX_BACKGROUND; i++) {
        index_stack[i] = MAX_BACKGROUND - i - 1;
    }
    int top = MAX_BACKGROUND - 1;

    char str[MAX_PATH];
    //execl("/bin/ls","/bin/ls",  (char*) NULL);
    printFilepath(path);
    //chdir("..");
    //printFilepath(path);
    scanf("%[^\n]%*c", str);
    //char str[] = "jobs";
    int flag=0;

    while(flag != EOF ){
    //printf("%s", str);


    char *found;
    
    //printf("Original string: '%s'\n",str);

    //printf("Does it work?\n");
    //system(str);

    char *tok =str, *end = str;
    int size = 0;
    char argv[MAX_PATH][MAX_PATH];
    while( tok != NULL ){
        strsep(&end, " ");
        found = tok;
        strcpy(argv[size], tok);
        //printf("Her kommer det\n");
        //printf("%s\n",argv[i]);
        tok = end;
        size++;
    }
    create_process(processes, index_stack, &top, str, size, argv);

    // Clean up processes
    pid_t pid;
    int status;
    while ((pid = waitpid(-1, &status, WNOHANG)) > 0) {
        struct Process p = remove_process(processes, pid, index_stack, &top);
        if (WIFEXITED(status)) {
             printf("Exit status[");
        for (int i = 0; i < p.size; i++)
        {
            if(i == p.size-1){
                 printf("%s", p.argv[i]);
            }
            else{
                 printf("%s ", p.argv[i]);
            }
        }

        printf("] = %d\n",WEXITSTATUS(status));
        //returns true if the child process was terminated by a signal.
        } 
        else if (WIFSIGNALED(status)) {
            printf("killed by signal %d\n", WTERMSIG(status));
            //returns true if the child process was stopped by delivery of a signal
            } 
        else if (WIFSTOPPED(status)) {
            printf("stopped by signal %d\n", WSTOPSIG(status));
            }
  

    }

    //TODO: Clean up processes
    /*
    for (int i = 0; i < MAX_BACKGROUND; i++) {
        if (!processes[i].active){
            continue;
        }
        //wait(0);
        //Parent wait
        int status;
        int w = waitpid(processes[i].process_id, &status, WNOHANG);
        printf("%d\n", w);
        // If not finished -> continue
        if (w != 0) {
            printf("WHAT!\n");
            continue;
        }
        printf("KILL!\n");
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
        remove_process(processes, i, index_stack, &top);
        
    }
    */

   
    printf("%s:\n", path);
    flag = scanf("%[^\n]%*c", str);
    //char str[] = "ls &";

    //system(str);

    }
        

    return(0);
}