#ifndef CORECONTROL_H
#define CORECONTROL_H

#include "Common.h"
#include "TelemetryManager.h"
#include "ThreadMonitorManager.h"

void coreControlLoopThread(TelemetryManager, ThreadMonitorManager*&);

#endif
