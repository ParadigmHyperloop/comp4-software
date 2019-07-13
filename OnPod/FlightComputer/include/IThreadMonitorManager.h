#ifndef FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H

#include <memory>

#include "ThreadMonitorWrapper.h"


class IThreadMonitorManager
{
public:
  virtual void SetCanTelemetryThreadManager(std::shared_ptr<ThreadMonitorWrapper> canThreadMonitor) = 0;
  virtual ThreadMonitorStats GetCanThreadStatus() = 0;
  virtual std::shared_ptr<ThreadMonitorWrapper>& GetCanThreadMonitorWrapper() = 0;
protected:
  std::shared_ptr<ThreadMonitorWrapper> _canThreadMonitorWrapper;
};
#endif //FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
