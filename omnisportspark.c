#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t fieldUse; // locked when field is in use
sem_t fieldSpots[22];
pthread_t baseball[36];
pthread_t football[44];
pthread_t soccer[44];

void *playbaseball(void *arg)
{
    while (1)
    {
        // wait
        sem_wait(&fieldUse);
        printf("\nPlaying Baseball..%d\n", getpid());

        // critical section
        sleep(4);

        // signal
        printf("\nJust Exiting...\n");
        sem_post(&fieldUse);
        sleep(4);
    }
}

void *playfootball(void *arg)
{
    while (1)
    {
        // wait
        sem_wait(&fieldUse);
        printf("\nPlaying Football..%d\n", getpid());

        // critical section
        sleep(4);

        // signal
        printf("\nJust Exiting...\n");
        sem_post(&fieldUse);
        sleep(4);
    }
}

void *playsoccer(void *arg)
{
    while (1)
    {
        // wait
        sem_wait(&fieldUse);
        printf("\nPlaying Soccer..%d\n", getpid());

        // critical section
        sleep(4);

        // signal
        printf("\nJust Exiting...\n");
        sem_post(&fieldUse);
        sleep(4);
    }
}

int main()
{
    sem_init(&fieldUse, 0, 1);

    for (int i = 0; i < 36; i++)
    {
        pthread_create(&(baseball[i]), NULL, playbaseball, NULL);
    }

    for (int i = 0; i < 44; i++)
    {
        pthread_create(&(football[i]), NULL, playfootball, NULL);
    }

    for (int i = 0; i < 44; i++)
    {
        pthread_create(&(soccer[i]), NULL, playsoccer, NULL);
    }
    // pthread_join(t1,NULL);
    // pthread_join(t2,NULL);
    for (int i = 0; i < 36; i++)
    {
        pthread_join(&(baseball[i]), NULL);
    }

    for (int i = 0; i < 44; i++)
    {
        pthread_join(&(football[i]), NULL);
    }

    for (int i = 0; i < 44; i++)
    {
        pthread_join(&(soccer[i]), NULL);
    }

    while (1)
        ;
    sem_destroy(&fieldUse);
    return 0;
}
