#include <osapi/Utility.hpp>
#include <unistd.h>

namespace osapi{

    void sleep(unsigned long msecs)
    {
        usleep(1000*msecs);
    }
}