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

/*
 * Inicialização das variaveis necessarias para a utilizacao do monitor
*/
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

/*
 * Implementacao do wait de variaveis condicionais com exclusao mutua utilizando semaforos para seguir o padrao dos monitores
 * @param i define o index da condicional que ira esperar
*/
static void wait(int i){
    (self[i].count)++;
    if(next_count > 0)
        sem_post(&next);
    else
        sem_post(&mutex);
    sem_wait(&(self[i].x_sem));
    (self[i].count)--;
}

/*
 * Signal carateristico de variaveis condicionais de monitores utilizando semaforos
 * @param i define o index da condicional que devera dar signal
*/
static void signal(int i){
    if(self[i].count > 0){
        next_count++;
        sem_post(&(self[i].x_sem));
        sem_wait(&next);
        next_count--;
    }
}

/* Testa a disponibilidade das condicionais para serem utilizadas
 * @param i define o index do filosofo a ser testado
*/
static void test(int i){
    if ((state[(i + 4) % 5] != 'E') && (state[i] == 'H') && (state[(i + 1) % 5] != 'E') && (intent[i] == i) && (intent[(i + 4) % 5] == i)){
            state[i] = 'E';
            signal(i);
    }
}

/* Funçao para pegar os palitos para comer
 * @param i define o index do filosofo que ira pegar os palitos
*/
void pickup(int i){
    state[i] = 'H';
    test(i);
    if(state[i] != 'E')
        wait(i);
}

/*Funçao para devolver os palitos e voltar a pensar
 * @param i define o index do filosofo que devolvera os palitos
*/
void putdown(int i){
    intent[i] = (i + 1) % 5;
    intent[(i + 4) % 5] = (i + 4) % 5;
    state[i] = 'T';
    test((i + 4) % 5);
    test((i + 1) % 5);
}




