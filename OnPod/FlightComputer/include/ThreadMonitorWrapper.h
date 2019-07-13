#ifndef FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
#define FLIGHTCOMPUTER_THREADMONITORWRAPPER_H

#include <memory>

#include "ThreadMonitor.h"
#include "ThreadMonitorFactory.h"

// Acts as ThreadMonitor Manager that manages safe access to resource
class ThreadMonitorWrapper {

private:
  std::shared_ptr<ThreadMonitor> _threadMonitor;
  std::unique_ptr<std::mutex> _mutex;

public:
  ThreadMonitorWrapper(ThreadMonitorFactory threadMonitorFactory);
  ThreadMonitorStats GetStatus();
  void Feed();
};

#endif //FLIGHTCOMPUTER_THREADMONITORWRAPPER_H