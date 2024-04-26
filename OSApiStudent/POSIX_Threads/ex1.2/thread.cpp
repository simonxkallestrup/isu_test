#include <iostream>
#include <pthread.h>
#include <stdio.h>

struct dataNString {
  int data;
  std::string string;
};

void *print_hello(void *dataNString) {
  // int mydata = *(int*)data;
  struct dataNString recievedStruct = *(struct dataNString *)dataNString;

  std::cout << "Hello from thread\n\tI have this recived this data: "
            << recievedStruct.data << "\n\tAnd this string: "
            << recievedStruct.string << std::endl;

  return NULL;
}

int main() {
  struct dataNString myStruct; // ikke krav med struct
  myStruct.data = 5;
  myStruct.string = "Hello";

  pthread_t threadID;

  pthread_create(&threadID, NULL, print_hello, &myStruct);

  pthread_join(threadID, NULL);

  printf("Created thread %lu ... \n", threadID);
}
