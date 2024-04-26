#include <osapi/ThreadFunctor.hpp>
#include "plcsConstants.hpp"

class ExitGuard : public osapi::ThreadFunctor
{
  public:
  enum{
    IND_EXIT_REQUEST,
    IND_CAR_EXITED
  };
  
  protected:
  virtual void run()
  {    
    for(;;)
    {
      unsigned long id;
      osapi::Message* msg = exitMq_.receive(id);
      handleMsg(id, msg);
      delete msg;
    }
  }

  private:
  void handleMsg(int id, osapi::Message* msg);

  osapi::MsgQueue exitMq_{MQ_SIZE};
  osapi::MsgQueue* carMq_;
};