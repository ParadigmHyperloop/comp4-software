#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "ProtoBuffer/NodeTelem.pb.h"
#include "FlightComputer/structs.h"
#include "FlightComputer/MemoryAccess.h"

void sendDataUdp(clientSocketConfig*, void*,  int32_t);

clientSocketConfig* initializeClientSocket();

void killConfigSocket(clientSocketConfig*);

 int32_t createNodeServerSocket();

 int32_t nodeServerThread(MemoryAccess* Pod);

void parseBreakNodePacket(fc::brakeNodeData, MemoryAccess);

#endif
