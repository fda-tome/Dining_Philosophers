#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include "monitor.h"

struct monitor DiningPhilosophers = {.init_code = &initialization_code, .pick = &pickup, .put = &putdown};

void *routine(void* i){
    int philo = *(int*)i;
    while(1){
        printf("Filósofo %d está pensando\n", philo);
        sleep(rand() % 5);
        DiningPhilosophers.pick(philo);
        printf("Filósofo %d está comendo\n", philo);
        sleep(rand() % 5);
        DiningPhilosophers.put(philo);
    }
    
}
   
void main(){
    DiningPhilosophers.init_code();
    pthread_t threads[5];
    pthread_attr_t attr;
    int philo[5];
    pthread_attr_init(&attr);

    for(int i = 0; i < 5; i++){
        philo[i] = i;
        pthread_create(&(threads[i]), NULL, routine, (int*)&philo[i]);
    }
    for(int i = 0; i < 5; i++)
        pthread_join(threads[i], NULL);
}
