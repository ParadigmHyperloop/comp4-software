#ifndef FLIGHTCOMPUTER_THREADMONITOR_H
#define FLIGHTCOMPUTER_THREADMONITOR_H


#include <chrono>
#include <ThreadMonitorStatus.h>
#include <mutex>

/*
 * NOTE:
 *  - Do not Reference / Use class directly.
 *  - Only use through the ThreadMonitorWrapper Public Interface
 */
class ThreadMonitor {
private:
  const long long _safeThreshold, _busyThreshold; // duration count() method returns a long long for milliseconds
  std::chrono::steady_clock::time_point _timeStamp;
public:
  explicit ThreadMonitor(long long safeThreshold = 200, long long busyThreshold = 500);
  ThreadMonitorStats GetStatus();
  void Feed();
};

#endif //FLIGHTCOMPUTER_THREADMONITOR_H
