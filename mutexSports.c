#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <pthread.h>

pthread_t baseBall[36];
pthread_t soccer[44];
pthread_t football[44];

pthread_mutex_t lock[22];
pthread_cond_t fieldSpots[22];

int fieldType = 0; //0 any, 1 Baseball, 2 soccer, 3 football

void* playBaseBall(void* args){
    int pid = getRand(1000,9999);
    printf("I'm a #%d baller\n", pid);
}

void* playSoccer(void* args){
    int pid = getRand(1000,9999);
    printf("I'm a #%d footer\n", pid);
}

void* playFootBall(void* args){
    int pid = getRand(1000,9999);
    printf("I'm chad #%d, yeeeeuhhh\n", pid);
}


int main(){

    for(int i = 0; i < 36; i++){
        int error = pthread_create(&(baseBall[i]), NULL, &playBaseBall, NULL);
        if(error != 0){
            printf("Error creating thread...\n");
        }
    }

    for(int i = 0; i < 44; i++){
        int error = pthread_create(&(soccer[i]), NULL, &playSoccer, NULL);
        if(error != 0){
            printf("Error creating thread...\n");
        }
    }

    for(int i = 0; i < 44; i++){
        int error = pthread_create(&(football[i]), NULL, &playFootBall, NULL);
        if(error != 0){
            printf("Error creating thread...\n");
        }
    }

    for(int i = 0; i < 22; i++){
        pthread_cond_init(&(fieldSpots[i]), NULL);
    }


    //Sport
    while(1){
        int newFT = getRand(1, 3);
        if(newFT == fieldType){
            continue;
        }

        fieldType = newFT;

        switch(fieldType){
            case 1:
                startBaseBall();
                break;
            case 2:
                startSoccer();
                break;
            case 3:
                startFootBall();
        }
    }

}

void startBaseBall(){
    
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