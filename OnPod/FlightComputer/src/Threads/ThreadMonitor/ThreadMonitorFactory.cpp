#include "ThreadMonitorFactory.h"

ThreadMonitor* ThreadMonitorFactory::CreateInstance() {
  return new ThreadMonitor();
}