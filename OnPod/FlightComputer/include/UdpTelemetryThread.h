#ifndef FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
#define FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H

#include "Common.h"
#include "UdpConnection.h"
#include "ThreadMonitorWrapper.h"
#include <NetworkHelpers.h>

/**
 * Creates and configures the brake node connection
 *
 * @param Pod
 * @return A pointer to a BrakeNodeConnection
 */
UdpConnection *getBrakeNodeConnection(TelemetryManager);

UdpConnection *getRearNodeConnection(TelemetryManager);



int32_t udpTelemetryThread(TelemetryManager Pod, ThreadMonitorWrapper*);

#endif //FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
