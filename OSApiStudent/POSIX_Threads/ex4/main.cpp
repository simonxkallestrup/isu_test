#include <iostream>
#include <thread>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include "Vector.hpp"

Vector sharedVector;

void* writer(void* data) {
    int id = *(int*)data;

    if (sharedVector.setAndTest(id) == false) {
        std::cerr << "Fejl: inkonsistens detekteret af writer " << id << std::endl;
    }
    sleep(1);
    std::cout << "Writer " << id << std::endl;
    
    return NULL;
}

int main() {
    int numThreads;
    std::cout << "Indtast antallet af traade: ";
    std::cin >> numThreads;
    pthread_t thread[numThreads];

    for (int i = 0; i < numThreads; i++) {
        pthread_create(&thread[i], NULL, writer, (void*)&i);
    }

    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(thread[i], NULL);
    }

    return 0;
}