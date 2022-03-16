#include "sem.h"

SEM *sem_init(int initVal){
   SEM *sem = malloc(sizeof(SEM));
   // The malloc() function allocates size bytes and returns a pointer
    //to the allocated memory.  The memory is not initialized. 
    sem->val = initVal;
    if(!sem){
        return NULL;
    }

    //Free /destroy on error //vet ikke om fungerer slik?
    check = pthread_mutex_init(&sem->m, NULL);
    if(!check){
        free(buffer);
    }
    check = pthread_cond_init(&sem->c, NULL);
    if(!check){
        pthread_mutex_destroy(&sem->m);
    }

    return sem;
}

int sem_del(SEM *sem){
    //destroying
    pthread_mutex_destroy(&sem->m);
    pthread_cond_destroy(&sem->c);
    //deallocates the memory previously allocated by a call to calloc, malloc, or realloc.
    free(sem);

}

void P(SEM *sem){
    //The mutex object referenced by mutex shall be locked by calling pthread_mutex_lock(). 
    //If the mutex is already locked, the calling thread shall block until the mutex becomes available.
    pthread_mutex_lock(&sem->m);

    //wait for positive value
    while (sem->val <1){
        //atomically release mutex and cause the calling thread to block on the condition variable
        pthread_cond_wait(&sem->c, &sem->m);
    }

    //decrement
    sem->val -=1;
    //wake waiting thread
    if(sem->val >0){
        //function shall unblock at least one of the threads that are blocked on the specified 
        //condition variable cond (if any threads are blocked on cond).
        pthread_cond_signal(&sem->c);
    }
    pthread_mutex_unlock(&sem->m),
}

void V(SEM *sem){
    pthread_mutex_lock(&sem->m);

    //add 1
    sem->val +=1:

    if(sem->val >0){
        pthread_cond_signal(&sem->c);
    }
    pthread_mutex_unlock(&sem->m);
}