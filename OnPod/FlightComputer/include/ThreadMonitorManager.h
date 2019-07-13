#ifndef FLIGHTCOMPUTER_THREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_THREADMONITORMANAGER_H


#include <memory>
#include <ThreadMonitorWrapper.h>
#include "IThreadMonitorManager.h"

class ThreadMonitorManager : IThreadMonitorManager {

public:
  ThreadMonitorManager(std::shared_ptr<ThreadMonitorWrapper> canThreadWrapper);
  void SetCanTelemetryThreadManager(std::shared_ptr<ThreadMonitorWrapper> canThreadMonitorWrapper);
  ThreadMonitorStats GetCanThreadStatus();
  std::shared_ptr<ThreadMonitorWrapper>& GetCanThreadMonitorWrapper();
};

#endif //FLIGHTCOMPUTER_THREADMONITORMANAGER_H