#include "sem.h"

SEM *sem_init(int initVal){
   SEM* sem = malloc(sizeof(struct SEM));
   // The malloc() function allocates size bytes and returns a pointer
    //to the allocated memory.  The memory is not initialized. 
    sem->val = initVal;
    if(!sem){
        return NULL;
    }

    //Free /destroy on error //vet ikke om fungerer slik?
    errno = pthread_mutex_init(&sem->m, NULL);
    if(errno){
        return NULL;
    }
    errno = pthread_cond_init(&sem->c, NULL);
    if(errno){
        pthread_mutex_destroy(&sem->m);
         return NULL;
    }
    printf("Dette funka jo!\n");
    return sem;
}

int sem_del(SEM *sem){
    //destroying

    //returns 0 errorNumber if not
    int check;
    check= pthread_mutex_destroy(&sem->m);
    if(check != 0){
        return -1;
    }

    
    check = pthread_cond_destroy(&sem->c);
    if(check != 0){
        return -1;
    }
    //deallocates the memory previously allocated by a call to calloc, malloc, or realloc.
    //does not return any error
    free(sem);

    return 1;

    //TODO, return correct

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
    pthread_mutex_unlock(&sem->m);
}

void V(SEM *sem){
    pthread_mutex_lock(&sem->m);

    //add 1
    sem->val +=1;

    if(sem->val >0){
        pthread_cond_signal(&sem->c);
    }
    pthread_mutex_unlock(&sem->m);
}