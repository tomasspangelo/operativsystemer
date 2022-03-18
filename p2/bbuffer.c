#include "bbuffer.h"
#include "sem.h"


#define MAX_BUFFER_SIZE 5


typedef struct BNDBUF{
    SEM *m; //mutual exclusion on shared data (the buffer itself, to ensure correct pointers end/start)
    SEM *full; //buffer full (full -> val = number of full slots)
    SEM *empty; //buffer empty (empyte -> val = number of empty slots)
    int size; // size of buffer
    int *buf; //pointer to contents of buffer
    int end; // end of ring buffer
    int start; // start of ring buffer
} BNDBUF;

BNDBUF *bb_init(unsigned int size){
    // Check if size is ok (should have at least 1 element).
    if (size < 1 || size > MAX_BUFFER_SIZE)
        return NULL;
    
    // Allocate memory for BNDBUF (and check if OK)
    BNDBUF *bndbuf = malloc(sizeof(BNDBUF));
    if (bndbuf == NULL) {
        bb_del(bndbuf);
        return NULL;
    }

    // Init full sephamore
    bndbuf->full = sem_init(0);
    if (bndbuf->full == NULL){
        bb_del(bndbuf);
        return NULL;
    }

    // Init empty sephamore
    bndbuf->empty = sem_init(size);
    if (bndbuf->empty == NULL){
        bb_del(bndbuf);
        return NULL;
    }

    // Init binary sephamore
    bndbuf->m = sem_init(1);
    if (! bndbuf->m){
        bb_del(bndbuf);
        return NULL;
    }

    // Allocate memory for the data (buffer itself)
    bndbuf -> buf = (int*) malloc(size * sizeof(int));
    if (! bndbuf->buf){
        bb_del(bndbuf);
        return NULL;
    }
    bndbuf->size = size;
    bndbuf-> end = 0;
    bndbuf -> start = 0;
    return bndbuf;
}


void bb_del(BNDBUF *bb){
    if (bb->m != NULL){
        sem_del(bb -> m);
    }
    if (bb->empty != NULL){
        sem_del(bb->empty);
    }
    if (bb->full != NULL){
        sem_del(bb->full);
    }
    free(bb -> buf);
    free(bb);
}


int  bb_get(BNDBUF *bb){
    P(bb->full);
    P(bb->m);

    int item = (bb -> buf)[bb->start++];
    bb->start %= bb-> size;

    V(bb->m);
    V(bb->empty);
    return item;
}


void bb_add(BNDBUF *bb, int fd){
    P(bb -> empty);
    P(bb -> m);

    (bb -> buf)[bb->end++] = fd;
    bb->end %= bb -> size;

    V(bb -> m);
    V(bb -> full);
}