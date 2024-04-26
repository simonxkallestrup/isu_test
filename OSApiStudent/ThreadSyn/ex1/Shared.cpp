#include <iostream>
#include <pthread.h>
#include <unistd.h>

void* incrementer(void* data) 
{
    //giving the right returnvalue
    unsigned int *shared = (unsigned int*)data;
    while (*shared <= 10)
    {
        (*shared)++;
        sleep(1);
    }
    return NULL;
}

void* reader(void* data)
{   
    //giving the right returnvalue
    unsigned int *shared = (unsigned int*)data;
    while(*shared <= 10)
    {
        std::cout << "Count: " << (*shared) << std::endl;
        sleep(1);
    }
    return NULL;
}

int main()
{
    unsigned int shared = 0;

    //creating the threads
    std::cout << "Creating threads.\n";
    pthread_t threadID0, threadID1;
    int threadnr1 = pthread_create(&threadID0, NULL, incrementer, (void*)&shared);
    int threadnr2 = pthread_create(&threadID1, NULL, reader, (void*)&shared);
    std::cout << "Waiting for threads to finish.\n";

    // making sure the threads will work till they are done
    pthread_join(threadID0, NULL);
    pthread_join(threadID1, NULL);
    std::cout << "Threads finished.\n";

    //terminating?
    return 0;

}