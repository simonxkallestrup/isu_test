#ifndef OSAPI_LINUX_MUTEX_HPP
#define OSAPI_LINUX_MUTEX_HPP

#include <osapi/Utility.hpp>
#include <pthread.h>

namespace osapi
{
  // Forward declaration, needed for friend designation
  class Conditional;
  
  class Mutex : Notcopyable
  {
  public:
    Mutex();
    void lock();
    void unlock();
    ~Mutex();
  private:
    friend class Conditional;

    pthread_mutex_t  mut_;
  };  

}

#endif
