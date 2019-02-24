#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "FlightComputer/ProtoStructs.pb.h"
#include "FlightComputer/structs.h"
#include "FlightComputer/MemoryAccess.h"

void sendDataUdp(clientSocketConfig*, void*, int);

clientSocketConfig* initializeClientSocket();

void killConfigSocket(clientSocketConfig*);

int createNodeServerSocket();

int nodeServerThread(MemoryAccess* Pod);

void parseBreakNodePacket(fc::brakeNodeData, MemoryAccess);

#endif
