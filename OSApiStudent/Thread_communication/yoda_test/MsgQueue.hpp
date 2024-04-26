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
    }
    
    void send(unsigned long id, Message* msg = NULL) { // is blocking if the internal queue is filled to the maximum denoted capacity.
        ScopedLocker lock(mutex_);//lock mutex, unlock automatisk
        while (msgQueue.size() == maxSize_) //vent på at der er plads i køen cond wait
        {
            pthread_cond_wait(&writeCond_, &mutex_);
        }
        Item temp;
        temp.id_ = id;
        temp.msg_ = msg;
        msgQueue.push(temp); //noget med queue.push()
        pthread_cond_signal(&readCond_); //broadcast cond
    }
    
    Message* receive(unsigned long& id) { // is likewise blocking if the queue is empty
        ScopedLocker lock(mutex_); //lock mutex, unlock automatisk
        while(msgQueue.empty()) //vent på at der er noget i kø cond wait
        {
            pthread_cond_wait(&readCond_, &mutex_);
        }
        Message* message = msgQueue.front().msg_; //Forreste besked i køen bliver modtaget
        id = msgQueue.front().id_;
        msgQueue.pop(); // fjerner forreste besked i køen
        pthread_cond_signal(&writeCond_); //broadcast cond
        return message;
    }

    ~MsgQueue() {
        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&writeCond_);
        pthread_cond_destroy(&readCond_);
        while(msgQueue.empty()==false) // ryd op. tøm køen for beskeder       
        {
            msgQueue.pop();
        }
        
    }
private:
    struct Item
    {
        unsigned long id_;
        Message* msg_;
    };

    unsigned long maxSize_;
    queue<Item> msgQueue; // Container with messages
    pthread_mutex_t mutex_ = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t writeCond_ = PTHREAD_COND_INITIALIZER, readCond_ = PTHREAD_COND_INITIALIZER;
};