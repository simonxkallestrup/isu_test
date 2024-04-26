#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

pthread_cond_t entryCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t exitCond =  PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutexEntry = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexExit = PTHREAD_MUTEX_INITIALIZER;

bool carWait = false;
bool entryDoorOpen = false;
bool exitDoorOpen = false;
int parkTime; //sekunder

void* carThread(void* arg)
{
    cout << "\nBil korer hen til dor" << endl;
    pthread_mutex_lock(&mutexEntry);
    carWait = true;
    pthread_cond_signal(&entryCond);

    while(entryDoorOpen == false)
    {
        pthread_cond_wait(&entryCond, &mutexEntry);
    }

    cout << "Bil korer ind" << endl;
    carWait = false;
    pthread_cond_signal(&entryCond);
   
    while(entryDoorOpen==true){
        pthread_cond_wait(&entryCond, &mutexEntry);
    }

    cout << "Bil parkerer i " << parkTime << " sekunder" << endl;
    sleep(parkTime);
    
    pthread_mutex_unlock(&mutexEntry);

    cout << "\nBil korer mod exit" << endl;
    pthread_mutex_lock(&mutexExit);
    carWait = true;
    pthread_cond_signal(&exitCond); 

    while(exitDoorOpen == false)
    {
        pthread_cond_wait(&exitCond, &mutexExit); 
    }

    cout << "Bil korer ud" << endl;
    carWait = false;
    pthread_cond_signal(&exitCond);
    
    while(exitDoorOpen==true){
        pthread_cond_wait(&exitCond, &mutexExit);
    }

    cout << "Bil venter udenfor parkering i " << parkTime << " sekunder" << endl;
    sleep(parkTime); 

    pthread_mutex_unlock(&mutexExit);


    return NULL;
}

void* exitGuardThread(void *arg)
{
    pthread_mutex_lock(&mutexExit);

    while(carWait == false)
    {
        pthread_cond_wait(&exitCond, &mutexExit);
    }

    cout << "Exit dor åbnes" << endl;
    exitDoorOpen = true;
    pthread_cond_signal(&exitCond);

    while(carWait == true)
    {
        pthread_cond_wait(&exitCond, &mutexExit);
    }

    cout << "Exit dor lukkes" << endl;
    exitDoorOpen = false;
    pthread_cond_signal(&exitCond);
    pthread_mutex_unlock(&mutexExit);

    return NULL;
}

void* entryGuardThread(void *arg)
{
    pthread_mutex_lock(&mutexEntry);
    
    while(carWait == false)
    {
        pthread_cond_wait(&entryCond, &mutexEntry);
    }

    cout << "Entry Dor åbnes" << endl;
    entryDoorOpen = true;
    pthread_cond_signal(&entryCond);

    while(carWait == true)
    {
        pthread_cond_wait(&entryCond, &mutexEntry);
    }

    cout << "Entry Dor lukkes" << endl;
    entryDoorOpen = false;
    pthread_cond_signal(&entryCond);
    pthread_mutex_unlock(&mutexEntry);

    return NULL;
}



int main() {
    srand(time(NULL));

    pthread_t car, entryGuard, exitGuard;

    for(;;){
    parkTime = rand() % 10 + 1; //parkeringstid mellem 1 og 10 sekunder

    pthread_create(&car, NULL, carThread, NULL);
    pthread_create(&entryGuard, NULL, entryGuardThread, NULL);
    pthread_create(&exitGuard, NULL, exitGuardThread, NULL);

    
    pthread_join(car, NULL);
    pthread_join(entryGuard, NULL);
    pthread_join(exitGuard, NULL);
    }

    return 0;
}