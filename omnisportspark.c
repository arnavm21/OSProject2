#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t mutex;
pthread_t baseball[36];
pthread_t football[44];
pthread_t soccer[44];

int main(){
    sem_init(&mutex, 0, 1);

    for(int i = 0; i < 36; i++){
        pthread_create(&(baseball[i]), NULL, playbaseball, NULL);
    }

    for(int i = 0; i < 44; i++){
        pthread_create(&(football[i]), NULL, playfootball, NULL);
    }

    for(int i = 0; i < 44; i++){
        pthread_create(&(soccer[i]), NULL, playsoccer, NULL);
    }
    //pthread_join(t1,NULL);
    //pthread_join(t2,NULL);
    sem_destroy(&mutex);
    return 0;
}

void* playbaseball(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nPlaying Baseball..\n");
  
    //critical section
    sleep(4);
      
    //signal
    printf("\nJust Exiting...\n");
    sem_post(&mutex);
}

void* playfootball(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nPlaying Football..\n");
  
    //critical section
    sleep(4);
      
    //signal
    printf("\nJust Exiting...\n");
    sem_post(&mutex);
}

void* playsoccer(void* arg)
{
    //wait
    sem_wait(&mutex);
    printf("\nPlaying Soccer..\n");
  
    //critical section
    sleep(4);
      
    //signal
    printf("\nJust Exiting...\n");
    sem_post(&mutex);
}
