#include "Car.hpp"

Car::Car(int id, EntryGuard *entryMq, ExitGuard *exitMq) 
: carID_(id), entryMq_(entryMq), exitMq_(exitMq) {}

void Car::handleMsg(int id, osapi::Message* msg)
{
    switch(id)
    {
        case IND_CAR_START:
        handleCarStart(*msg);
        break;

        case IND_ENTRY_REQUEST_CFM:
        handleEntryRequestCfm(*msg);
        break;

        case IND_EXIT_REQUEST_CFM:
        handleExitRequestCfm(*msg);
        break;
        
        default:
        break;
    }
}

void Car::handleCarStartInd(osapi::Message carStartInd)
{
    
}


void Car::run()
{
  //carMq_.send();

  for(;;)
  {
    unsigned long id;
    osapi::Message* msg = carMq_.receive(id);
    handleMsg(id, msg);
    delete msg;
  }
}

int Car::getNrId()
{
  return carID_;
}