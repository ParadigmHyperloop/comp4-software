#ifndef FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H

#include <memory>

#include "ThreadMonitorWrapper.h"


class IThreadMonitorManager
{
public:
  virtual ThreadMonitorStats GetCanThreadStatus() = 0;
  virtual ThreadMonitorWrapper*& GetCanThreadMonitorWrapper() = 0;
  virtual void SetCanTelemetryThreadManager(ThreadMonitorWrapper* canThreadMonitor) = 0;
protected:
  ThreadMonitorWrapper* _canThreadMonitorWrapper;
};
#endif //FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
