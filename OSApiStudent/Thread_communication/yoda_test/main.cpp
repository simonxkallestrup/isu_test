//
//  main.cpp
//  Formatting
//
//  Created by Søren Hansen on 19/10/15.
//  Copyright (c) 2015 DevelEdu. All rights reserved.
//

#include <array>
#include <iostream>
#include <vector>
#include <list>
#include <map>
#include <type_traits>
#include <memory>
#include <algorithm>
#include <pthread.h>
#include <unistd.h>
#include <thread>
#include <chrono>
#include <cmath>
#include <sstream>
#include <iomanip>
#include <memory>
//#include "Playground.hpp"
#include <tuple>
#include <queue>
#include <variant>
#include <semaphore.h>
//#include <libfmt10/fmt/core.h>
#include "MsgQueue.hpp"





pthread_mutex_t coutMut = PTHREAD_MUTEX_INITIALIZER;

#define LOG(...) do { \
    pthread_mutex_lock(&coutMut); \
    std::cout << "[" << __FILE__ << ":" << __LINE__ << "] (" << __PRETTY_FUNCTION__ << ") :" \
    << __VA_ARGS__;  \
    pthread_mutex_unlock(&coutMut); \
} while(0)


#define PP do { \
std::cout << __PRETTY_FUNCTION__ << std::endl; } while (0)

const int MAX_QUEUE_SIZE=10;


struct Pong
{
    MsgQueue mq{MAX_QUEUE_SIZE};
    int id{0};
    bool running{true};
    pthread_t tid;
};


struct PingMessageInd : Message
{
    int         count;
    std::string text;
};

enum {
    ID_PING_MESSAGE_IND

};

void* pingFunc(void* data)
{
    LOG("PIF Starting...\n");
    int count = 0;

    MsgQueue* pongMq = static_cast<MsgQueue*>(data);
    for(;;)
    {
        PingMessageInd* pingMessageInd = new PingMessageInd;
        pingMessageInd->count = ++count;
        pingMessageInd->text = "The answer to everything is '42'!";
        LOG("Sending ping message...\n");
        pongMq->send(ID_PING_MESSAGE_IND, pingMessageInd);
        sleep(1);
    }
    return nullptr;
}


void pongHandlerMsgIdPingMessageInd(Pong* pong, PingMessageInd* pingMessageInd)
{
    LOG("ID: " << pong->id << " :: Count: " << pingMessageInd->count << " - Text: '" << pingMessageInd->text << "'\n");
}

void pongHandler(Pong* pong, unsigned long id, Message* msg)
{
    switch (id) {
        case ID_PING_MESSAGE_IND:
            pongHandlerMsgIdPingMessageInd(pong, static_cast<PingMessageInd*>(msg));
            break;
            
        default:
            break;
    }
}


void* pongFunc(void* data)
{
    LOG("POF Starting...\n");
    Pong* pong = static_cast<Pong*>(data);
    
    while(pong->running)
    {
        // ...
        unsigned long id;
        Message* msg = pong->mq.receive(id);
        pongHandler(pong, id, msg);
        delete msg;
        
    }
    
    return nullptr;
}

int main(int argc, char *argv[])
{
    pthread_t tid1, tid2;

    LOG("Starting...\n");

    Pong pong;
    
    pthread_create(&tid1, nullptr, pingFunc, &pong.mq);
    pthread_create(&tid2, nullptr, pongFunc, &pong);
    pthread_join(tid1,nullptr);
    pthread_join(tid2,nullptr);

    return 0;
}