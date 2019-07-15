#include "ThreadMonitorWrapper.h"


ThreadMonitorWrapper::ThreadMonitorWrapper(ThreadMonitorFactory threadMonitorFactory)
{
  _threadMonitor = std::move(threadMonitorFactory.CreateInstance());
}

ThreadMonitorWrapper::~ThreadMonitorWrapper() {
  delete this-> _threadMonitor;
//  delete this-> _mutex;
}

ThreadMonitorStatus ThreadMonitorWrapper::GetStatus()
{
  std::lock_guard<std::mutex> lck(this->_mutex);
  return this->_threadMonitor->GetStatus();
}

void ThreadMonitorWrapper::Feed() {
  std::lock_guard<std::mutex> lck(this->_mutex);
  this->_threadMonitor->Feed();
}