#ifndef FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H

#include <memory>

#include "ThreadMonitorWrapper.h"


class IThreadMonitorManager
{
public:
  virtual ThreadMonitorStatus GetCanThreadStatus() = 0;
  virtual ThreadMonitorWrapper*& GetCanThreadMonitorWrapper() = 0;
  virtual ThreadMonitorWrapper*& GetNavThreadMonitorWrapper() = 0;
  virtual ThreadMonitorWrapper*& GetPinThreadMonitorWrapper() = 0;
  virtual ThreadMonitorWrapper*& GetControlInterfaceThreadMonitorWrapper() = 0;
//  virtual void SetCanTelemetryThreadManager(
//      ThreadMonitorWrapper* canThreadMonitor,
//      ThreadMonitorWrapper* navThreadMonitor,
//      ThreadMonitorWrapper* pinThreadMonitor,
//      ThreadMonitorWrapper* controlsInterfaceThreadMonitor,
//      ) = 0;
protected:
  ThreadMonitorWrapper* _canThreadMonitorWrapper;
  ThreadMonitorWrapper* _navTelemetryMonitorWrapper;
  ThreadMonitorWrapper* _pinThreadMonitorWrapper;
  ThreadMonitorWrapper* _controlsInterfaceThreadMonitorWrapper;
};
#endif //FLIGHTCOMPUTER_ITHREADMONITORMANAGER_H
