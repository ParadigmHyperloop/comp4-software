#ifndef FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
#define FLIGHTCOMPUTER_THREADMONITORWRAPPER_H


#include <ThreadMonitor.h>

// Acts as ThreadMonitor Manager that manages safe access to resource

class ThreadMonitorWrapper {

private:
  ThreadMonitor* _threadMonitor;
  std::mutex* _mutex;
public:
  ThreadMonitorWrapper(ThreadMonitor* threadMonitor);
  ThreadMonitorStats GetStatus();
  void Feed();
};


#endif //FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
