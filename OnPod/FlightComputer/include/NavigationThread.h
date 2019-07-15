
#ifndef FLIGHTCOMPUTER_NAVIGATIONTHREAD_H
#define FLIGHTCOMPUTER_NAVIGATIONTHREAD_H

#include "Common.h"
#include "TelemetryManager.h"
#include "ThreadMonitorManager.h"

int32_t NavigationThread(TelemetryManager Pod, ThreadMonitorWrapper*);


#endif //FLIGHTCOMPUTER_NAVIGATIONTHREAD_H
