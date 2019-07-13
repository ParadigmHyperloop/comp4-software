#include <ThreadMonitor.h>
#include <memory>
#include <chrono>
#include <ThreadMonitorStatus.h>
#include "ThreadMonitor.h"

ThreadMonitor::ThreadMonitor(long long safeThreshold, long long busyThreshold)
: _safeThreshold(safeThreshold), _busyThreshold(busyThreshold)
{
//  this->_busyThreshold = std::chrono::microseconds(busyThreshold);
//  this->_safeThreshold = std::chrono::microseconds(safeThreshold);
  this->_timeStamp = std::chrono::steady_clock::now();
}

ThreadMonitorStats ThreadMonitor::GetStatus() {
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      std::chrono::steady_clock::now() - this->_timeStamp).count();

  // TODO: Evaluate comparing long long to milliseconds comparison.
  if (duration < _safeThreshold)
  {
    return ThreadMonitorStats::Active;
  }
  else if (duration < _busyThreshold)
  {
    return ThreadMonitorStats::Busy;
  }
  else
  {
    return ThreadMonitorStats::Danger;
  }
}

void ThreadMonitor::Feed() {
  // TODO: wrapper.
  this->_timeStamp = std::chrono::steady_clock::now();
}