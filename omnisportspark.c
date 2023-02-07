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

sem_t fieldUse;//locked when field is in use
sem_t playGame;
pthread_cond_t startGame;
pthread_mutex_t gameLock;

int sport;

struct baseballP {
  int id;
  int state;
  pthread_t thr;
};
struct footballP {
  int id;
  int state;
  pthread_t thr;
};
struct soccerP {
  int id;
  int state;
  pthread_t thr;
};

struct player{
    int pType;
    int id;
    int state;
    pthread_t thr;
};


// void* playerGo(struct player p){
//     while(1){
//         //wait
//         sem_wait(&fieldUse);
//         printf("\nPlaying game..%d\n", p.id);
    
//         //critical section
//         sleep(4);
        
//         //signal
//         printf("\nJust Exiting...\n");
//         sem_post(&fieldUse);
//         printf("Going to sleep: P%d\n", p.id);
//         sleep(4);
//         printf("PUT ME IN COACH: P%d\n", p.id);
//     }
// }

void* playerGo(struct player p){
    while(1){
        if(sport == p.pType){
            printf("[%d] I am player\n", p.id);
            int taken = sem_trywait(&fieldUse);

            if(taken != 0){
                printf("[%d] Missed lock\n", p.id);
                sleep(5);
                continue;
            }
            printf("[%d] Took lock to play\n", p.id);
            pthread_cond_wait(&startGame, &gameLock);
            p.state = PLAYING;
            printf("Playing baseball!!");

            sleep(10);

            printf("[%d] Exiting field\n", p.id);
            
        }
            //Not my sport
            sleep(5);
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
        printf("Created player [%d]\n", bplayers[i].id);
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

    //sport = getRand(1,3);
    sport = 1;

    while (1){
        int gameTime; //seconds
        switch(sport){
            case BASEBALL:
                gameTime = getRand(3, 9);
                break;
            case FOOTBALL:
                gameTime = getRand(5, 12);
                break;
            case SOCCER:
                gameTime = getRand(10, 15);
                break;
            default:
                printf("Error, not a sport");
                exit(2);
        }

        if(sport == BASEBALL){
            pthread_mutex_lock(&gameLock);
            sem_init(&fieldUse, 0, 18);
            sem_init(&playGame, 0, 0);
            sem_wait(&fieldUse);
            pthread_cond_broadcast(&startGame);
            printf("Playing BASEBALL for %d seconds", gameTime);
            sleep(gameTime);
        }

    }


    sem_destroy(&fieldUse);
    return 0;
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