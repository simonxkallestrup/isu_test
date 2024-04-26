#include <iostream>
#include <vector>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include "Vector.hpp" 

Vector sharedVector;
sem_t semaphore; //Creating a semaphore variable named semaphore
//Declared outside of main fnc, else the writer cant access the variable

void* writer(void* data) {
    int id = *(int*)data;

    if (sharedVector.setAndTest(id) == false) {
        std::cerr << "Error: Inconsistency detected by writer " << id << std::endl;
    }
    sleep(1);
    std::cout << "Writer " << id << std::endl;

    sem_post(&semaphore); // Signal that this thread is done (increment)

    return NULL;
}

int main() {
    int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    if (sem_init(&semaphore, 0, 1) != 0) {
        std::cerr << "Semaphore initialization failed!" << std::endl;
        return 1;
    }

    pthread_t thread[numThreads];

    for (int i = 0; i < numThreads; i++) {
        sem_wait(&semaphore); // Wait for semaphore to be available (decrement)
        pthread_create(&thread[i], NULL, writer, (void*)&i);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(thread[i], NULL);
    }

    sem_destroy(&semaphore); // Cleanup semaphore

    return 0;
}