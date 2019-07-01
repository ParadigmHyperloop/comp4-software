#ifndef FLIGHTCOMPUTER_CANBUSTHREAD_H
#define FLIGHTCOMPUTER_CANBUSTHREAD_H

#include "TelemetryManager.h"

#define TORQUE_LOW_B 0
#define TORQUE_HIGH_B 1
#define SPEED_LOW_B 2
#define SPEED_HIGH_B 3
#define DIRECTION_B 4
#define FORWARD 1
#define REVERSE 0
#define INVERTER_RUN_B 5
#define ON 1
#define OFF 0

int canNetworkThread(TelemetryManager Pod);

#endif //FLIGHTCOMPUTER_CANBUSTHREAD_H
