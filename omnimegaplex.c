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

int playersLeft;
int numBball = 0;
int numSoccer = 0;
int numFootball = 0;
int avail = 1;
int bballPlaytime;
int footballPlaytime;

int donePlaying = 0;
int onField = 0;



struct timespec start;

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
            printf("[Baseball %d] I want to play baseball!\n", p->id);
            numBball++;
            sem_wait(&bball);
            int pos = 19-playersLeft;
            printf("[Baseball %d] I'm going on the field in position [%d]", p->id, pos);
            playersLeft--;
            if(playersLeft > 0)
                sem_post(&bball);
            sleep(bballPlaytime);
            sem_wait(&game);
            donePlaying++;
            printf("[Baseball %d] I finished playing!");
            if(donePlaying == 18){
                avail = 1;
                sem_post(&fieldAvail);
                donePlaying = 0;
            }
            break;
        case FOOTBALL:
            printf("[Football %d] I want to play ootball!\n", p->id);
            numFootball++;
            sem_wait(&football);
            int pos = 23-playersLeft;
            printf("[Football %d] I'm going on the field in position [%d]", p->id, pos);
            playersLeft--;
            if(playersLeft > 0)
                sem_post(&football);
            sleep(footballPlaytime);
            sem_wait(&game);
            donePlaying++;
            printf("[Football %d] I finished playing!");
            if(donePlaying == 22){
                avail = 1;
                sem_post(&fieldAvail);
                donePlaying = 0;
            } 
            break;
        case SOCCER:
            break;
    }




}

int getTheField(){
    sem_wait(&fieldAvail);
    int gameType = getRand(1,3);
    if(numBball > 17 && gameType == BASEBALL){
        bballPlaytime-=18;
        playersLeft = 18;
        avail = 0;
        sem_post(&bball);
        return 1;
    }
    else if(footballPlaytime > 21 && gameType == FOOTBALL){
        footballPlaytime-=22;
        playersLeft = 22;
        avail = 0;
        sem_post(&football);
        return 1;
    }
    else if(numSoccer%2 == 0 && gameType == SOCCER){
        numSoccer-=2;
        avail = 0;
        onField=  2;
        clock_gettime(CLOCK_REALTIME, &start);
        sem_post(&soccer);
        sem_post(&soccer);
        return 1;
    }
    //Not enough players
    sem_post(&fieldAvail);
    return 0;
}

int main(){
    int seed = getSeed("seed.txt");
    srand(seed);

    sem_init(&bball, 0, 0);
    sem_init(&football, 0, 0);
    sem_init(&soccer, 0, 0);
    sem_init(&fieldAvail, 0, 0);
    sem_init(&game, 0, 0);

    bballPlaytime = getRand(3, 9);
    footballPlaytime = getRand(5, 8);

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
