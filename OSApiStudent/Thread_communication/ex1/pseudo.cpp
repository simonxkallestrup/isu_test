#include <pthread.h>
#include <queue>

using namespace std;

// the destructor must be virtual. Why is this last bit very important? Explain!
// Den er skal være virtuel, fordi metoden så kan blive overskrevet af enhver afledt klasse. 
// Så hvis en afledt class (child), lets say Supermessage(), har ekstra metoder. 
// Så er det vigtigt at SuperMessage() kan overskrive destructeren.

class Message
{
public:
    virtual ~Message() {}
};

class MsgQueue
{
public:
    MsgQueue(unsigned long maxSize){
        // Constructor
        // initialiser mutex og cond
    }
    
    void send(unsigned long id, Message* msg = NULL) { // is blocking if the internal queue is filled to the maximum denoted capacity.
        //lock mutex
        //vent på at der er plads i køen cond wait
        //noget med queue.push()
        //broadcast cond
        //unlock mutex
    }
    
    Message* receive(unsigned long& id) { // is likewise blocking if the queue is empty
        //lock mutex
        //vent på at der er noget i kø cond wait
        Message* message = queue.front(); // forreste besked i køen bliver modtaget
        //noget med queue.pop()
        //broadcast cond
        //unlock mutex
        return message;
    }

    ~MsgQueue() {
        // ryd op. tøm køen for beskeder

        //destroy mutex og cond
    }
private:
    unsigned long maxSize;
    queue<Message*> queue; // Container with messages
    // Plus other relevant variables
    // måske mutex og cond?
};