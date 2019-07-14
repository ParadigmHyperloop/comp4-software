#ifndef FLIGHTCOMPUTER_THREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_THREADMONITORMANAGER_H

#include <memory>
#include "IThreadMonitorManager.h"
#include "ThreadMonitorWrapper.h"

class ThreadMonitorManager : IThreadMonitorManager {

public:
  ThreadMonitorManager(ThreadMonitorWrapper* canThreadWrapper);
  ~ThreadMonitorManager();
  void SetCanTelemetryThreadManager(ThreadMonitorWrapper* canThreadMonitorWrapper);
  ThreadMonitorStats GetCanThreadStatus();
  ThreadMonitorWrapper*& GetCanThreadMonitorWrapper();
  void TEMP_FeedCanThreadMonitor();
};

#endif //FLIGHTCOMPUTER_THREADMONITORMANAGER_H