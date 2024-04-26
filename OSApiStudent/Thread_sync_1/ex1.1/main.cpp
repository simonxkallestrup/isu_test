#include <iostream>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock;

void* printHello(void* id) {
  int threadID = *(int*)id;

  for(int i = 0; i < 10; i++) {
    pthread_mutex_lock(&lock); // critical section start
    std::cout << "Hello #" << i << " from thread " << threadID << std::endl;
    pthread_mutex_unlock(&lock); // critical section slut
    sleep(1);
  }

  return NULL;
}

int main() {
  pthread_t threadID0, threadID1;
  pthread_mutex_init(&lock, NULL);

  int id0 = 0;
  int id1 = 1;

  pthread_create(&threadID0, NULL, printHello, &id0);
  pthread_create(&threadID1, NULL, printHello, &id1);

  pthread_join(threadID0, NULL);
  pthread_join(threadID1, NULL);

  pthread_mutex_destroy(&lock);

  return 0;
}