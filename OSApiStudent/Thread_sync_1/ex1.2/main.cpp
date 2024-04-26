#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

sem_t sem;

void* printHello(void* id) {
  int threadID = *(int*)id;

  for(int i = 0; i < 10; i++) {
    sem_wait(&sem); // critical section start
    std::cout << "Hello #" << i << " from thread " << threadID << std::endl;
    sem_post(&sem); // critical section end
    sleep(1);
  }

  return NULL;
}

int main() {
  pthread_t threadID0, threadID1;
  sem_init(&sem, 0, 1); // Initialize the semaphore

  int id0 = 0;
  int id1 = 1;

  pthread_create(&threadID0, NULL, printHello, &id0);
  pthread_create(&threadID1, NULL, printHello, &id1);

  pthread_join(threadID0, NULL);
  pthread_join(threadID1, NULL);

  sem_destroy(&sem); // Destroy the semaphore

  return 0;
}

//Mutex
//Mutex er primært brugt til at sikre gensidig udelukkelse for samtidig adgang til en 
//delt ressource. Kun én tråd kan låse en mutex ad gangen.
//En mutex har en ejer, hvilket betyder, at den tråd, der låste mutexen, 
//skal være den, der låser den op.
//Semaphore:
//En semafor kan bruges til at styre adgangen til en delt ressource, men den kan også 
//bruges til at synkronisere flere tråde, da den tillader mere end én tråd at låse 
//den ad gangen.
//En semafor har ingen ejer, hvilket betyder, at en tråd kan låse en semafor op, 
//som en anden tråd har låst.