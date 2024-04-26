#include <osapi/ThreadFunctor.hpp>
#include "plcsConstants.hpp"

class EntryGuard : public osapi::ThreadFunctor
{
  public:
  enum{
    IND_ENTRY_REQUEST,
    IND_CAR_ENTERED
  }


  
  protected:
  virtual void run()
  {    
    for(;;)
    {
      unsigned long id;
      osapi::Message* msg = entryMq_.receive(id);
      handleMsg(id, msg);
      delete msg;
    }
  }

  private:
  void handleMsg(int id, osapi::Message *msg);
  osapi::MsgQueue entryMq_{MQ_SIZE};
  osapi::MsgQueue* carMq_;
};