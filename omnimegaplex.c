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

sem_t bball;
sem_t football;
sem_t soccer;
sem_t fieldAvail;
sem_t game;

int pNum = 0;
int avail = 1;
int baseballLength;
int footballLength;

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
    srand(seed);

    sem_init(&bball, 0, 0);
    sem_init(&football, 0, 0);
    sem_init(&soccer, 0, 0);
    sem_init(&fieldAvail, 0, 0);
    sem_init(&game, 0, 0);

    baseballLength = getRand(3, 9);

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

    while(1);
}

int getRand(int low, int high){
    int random = (rand() % (high - low + 1)) + low;
    return random;
}

int getSeed(const char* file_name){

    FILE* file = fopen (file_name, "r");
    int i = 0;

    fscanf (file, "%d", &i);    

    printf("Read seed value: %d\n\n", i);

    fclose (file);
    return i;
}
