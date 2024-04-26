#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


 unsigned int shared;


static void* incrementer(void* data) {
    while(1){
      shared++;
      sleep(1);
    }
  char *s = (char*) data;
  std:: cout << *s << " im working " << std:: endl;
    
  return (void*) s;
}

static void* reader(void* data) {
 
    while (1)
    {
      std::cout << "the number is: " << shared << std::endl;
      sleep(1);
    }

  char *s = (char*) data;
  std:: cout << *s << " im working " << std:: endl;
  return (void*) s;
}

int main() {
  pthread_t increment, read;

  shared = 0;

  for(int i = 0; i < 10; i++) {
    pthread_create(&increment, NULL, incrementer, &shared);
    pthread_join(increment, NULL);
    pthread_create(&read, NULL, reader, &shared);
    pthread_join(read, NULL);
    sleep(1);
  }
}