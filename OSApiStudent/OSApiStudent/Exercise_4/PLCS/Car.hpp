#include <osapi/MsgQueue.hpp>
#include <osapi/Message.hpp>
#include <osapi/ThreadFunctor.hpp>
#include <osapi/Thread.hpp>
#include <osapi/Log.hpp>


#include "EntryGuard.hpp"
#include "ExitGuard.hpp"


class Car : public osapi::ThreadFunctor
{
  public:
  Car(int id, EntryGuard *entryMq, ExitGuard *exitMq);
  enum{
    IND_CAR_START,
    IND_ENTRY_REQUEST_CFM,
    IND_EXIT_REQUEST_CFM};

  int getNrId();
  osapi::MsgQueue* getCarMq();
  
  protected:
  virtual void run();
  
  private:
  void handleMsg(int id, osapi::Message* msg);
  void handleCarStartInd(osapi::Message *carStart);
  void handleEntryRequestCfmInd(osapi::Message *entryRequestCfm);
  void handleexitRequestCfmInd(osapi::Message *exitRequestCfm);

  int carID_;
  osapi::MsgQueue carMq_{MQ_SIZE};
  EntryGuard* entryMq_;
  ExitGuard* exitMq_;
};


