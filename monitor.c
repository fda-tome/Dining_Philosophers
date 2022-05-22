#include<stdlib.h>
#include<stdio.h>
#include <pthread.h>
#include <semaphore.h>


struct condition{
    int count;
    sem_t x_sem;
};
static int intent[5];
static sem_t next;
static int next_count;
static sem_t mutex;
static char state[5];
static struct condition self[5];

void initialization_code(){
    next_count = 0;
    sem_init(&next, 0, 0);
    sem_init(&mutex, 0, 0);
    for (int i = 0; i < 5; i++){
        intent[i] = i;
        state[i] = 'T';
        sem_init(&(self[i].x_sem), 0, 0);
        self[i].count = 0;
    }
    intent[1] = 2;
    intent[3] = 4;
}

static void wait(int i){
    (self[i].count)++;
    if(next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
    sem_wait(&(self[i].x_sem));
    (self[i].count)--;
}

static void signal(int i){
    if(self[i].count > 0){
        next_count++;
        sem_post(&(self[i].x_sem));
        sem_wait(&next);
        next_count--;
    }
}
static void test(int i){
    if ((state[(i + 4) % 5] != 'E') && (state[i] == 'H') && (state[(i + 1) % 5] != 'E') && (intent[i] == i) && (intent[(i + 4) % 5] == i)){
            state[i] = 'E';
            signal(i);
    }
}

void pickup(int i){
    state[i] = 'H';
    test(i);
    if(state[i] != 'E')
        wait(i);
}

void putdown(int i){
    intent[i] = (i + 1) % 5;
    intent[(i + 4) % 5] = (i + 4) % 5;
    state[i] = 'T';
    test((i + 4) % 5);
    test((i + 1) % 5);
}




