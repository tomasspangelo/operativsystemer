#include "bbuffer.h"
#include "sem.c"


#define BUFFER_SIZE 5


typedef struct BNDBUF{
    pthread_mutex_t m;
    SEM_H P; //buffer full
    SEM_H V; //buffer empty
    int counter;
} BNDBUF;

BNDBUF *bb_init(unsigned int size){
    if (size < 0 || size > BUFFER_SIZE)
        return printf("Error. Buffer not made");

    return bb_init;
}


void bb_del(BNDBUF *bb){
    pthread_mutex_destroy(&bb->m);
    free(bb);
}


int  bb_get(BNDBUF *bb){
    if (bb->counter > 0) {
        BNDBUF remItem;
        remItem = bb[bb->counter-1];
        bb->counter--;
        return remItem;
    }else{
        return -1;
    }
}


void bb_add(BNDBUF *bb, int fd){
    if (bb->counter < BUFFER_SIZE) {
        bb->counter = fd;
        bb->counter++;
        return;
    } else
        printf("Error inserting item");
}