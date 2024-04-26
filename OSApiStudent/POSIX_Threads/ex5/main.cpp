#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"

Vector sharedVector;

struct structData{
  int id;
  int time;
};

void* writer(void* structData) {
    struct structData recievedStruct = *(struct structData *)structData;

    if (sharedVector.setAndTest(recievedStruct.id) == false) {
        std::cerr << "Fejl: inkonsistens detekteret af writer " << recievedStruct.id << std::endl;
    }
    usleep(recievedStruct.time);
    std::cout << "Writer " << recievedStruct.id << std::endl;
    
    return NULL;
}

int main() {
    struct structData myStruct;
    int numThreads;
    std::cout << "Indtast antallet af traade: ";
    std::cin >> numThreads ;
    std::cout << "Indtast sleep time i microsekunder: ";
    std::cin >> myStruct.time;
    pthread_t thread[numThreads];

    for (int i = 0; i < numThreads; i++) {
        myStruct.id = i;
        pthread_create(&thread[i], NULL, writer, (void*)&myStruct);  
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    return 0;
}