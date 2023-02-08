#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <string.h>

#define BASEBALL 1
#define FOOTBALL 2
#define SOCCER 3

const READY = 0;
const PLAYING = 1;
const RESTING = 2;

struct player{
    int pType;
    int id;
    int state;
    pthread_t thr;
};


void playerGo(void* args){
    struct player *p = args;

    sleep(1);

    switch(p->pType){
        case BASEBALL:
            break;
        case FOOTBALL:
            break;
        case SOCCER:
            break;
    }




}

int main(){
    int seed = getSeed("seed.txt");

//struct line* array = malloc(number_of_elements * sizeof(struct line));
    struct player* bplayers = malloc(36 * sizeof(struct player));
    struct player* fplayers = malloc(44 * sizeof(struct player));
    struct player* splayers = malloc(44 * sizeof(struct player));


    for (int i = 0; i < 36; i++){
        bplayers[i].pType = BASEBALL;
        bplayers[i].id = i;
        bplayers[i].state = READY;
        //printf("Created player [%d]\n", bplayers[i].id);
        pthread_create(&bplayers[i].thr, NULL, playerGo, &bplayers[i]);
    }

    for (int i = 0; i < 44; i++){
        fplayers[i].pType = FOOTBALL;
        fplayers[i].id = i;
        fplayers[i].state = READY;
        pthread_create(&(fplayers[i].thr), NULL, playerGo, &fplayers[i]);
    }

    for (int i = 0; i < 36; i++){
        splayers[i].pType = SOCCER;
        splayers[i].id = i;
        splayers[i].state = READY;
        pthread_create(&(splayers[i].thr), NULL, playerGo, &splayers[i]);
    }
}