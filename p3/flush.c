#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


#define MAX_PATH 200

void printFilepath( char *path ){
     getcwd(path, MAX_PATH);
    printf("%s:\n", path);
}


int main(void) {
    char path[MAX_PATH];

    printFilepath(path);

    //return 0;
}