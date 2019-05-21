#ifndef FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
#define FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H

#include "Common.h"
#include "UdpConnection.h"
#include <NetworkHelpers.h>

/**
 * Creates and configures the brake node connection
 *
 * @param Pod
 * @return A pointer to a BrakeNodeConnection
 */
UdpConnection *getBrakeNodeConnection(Pod);

UdpConnection *getRearNodeConnection(Pod);



int32_t udpTelemetryThread(Pod Pod);

#endif //FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
