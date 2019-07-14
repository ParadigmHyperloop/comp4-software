#include "ThreadMonitorManager.h"


ThreadMonitorManager::ThreadMonitorManager(ThreadMonitorWrapper* canThreadWrapper) {
  this->_canThreadMonitorWrapper = canThreadWrapper;
}

ThreadMonitorManager::~ThreadMonitorManager()
{
  delete this->_canThreadMonitorWrapper;
}

void ThreadMonitorManager::SetCanTelemetryThreadManager(ThreadMonitorWrapper* canThreadMonitorWrapper)
{
  this->_canThreadMonitorWrapper = canThreadMonitorWrapper;
}

ThreadMonitorStats ThreadMonitorManager::GetCanThreadStatus()
{
  return _canThreadMonitorWrapper->GetStatus();
}

ThreadMonitorWrapper*& ThreadMonitorManager::GetCanThreadMonitorWrapper()
{
  return _canThreadMonitorWrapper;
}

void ThreadMonitorManager::TEMP_FeedCanThreadMonitor() {
  this -> _canThreadMonitorWrapper->Feed();
}