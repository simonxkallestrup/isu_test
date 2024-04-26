#ifndef SCOPEDLOCKER_HPP_
#define SCOPEDLOCKER_HPP_
#include <pthread.h>

 pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
class ScopedLocker
{

    public:
    ScopedLocker(pthread_mutex_t &mutex) : mutex_{mutex}{

        pthread_mutex_lock(&mutex_);
    }

    ~ScopedLocker(){
        pthread_mutex_unlock(&mutex_);
    }

    private:
        pthread_mutex_t mutex_;

};



#endif