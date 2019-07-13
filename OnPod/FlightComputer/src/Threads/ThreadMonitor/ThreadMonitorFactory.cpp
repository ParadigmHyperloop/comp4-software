#include "ThreadMonitorFactory.h"

std::unique_ptr<ThreadMonitor> ThreadMonitorFactory::CreateInstance() {
  return std::make_unique<ThreadMonitor>();
}