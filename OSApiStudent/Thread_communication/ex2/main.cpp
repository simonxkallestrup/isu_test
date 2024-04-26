#include <iostream>
#include <unistd.h> // bruges til sleep
#include "msgQueue.hpp"

using namespace std;
enum {ID_START_IND};

struct Point3D : public Message
{
    int x;
    int y;
    int z;
};

void handleMsg(unsigned long id, Message* msg);
void handlePoint3D(Point3D* point);

void* senderThread(void* data)
{
    MsgQueue* queue = static_cast<MsgQueue*>(data);
    int i = 0;
    bool running = true;
    while(running)
    {
        Point3D* point = new Point3D;
        point->x = i;
        point->y = i+1;
        point->z = i+2;
        i++;
        unsigned long id;
        queue->send(id, point); 
        sleep(1);// vent 1 sekund
    }
    return NULL;
}

void* receiverThread(void* data)
{
    MsgQueue* queue = static_cast<MsgQueue*>(data);
    for(;;)
    {
        unsigned long id;
        Message* msg = queue->receive(id);
        handleMsg(id, msg);
    }
    return NULL;
}

int main()
{
    MsgQueue msgQueue1(10);

    pthread_t sender, receiver;

    pthread_create(&sender, NULL, senderThread, &msgQueue1);
    pthread_create(&receiver, NULL, receiverThread, &msgQueue1);

    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);

    return 0;
}

void handleMsg(unsigned long id, Message* msg)
{
    switch(id)
    {
        case 0: // Det burde være en indikator med en enum
            handlePoint3D(static_cast<Point3D*>(msg));
            break;
        default:
            cout << "Unknown message id" << endl;
            break;
    }
}

void handlePoint3D(Point3D* point)
{
    cout << "Received Point3D: x=" << point->x << ", y=" << point->y << ", z=" << point->z << endl;
    delete point;
}