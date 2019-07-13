#ifndef FLIGHTCOMPUTER_THREADMONITORFACTORY_H
#define FLIGHTCOMPUTER_THREADMONITORFACTORY_H


#include <memory>
#include "ThreadMonitor.h"

class ThreadMonitorFactory {
public:
  static std::unique_ptr<ThreadMonitor> CreateInstance();
};

#endif //FLIGHTCOMPUTER_THREADMONITORFACTORY_H
