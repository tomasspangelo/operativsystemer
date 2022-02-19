#include "sound.h"
void play_song(int song){
    switch(song){
        case 1:
            #ifdef __APPLE__
                system("afplay music/bensound-jazzyfrenchy.mp3 -t 5");
             #elif __linux__
                system("mpg123 -n 192 music/bensound-jazzyfrenchy.mp3");
            #endif
            break;
        
        case 2:
            #ifdef __APPLE__
                system("afplay music/bensound-acousticbreeze.mp3 -t 5");
             #elif __linux__
                system("mpg123 -n 192 music/bensound-acousticbreeze.mp3");
            #endif
            break;

        case 3:
            #ifdef __APPLE__
                system("afplay music/bensound-memories.mp3 -t 5");
             #elif __linux__
                system("mpg123 -n 192 music/bensound-memories.mp3");
            #endif
            break;

        case 4:
            #ifdef __APPLE__
                system("afplay music/bensound-littleidea.mp3 -t 5");
             #elif __linux__
                system("mpg123 -n 192 music/bensound-littleidea.mp3");
            #endif
            break;

        default:
            printf("No such song\n");
    }
    
}

void play_sounds(){
    int i;
    for(i = 1 ; i <= 4; i++){
        play_song(i);
    }
}

int choose_sound(){
    printf("Which sound do you want as the alarm (Choose between 1-4)\n");
    int sound;
    while(true){
        scanf("%d", &sound);
        if(sound>0 && sound<=4){
            break;
        }
        printf("Not an option, please choose a number between 1 and 4.\n");
    }
    return sound;
    
}