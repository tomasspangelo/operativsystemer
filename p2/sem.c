#include <stdio.h>
#include <stdatomic.h>
#include <pthread.h>
#include "sem.h"

struct SEM
{
    pthread_mutex_t lock;
    pthread_cond_t condition;
    int resource;
};

SEM* sem_init(int initVal)
{
    SEM *ptr = (SEM *) malloc(sizeof(struct SEM));
    int mutex_result = pthread_mutex_init(&ptr->lock, NULL);
    int condition_result = pthread_cond_init(&ptr->condition, NULL);
    if(mutex_result != 0 || condition_result != 0)
    {
        sem_del(ptr);
        return NULL;
    }
    ptr->resource = initVal;
    return ptr;
}

int sem_del(SEM *sem)
{
    if (sem == NULL)
       return 0; 
    pthread_mutex_destroy(&sem->lock);
    pthread_cond_destroy(&sem->condition);
    free(sem);
    return 1;
}

void P(SEM *sem)
{ 
    pthread_mutex_lock(&sem->lock);
    while (sem->resource == 0)
    {
        pthread_cond_wait(&sem->condition, &sem->lock);
    }
    sem->resource--;
    pthread_mutex_unlock(&sem->lock);
}

void sem_wait(SEM *sem)
{
    P(sem);
}

void V(SEM *sem)
{
    pthread_mutex_lock(&sem->lock);
    sem->resource++;
    pthread_mutex_unlock(&sem->lock);
    pthread_cond_signal(&sem->condition);
}

void sem_signal(SEM *sem)
{
    V(sem);
}
