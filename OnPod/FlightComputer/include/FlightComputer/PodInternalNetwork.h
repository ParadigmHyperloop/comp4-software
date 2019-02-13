#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H

#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include "FlightComputer/ProtoStructs.pb.h"
#include "FlightComputer/structs.h"



struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};


void sendDataUdp(clientSocketConfig*, void*);

clientSocketConfig* initializeClientSocket();

void killConfigSocket(clientSocketConfig*);

int createNodeServerSocket();

int nodeServerThread(int sckt);

#endif
