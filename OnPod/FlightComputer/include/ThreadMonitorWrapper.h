#ifndef FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
#define FLIGHTCOMPUTER_THREADMONITORWRAPPER_H

#include <memory>

#include "ThreadMonitor.h"
#include "ThreadMonitorFactory.h"

// Acts as ThreadMonitor Manager that manages safe access to resource fo 1 Thread Monitor
class ThreadMonitorWrapper {

private:
  ThreadMonitor* _threadMonitor;
  std::mutex _mutex;

public:
  ThreadMonitorWrapper(ThreadMonitorFactory threadMonitorFactory);
  ~ThreadMonitorWrapper();
  ThreadMonitorStats GetStatus();
  void Feed();
};

#endif //FLIGHTCOMPUTER_THREADMONITORWRAPPER_H