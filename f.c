# include "a.h"

void playSong(int song){
    switch(song){
        case 1:
            #ifdef __APPLE__
                system("afplay music/bensound-jazzyfrenchy.mp3 -t 5");
             #elif __linux__
                 system("mpg123 music/bensound-jazzyfrenchy.mp3 -t 5");
            #endif
            break;
        
        case 2:
            #ifdef __APPLE__
                system("afplay music/bensound-acousticbreeze.mp3 -t 5");
             #elif __linux__
                 system("mpg123 music/bensound-acousticbreeze.mp3 -t 5");
            #endif
            break;

        case 3:
            #ifdef __APPLE__
                system("afplay music/bensound-memories.mp3 -t 5");
             #elif __linux__
                 system("mpg123 music/bensound-memories.mp3 -t 5");
            #endif
            break;

        case 4:
            #ifdef __APPLE__
                system("afplay music/bensound-littleidea.mp3 -t 5");
             #elif __linux__
                 system("mpg123 music/bensound-littleidea.mp3 -t 5");
            #endif
            break;

        default:
            printf("no such song");
    }

        //system("afplay and song=%s and -t 5");
    
}