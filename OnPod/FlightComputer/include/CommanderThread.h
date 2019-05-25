#ifndef FLIGHTCOMPUTER_COMMANDER_H
#define FLIGHTCOMPUTER_COMMANDER_H

#endif //FLIGHTCOMPUTER_COMMANDER_H

#include "Common.h"

int32_t createCommanderServerSocket(int32_t iPortNumber);

void parseProtoCommand(PodCommand pPodCommand, TelemetryManager *Pod);

int32_t unserializeProtoMessage(TelemetryManager *Pod, char cBuffer[], int32_t iMessageSize);

int32_t commanderThread(TelemetryManager Pod);
