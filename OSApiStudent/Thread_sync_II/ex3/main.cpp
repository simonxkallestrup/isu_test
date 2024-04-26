#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

using namespace std;

int carWaitingEntry = 0;
int carWaitingExit = 0;
int carCount = 0; //Tæller biler på parkeringspladsen
int maxCarsParked = 3; //Maksimalt antal biler på parkeringspladsen
int antalBiler = 20;
int parkTime;
int maxParkTime = 6; // Maksimal parkeringstid i sekunder

bool entryDoorOpen = false;
bool exitDoorOpen = false;
bool entrySensor = false;
bool exitSensor = false;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t entryCond = PTHREAD_COND_INITIALIZER; //Siger til entryguard at der er en bil der venter
pthread_cond_t exitCond = PTHREAD_COND_INITIALIZER; //Siger til exitguard at der er en bil der venter
pthread_cond_t carLeft = PTHREAD_COND_INITIALIZER; // Siger til entryguard at en bil er kørt ud
pthread_cond_t sensorEntry = PTHREAD_COND_INITIALIZER; //Siger til entryguard at en bil er kørt ind
pthread_cond_t sensorExit = PTHREAD_COND_INITIALIZER; //Siger til exitguard at en bil er kørt ud
pthread_cond_t driveCond = PTHREAD_COND_INITIALIZER; //Siger til en bil at den kan køre ind/ud

void* carThread(void *arg)
{
    int carID = *(int*)arg;

    while(1)
    {
        pthread_mutex_lock(&mutex);
        ++carWaitingEntry;
        cout << "Bil" << carID << " korer hen til entrydoor" << endl;
        pthread_cond_signal(&entryCond);

        while(entryDoorOpen==false) //Venter på at entryguard åbner døren
        {
            pthread_cond_wait(&driveCond, &mutex);
        }

        cout << "Bil" << carID << " korer ind" << endl; 
        entrySensor = true;
        pthread_cond_signal(&sensorEntry); //Siger til entryguard at bilen er kørt ind

        parkTime = rand() % maxParkTime + 1;
        cout << "Bil" << carID <<  " parkerer i " << parkTime << " sekunder" << endl;
        pthread_mutex_unlock(&mutex);

        sleep(parkTime); //Parkerer i noget tid

        pthread_mutex_lock(&mutex);
        ++carWaitingExit;
        cout << "Bil"<< carID << " korer mod exit" << endl;
        pthread_cond_signal(&exitCond); //Siger til exitguard at der er en bil der venter

        while(exitDoorOpen==false) //Venter på at exitguard åbner døren
        {
            pthread_cond_wait(&driveCond, &mutex);
        }
        cout << "Bil" << carID << " korer ud" << endl;
        exitSensor = true;
        pthread_cond_signal(&sensorExit); //Siger til exitguard at bilen er kørt ud

        parkTime = rand() % maxParkTime + 1;
        cout << "Bil" << carID << " venter udenfor parkering i " << parkTime << " sekunder" << endl;
        pthread_mutex_unlock(&mutex);        
        sleep(parkTime); //Venter udenfor parkering
    }
    
    return 0;
}


void* entryGuardThread(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(carWaitingEntry == 0) //Vent på at der er en bil der venter
        {
            pthread_cond_wait(&entryCond,&mutex);
        }

        if(carCount >= maxCarsParked)
        {
            cout << "Parkeringsplads fyldt. Bil venter..." << endl;
        }
        
        while(carCount >= maxCarsParked) //Hvis der er for mange biler på parkeringspladsen, så vent til exitGuard siger til
        {
            pthread_cond_wait(&carLeft,&mutex); //vent på mutex til en bil er kørt ud
        }

        cout << "Entry Door åbnes..." << endl;
        entryDoorOpen = true;
        pthread_cond_signal(&driveCond); //Bare kør, bil

        while(entrySensor == false) //Vent på at en bil er kørt ind
        {
            pthread_cond_wait(&sensorEntry, &mutex);
        }

        --carWaitingEntry; //Der er nu en bil mindre der venter
        ++carCount; //Der er nu en bil mere på parkeringspladsen
        cout << "Antal biler på parkeringspladsen: " << carCount << endl;

        cout << "Entry Door lukkes..." << endl;
        entryDoorOpen = false; //Entry door er nu lukket
        entrySensor = false; //Ingen bil passerer sensoren

        pthread_mutex_unlock(&mutex); 
    }
    return 0;
}

void* exitGuardThread(void *arg)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);

        while(carWaitingExit == false) //Vent på at der er en bil ved exit
        {
            pthread_cond_wait(&exitCond,&mutex);
        }

        cout << "Exit door åbnes..." << endl;
        exitDoorOpen = true;
        pthread_cond_signal(&driveCond); //Bare kør, bil

        while(exitSensor == false) //Vent på at en bil er kørt ud
        {
            pthread_cond_wait(&sensorExit, &mutex);
        }

        --carWaitingExit;
        --carCount;
        if(carCount<maxCarsParked) 
        {
            pthread_cond_signal(&carLeft);
        }

        cout << "Antal biler på parkeringspladsen: " << carCount << endl;

        cout << "Exit door lukkes..." << endl;
        exitDoorOpen = false;
        exitSensor = false;

        pthread_mutex_unlock(&mutex);
    }
    return 0;
}


int main(void)
{
    srand(time(NULL));

    pthread_t car[antalBiler+1];
    pthread_t entryThread;
    pthread_t exitThread;

    pthread_create(&entryThread, NULL, entryGuardThread, NULL);
    pthread_create(&exitThread, NULL, exitGuardThread , NULL);

    for(int i = 1; i <= antalBiler; i++) 
    {
        pthread_create(&car[i], NULL, carThread, new int{i});
    }

    pthread_join(entryThread, NULL);
    pthread_join(exitThread, NULL);

    return 0;
}