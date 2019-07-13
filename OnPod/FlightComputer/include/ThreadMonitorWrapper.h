#ifndef FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
#define FLIGHTCOMPUTER_THREADMONITORWRAPPER_H


#include <ThreadMonitor.h>

// Acts as ThreadMonitor Manager that manages safe access to resource

class ThreadMonitorWrapper {

private:
  //ThreadMonitor _threadMonitor;
  std::shared_ptr<ThreadMonitor> _threadMonitor;
  std::unique_ptr<std::mutex> _mutex; //TODO: make unique pointer.
public:
  ThreadMonitorWrapper(ThreadMonitorFactory threadMonitorFactory);
  ThreadMonitorStats GetStatus();
  void Feed();
};

class ThreadMonitorFactory
{
  public:
    static std::unique_ptr<ThreadMonitor> createThreadMonitor()
    {
      return std::make_unique<ThreadMonitor>();
    }

};

#endif //FLIGHTCOMPUTER_THREADMONITORWRAPPER_H
