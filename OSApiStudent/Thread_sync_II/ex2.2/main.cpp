#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

pthread_cond_t entryCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t exitCond =  PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexEntry = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexExit = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t coutMutex = PTHREAD_MUTEX_INITIALIZER; 

int carWaitEntry = -1;
int carWaitExit = -1;
bool entryDoorOpen = false;
bool exitDoorOpen = false;
int parkTime; //sekunder


void* carThread(void* arg)
{
    while(1)
    {
        int carID = *((int*)arg);
        
        pthread_mutex_lock(&coutMutex); cout << "\nBil" << carID << " korer hen til entrydoor" << endl; pthread_mutex_unlock(&coutMutex);

        pthread_mutex_lock(&mutexEntry);
        carWaitEntry = carID;
        pthread_cond_signal(&entryCond);

        while(entryDoorOpen == false)
        {
            pthread_cond_wait(&entryCond, &mutexEntry);
        }

        pthread_mutex_lock(&coutMutex); cout << "Bil" << carID << " korer ind" << endl;  pthread_mutex_unlock(&coutMutex);

        carWaitEntry = -1;
        pthread_cond_signal(&entryCond);
    
        while(entryDoorOpen==true){
            pthread_cond_wait(&entryCond, &mutexEntry);
        }

        parkTime = rand() % 3 + 4; //parkeringstid mellem 4 og 6 sekunder

        pthread_mutex_lock(&coutMutex); cout << "Bil" << carID <<  " parkerer i " << parkTime << " sekunder" << endl; pthread_mutex_unlock(&coutMutex);
        sleep(parkTime);
        
        pthread_mutex_unlock(&mutexEntry);

        pthread_mutex_lock(&coutMutex); cout << "\nBil"<< carID << " korer mod exit" << endl; pthread_mutex_unlock(&coutMutex);
        pthread_mutex_lock(&mutexExit);
        carWaitExit = carID; // true
        pthread_cond_broadcast(&exitCond); 

        while(exitDoorOpen == false)
        {
            pthread_cond_wait(&exitCond, &mutexExit); 
        }

        pthread_mutex_lock(&coutMutex); cout << "Bil" << carID << " korer ud" << endl; pthread_mutex_unlock(&coutMutex);
        carWaitExit = -1; // false
        pthread_cond_signal(&exitCond);
        
        while(exitDoorOpen==true){
            pthread_cond_wait(&exitCond, &mutexExit);
        }

        parkTime = rand() % 6 + 4; //parkeringstid mellem 4 og 6 sekunder
        pthread_mutex_lock(&coutMutex); cout << "Bil" << carID << " venter udenfor parkering i " << parkTime << " sekunder" << endl; pthread_mutex_unlock(&coutMutex);
        sleep(parkTime); 

        pthread_mutex_unlock(&mutexExit);
    }


    return NULL;
}

void* exitGuardThread(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutexExit);

        while(carWaitExit == -1)
        {
            pthread_cond_wait(&exitCond, &mutexExit);
        }

        pthread_mutex_lock(&coutMutex); cout << "Exit door åbnes for Bil" << carWaitExit << endl; pthread_mutex_unlock(&coutMutex);
        exitDoorOpen = true;
        pthread_cond_broadcast(&exitCond);

        while(carWaitExit != -1)
        {
            pthread_cond_wait(&exitCond, &mutexExit);
        }

        pthread_mutex_lock(&coutMutex); cout << "Exit door lukkes" << endl; pthread_mutex_unlock(&coutMutex);
        exitDoorOpen = false;
        pthread_cond_broadcast(&exitCond);
        pthread_mutex_unlock(&mutexExit);
    }

    return NULL;
}

void* entryGuardThread(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutexEntry);
        
        while(carWaitEntry == -1)
        {
            pthread_cond_wait(&entryCond, &mutexEntry);
        }
        
        pthread_mutex_lock(&coutMutex); cout << "Entry Door åbnes for Bil" << carWaitEntry << endl; pthread_mutex_unlock(&coutMutex);
        entryDoorOpen = true;
        pthread_cond_broadcast(&entryCond);

        while(carWaitEntry != -1)
        {
            pthread_cond_wait(&entryCond, &mutexEntry);
        }

        pthread_mutex_lock(&coutMutex); cout << "Entry Door lukkes" << endl; pthread_mutex_unlock(&coutMutex);
        entryDoorOpen = false;
        pthread_cond_broadcast(&entryCond);
        pthread_mutex_unlock(&mutexEntry);
    }

    return NULL;
}



int main() {
    srand(time(NULL));

    pthread_t car1, car2, car3, entryGuard, exitGuard;
    int carID1 = 1;
    int carID2 = 2;
    int carID3 = 3;

    pthread_create(&entryGuard, NULL, entryGuardThread, NULL);
    pthread_create(&exitGuard, NULL, exitGuardThread, NULL);  

    while(1)
    {
    pthread_create(&car1, NULL, carThread, &carID1);
    pthread_create(&car2, NULL, carThread, &carID2);
    pthread_create(&car3, NULL, carThread, &carID3);

    pthread_join(car1, NULL);
    }

    pthread_join(entryGuard, NULL);
    pthread_join(exitGuard, NULL);

    return 0;
}