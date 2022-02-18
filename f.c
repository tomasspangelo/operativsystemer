# include "a.h"

void playSong(int song){
    switch(song){
        case 1:
            #ifdef __APPLE__
                system("afplay ./bensound-jazzyfrenchy.mp3 -t 5");
             #elif __linux__
                 system("mpg123 ./bensound-jazzyfrenchy.mp3 -t 5");
            #endif
            break;
        
        case 2:
            #ifdef __APPLE__
                system("afplay ./bensound-acousticbreeze.mp3 -t 5");
             #elif __linux__
                 system("mpg123 ./bensound-acousticbreeze.mp3 -t 5");
            #endif
            break;

        case 3:
            #ifdef __APPLE__
                system("afplay ./bensound-memories.mp3 -t 5");
             #elif __linux__
                 system("mpg123 ./bensound-memories.mp3 -t 5");
            #endif
            break;

        case 4:
            #ifdef __APPLE__
                system("afplay ./bensound-littleidea.mp3 -t 5");
             #elif __linux__
                 system("mpg123 ./bensound-littleidea.mp3 -t 5");
            #endif
            break;

        default:
            printf("no such song");
    }

        //system("afplay and song=%s and -t 5");
    
}