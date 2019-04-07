#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include "ProtoBuffer/Paradigm.pb.h"
#include "FlightComputer/structs.h"
#include "FlightComputer/Pod.h"

void sendDataUdp(clientSocketConfig*, const void*,  int32_t,  std::string);

clientSocketConfig initializeClientSocket();

int32_t createNodeServerSocket(int32_t);

int32_t nodeServerThread(Pod Pod);

void parseBreakNodePacket(brakeNodeData, Pod);

int32_t podInternalNetworkThread(Pod);

const char* getPodUpdateMessage(Pod*);

int32_t createCommanderServerSocket(int32_t);

void commanderThread(Pod);

#endif
