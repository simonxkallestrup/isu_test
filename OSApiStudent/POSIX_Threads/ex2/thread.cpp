#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

struct threadData {
  int data;
  int threadID0;
  int threadID1;
};

void* t0(void* data) {
  struct threadData recivedData = *(struct threadData*)data;

  std::cout << "#"<< recivedData.data << " Hello from thread " << recivedData.threadID0 << std::endl;

  return NULL;
}

void* t1(void* data) {
  struct threadData recivedData = *(struct threadData*)data;

  std::cout << "#"<< recivedData.data << " Hello from thread " << recivedData.threadID1 << std::endl;

  return NULL;
}

int main() {
  pthread_t threadID0, threadID1;

  struct threadData myData;
  myData.threadID0 = threadID0;
  myData.threadID1 = threadID1;

  for(myData.data = 0; myData.data < 10; myData.data++) {
    pthread_create(&threadID0, NULL, t0, &myData);
    pthread_join(threadID0, NULL);
    pthread_create(&threadID1, NULL, t1, &myData);
    pthread_join(threadID1, NULL);
    sleep(1);
  }
}