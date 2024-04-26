#include <pthread.h>
#include <queue>
#include "ScopedLocker.hpp"

using namespace std;

class Message
{
public:
    virtual ~Message() {}
};

class MsgQueue
{
public:
    MsgQueue(unsigned long maxSize){ // Constructor  
        maxSize_ = maxSize;       
        pthread_mutex_init(&mutex_, NULL); // initialiser mutex og cond
        pthread_cond_init(&cond_, NULL);
    }
    
    void send(unsigned long id, Message* msg = NULL) { // is blocking if the internal queue is filled to the maximum denoted capacity.
        ScopedLocker lock(mutex_);//lock mutex, unlock automatisk
        while (queue.size() == maxSize_) //vent på at der er plads i køen cond wait
        {
            pthread_cond_wait(&cond_, &mutex_);
        }
        queue.push(msg); //noget med queue.push()
        pthread_cond_broadcast(&cond_); //broadcast cond
    }
    
    Message* receive(unsigned long& id) { // is likewise blocking if the queue is empty
        ScopedLocker lock(mutex_); //lock mutex, unlock automatisk
        while(queue.empty()) //vent på at der er noget i kø cond wait
        {
            pthread_cond_wait(&cond_, &mutex_);
        }
        Message* message = queue.front(); //Forreste besked i køen bliver modtaget
        queue.pop(); // fjerner forreste besked i køen
        pthread_cond_broadcast(&cond_); //broadcast cond
        return message;
    }

    ~MsgQueue() {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&cond_);
        while(queue.empty()==false) // ryd op. tøm køen for beskeder       
        {
            queue.pop();
        }
        
    }
private:
    unsigned long maxSize_;
    queue<Message*> queue; // Container with messages
    pthread_mutex_t mutex_;
    pthread_cond_t cond_;
};