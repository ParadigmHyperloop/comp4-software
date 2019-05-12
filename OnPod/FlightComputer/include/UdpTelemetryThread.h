#ifndef FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
#define FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H

#include "Common.h"
#include "UdpConnection.h"

UdpConnection *getBrakeNodeConnection(Pod);

UdpConnection *getRearNodeConnection(Pod);

int32_t udpTelemetryThread(Pod Pod);

#endif //FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
