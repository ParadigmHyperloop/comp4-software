#include "ThreadMonitorManager.h"


ThreadMonitorManager::ThreadMonitorManager(std::shared_ptr<ThreadMonitorWrapper> canThreadWrapper) {
  this->_canThreadMonitorWrapper = canThreadWrapper;
}

void ThreadMonitorManager::SetCanTelemetryThreadManager(std::shared_ptr<ThreadMonitorWrapper> canThreadMonitorWrapper)
{
  this->_canThreadMonitorWrapper = canThreadMonitorWrapper;
}

ThreadMonitorStats ThreadMonitorManager::GetCanThreadStatus()
{
  return _canThreadMonitorWrapper->GetStatus();
}

std::shared_ptr<ThreadMonitorWrapper>& ThreadMonitorManager::GetCanThreadMonitorWrapper()
{
  return _canThreadMonitorWrapper;
}