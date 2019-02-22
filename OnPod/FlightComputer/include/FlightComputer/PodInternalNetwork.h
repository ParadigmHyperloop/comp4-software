#ifndef PODINTERNALNETWORK_H
#define PODINTERNALNETWORK_H

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include "ProtoBuffer/ProtoStructs.pb.h"
#include "FlightComputer/structs.h"

void sendDataUdp(clientSocketConfig*, void*, int);

clientSocketConfig* initializeClientSocket();

void killConfigSocket(clientSocketConfig*);

int createNodeServerSocket();

int nodeServerThread(int, PodValues*);

void parseBreakNodePacket(fc::brakeNodeData, PodValues*);

#endif
