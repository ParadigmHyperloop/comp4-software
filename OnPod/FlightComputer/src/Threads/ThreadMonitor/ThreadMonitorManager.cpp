#include "ThreadMonitorManager.h"


ThreadMonitorManager::ThreadMonitorManager(ThreadMonitorWrapper* canThreadWrapper,
                                          ThreadMonitorWrapper* navThreadMonitor,
                                          ThreadMonitorWrapper* pinThreadMonitor,
                                          ThreadMonitorWrapper* controlsInterfaceThreadMonitor)
{
  this->_canThreadMonitorWrapper = canThreadWrapper;
  this->_navTelemetryMonitorWrapper = navThreadMonitor;
  this->_pinThreadMonitorWrapper = pinThreadMonitor;
  this->_controlsInterfaceThreadMonitorWrapper= controlsInterfaceThreadMonitor;
}

ThreadMonitorManager::~ThreadMonitorManager()
{
  delete this->_canThreadMonitorWrapper;
}

void ThreadMonitorManager::SetCanTelemetryThreadManager(ThreadMonitorWrapper* canThreadMonitorWrapper)
{
  this->_canThreadMonitorWrapper = canThreadMonitorWrapper;
}


// Thread MonitorWrapper Getters.
ThreadMonitorWrapper*& ThreadMonitorManager::GetCanThreadMonitorWrapper()
{
  return _canThreadMonitorWrapper;
}

ThreadMonitorWrapper*& ThreadMonitorManager::GetNavThreadMonitorWrapper(){
  return _navTelemetryMonitorWrapper;
}

ThreadMonitorWrapper*& ThreadMonitorManager::GetPinThreadMonitorWrapper(){
  return _pinThreadMonitorWrapper;
}

ThreadMonitorWrapper*& ThreadMonitorManager::GetControlInterfaceThreadMonitorWrapper(){
  return _controlsInterfaceThreadMonitorWrapper;
}

// Thread MonitorWrapper Getters.

ThreadMonitorStatus ThreadMonitorManager::GetCanThreadStatus()
{
  return _canThreadMonitorWrapper->GetStatus();
}


ThreadMonitorStatus ThreadMonitorManager::GetNavThreadStatus(){
  return _navTelemetryMonitorWrapper->GetStatus();
}

ThreadMonitorStatus ThreadMonitorManager::GetPinThreadStatus(){
  return _pinThreadMonitorWrapper->GetStatus();
}

ThreadMonitorStatus ThreadMonitorManager::GetControlInterfaceThreadStatus(){
  return _controlsInterfaceThreadMonitorWrapper->GetStatus();
}



/*
 * Vector Order:
 *  (1) - Can Thread
 *  (2) - NavThread
 *  (3) - PinThread
 *  (4) - ControlInterface
 */
std::vector<ThreadMonitorStatus> ThreadMonitorManager::GetAllThreadStatus() {
  std::vector<ThreadMonitorStatus> threadsStatus;

  threadsStatus.push_back(this->GetCanThreadStatus());
  threadsStatus.push_back(this->GetNavThreadStatus());
  threadsStatus.push_back(this->GetPinThreadStatus());
  threadsStatus.push_back(this->GetControlInterfaceThreadStatus());

  return threadsStatus;
}

void ThreadMonitorManager::TEMP_FeedCanThreadMonitor() {
  this -> _canThreadMonitorWrapper->Feed();
}