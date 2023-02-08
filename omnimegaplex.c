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

int socPOnField[22];

struct timespec start, now;

struct player{
    int pType;
    int id;
    int state;
    pthread_t thr;
};


void playerGo(void* args){
    struct player *p = args;
    p->state = READY;
    sleep(getRand(1, 30));
    while(1){
        if(avail == 1){
            getTheField();
        }
        switch(p->pType){
            case BASEBALL:
                printf("[Baseball %d] I want to play baseball!\n", p->id);
                numBball++;
                sem_wait(&bball);
                int posB = 19-playersLeft;
                printf("[Baseball %d] I'm going on the field in position [%d]\n", p->id, posB);
                playersLeft--;
                p->state = PLAYING;
                if(playersLeft > 0)
                    sem_post(&bball);
                sleep(bballPlaytime);
                sem_wait(&game);
                donePlaying++;
                sem_post(&game);
                printf("[Baseball %d] I finished playing!\n", p->id);
                if(donePlaying == 18){
                    avail = 1;
                    sem_post(&fieldAvail);
                    donePlaying = 0;
                }
                p->state = RESTING;
                sleep(5);
                break;
            case FOOTBALL:
                printf("[Football %d] I want to play football!\n", p->id);
                numFootball++;
                sem_wait(&football);
                int pos = 23-playersLeft;
                p->state = PLAYING;
                printf("[Football %d] I'm going on the field in position [%d]\n", p->id, pos);
                playersLeft--;
                if(playersLeft > 0)
                    sem_post(&football);
                sleep(footballPlaytime);
                sem_wait(&game);
                donePlaying++;
                sem_post(&game);
                printf("[Football %d] I finished playing!\n", p->id);
                if(donePlaying == 22){
                    avail = 1;
                    sem_post(&fieldAvail);
                    donePlaying = 0;
                }
                p->state = RESTING;
                sleep(5);
                break;
            case SOCCER:
                printf("[Soccer %d] I want to player soccer!\n", p->id);
                sem_wait(&game);
                numSoccer++;
                sem_post(&game);
                clock_gettime(CLOCK_REALTIME, &now);
                int tNow = now.tv_sec - start.tv_sec;
                if(tNow > 6 || onField >= 22){
                    sem_wait(&soccer);
                }
                else{
                    sem_wait(&game);
                    numSoccer--;
                    sem_post(&game);
                }
                int spot = -1;
                sem_wait(&game);
                for(int i = 0; i < 22; i++){
                    if(socPOnField[i] == 0){
                        spot = i + 1;
                        socPOnField[i] = 1;
                        break;
                    }
                }
                sem_post(&game);
                p->state = PLAYING;
                printf("[Soccer %d] I'm going on the field in position [%d]\n", p->id, spot);
                clock_gettime(CLOCK_REALTIME, &now);
                tNow = now.tv_sec - start.tv_sec;
                if(numSoccer > 1 && tNow < 7 && onField < 22){
                    sem_wait(&game);
                    numSoccer--;
                    onField++;
                    sem_post(&game);
                    sem_post(&soccer);

                }
                sleep(getRand(2, 5));
                sem_wait(&game);
                printf("[Soccer %d] I'm finished playing!\n", p->id);
                socPOnField[spot -1] = 0;
                onField--;
                sem_post(&game);
                if(onField == 0){
                    avail = 1;
                    sem_post(&fieldAvail);
                }
                p->state = RESTING;
                sleep(20);
                break;
        }
    }
}

int getTheField(){
    sem_wait(&fieldAvail);
    int gameType = getRand(1,3);
    for(int i = 0; i < 3; i++){
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
        gameType = ++gameType % 3;
    }
    //Not enough players
    printf("Checking field avail\n");
    sem_post(&fieldAvail);
    return 0;
}

int main(){
    int seed = getSeed("seed.txt");
    srand(seed);

    sem_init(&bball, 0, 0);
    sem_init(&football, 0, 0);
    sem_init(&soccer, 0, 0);
    sem_init(&fieldAvail, 0, 1);
    sem_init(&game, 0, 1);

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