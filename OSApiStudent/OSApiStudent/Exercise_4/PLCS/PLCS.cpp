#include <iostream>
#include <osapi/MsgQueue.hpp>
#include <osapi/Timer.hpp>
#include <osapi/Log.hpp>
#include <osapi/Utility.hpp>
#include <osapi/Thread.hpp>

#include "plcsConstants.hpp"
#include "EntryGuard.hpp"
#include "ExitGuard.hpp"
#include "Car.hpp"


int main(int argc, char* argv[])
{
  EntryGuard enG;
  ExitGuard exG;

  osapi::Thread entryThread(&enG);
  osapi::Thread exitThread(&exG);

  entryThread.start();
  exitThread.start();

  osapi::Thread *carThread[TOTAL_CARS];

  for (size_t i = 0; i < TOTAL_CARS; i++)
  {
    carThread[i] = new osapi::Thread(new Car(i, &enG, &exG));
    carThread[i]->start();
  }

  entryThread.join();
  exitThread.join();
}
