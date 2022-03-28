#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define MAX_PATH 200

// The entered
// text is split into command name and arguments. Arguments are separated from each other and from the command
// name by space (ASCII 0x20) or tab (0x09) characters.

void printFilepath( char *path ){
     getcwd(path, MAX_PATH);
    printf("%s:\n", path);
}


int main(void) {
    char path[MAX_PATH];
    printFilepath(path);
    char *str;
    scanf("%[^\n]%*c", str);
    printf("%s", str);

    char *found;

    
    printf("Original string: '%s'\n",str);

    printf("Does it work?");
    //system(str);

    //char commanName[MAX_PATH];
    //char arguments[MAX_PATH];
    //int i = 0;
    while( (found = strsep(&str," ")) != NULL ){
        printf("Her kommer det");
        printf("%s\n",found);
        
    }
   
        

    return(0);
}