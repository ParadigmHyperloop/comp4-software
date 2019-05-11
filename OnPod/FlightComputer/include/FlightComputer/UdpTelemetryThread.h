#ifndef FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H
#define FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H

#endif //FLIGHTCOMPUTER_UDPTELEMETRYTHREAD_H

#include "FlightComputer/Common.h"
#include "FlightComputer/UdpConnection.h"

UdpConnection *getBrakeNodeConnection(Pod);

UdpConnection *getRearNodeConnection(Pod);

int32_t udpTelemetryThread(Pod Pod);
