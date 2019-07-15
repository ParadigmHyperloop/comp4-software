#ifndef FLIGHTCOMPUTER_THREADMONITORMANAGER_H
#define FLIGHTCOMPUTER_THREADMONITORMANAGER_H

#include <memory>
#include <vector>
#include "IThreadMonitorManager.h"
#include "ThreadMonitorWrapper.h"

class ThreadMonitorManager : IThreadMonitorManager {

public:
  ThreadMonitorManager(
      ThreadMonitorWrapper* canThreadWrapper,
      ThreadMonitorWrapper* navThreadMonitor,
      ThreadMonitorWrapper* pinThreadMonitor,
      ThreadMonitorWrapper* controlsInterfaceThreadMonitor
      );
  ~ThreadMonitorManager();
  void SetCanTelemetryThreadManager(ThreadMonitorWrapper* canThreadMonitorWrapper);

  // Wrapper.
  ThreadMonitorWrapper*& GetCanThreadMonitorWrapper();
  ThreadMonitorWrapper*& GetNavThreadMonitorWrapper();
  ThreadMonitorWrapper*& GetPinThreadMonitorWrapper();
  ThreadMonitorWrapper*& GetControlInterfaceThreadMonitorWrapper();

  //status
  ThreadMonitorStatus GetCanThreadStatus();
  ThreadMonitorStatus GetNavThreadStatus();
  ThreadMonitorStatus GetPinThreadStatus();
  ThreadMonitorStatus GetControlInterfaceThreadStatus();

  std::vector<ThreadMonitorStatus> GetAllThreadStatus();

  void TEMP_FeedCanThreadMonitor();
};

#endif //FLIGHTCOMPUTER_THREADMONITORMANAGER_H