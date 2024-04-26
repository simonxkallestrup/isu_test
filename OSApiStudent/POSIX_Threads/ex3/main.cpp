#include <iostream>
#include <pthread.h>
#include <unistd.h>

unsigned int shared = 0;

void* incrementer(void* data) 
{
    shared = *(unsigned int*)data;
    
    while (shared <= 10)
    {
        (shared)++;
        sleep(1);
    }
    return NULL;
}

void* reader(void* data)
{
    shared = *(unsigned int*)data;
    
    while(shared <= 10)
    {
        std::cout << "Count: " << shared << std::endl;
        sleep(1);
    }
    return NULL;
}

int main()
{
    std::cout << "Creating threads.\n";
    pthread_t threadID0, threadID1;
    pthread_create(&threadID0, NULL, incrementer, (void*)&shared);
    pthread_create(&threadID1, NULL, reader, (void*)&shared);
    std::cout << "Waiting for threads to finish.\n";

    pthread_join(threadID0, NULL);
    pthread_join(threadID1, NULL);
    std::cout << "Threads finished.\n";

    return 0;
}