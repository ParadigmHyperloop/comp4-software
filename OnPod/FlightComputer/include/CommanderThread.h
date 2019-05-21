#ifndef FLIGHTCOMPUTER_COMMANDER_H
#define FLIGHTCOMPUTER_COMMANDER_H

#endif //FLIGHTCOMPUTER_COMMANDER_H

#include "Common.h"

int32_t createCommanderServerSocket(int32_t iPortNumber);

void parseProtoCommand(PodCommand pPodCommand, Pod *Pod);

int32_t unserializeProtoMessage(Pod *Pod, char cBuffer[], int32_t iMessageSize);

int32_t commanderThread(Pod Pod);
