#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H
#include <string>
#include <stdlib.h>

struct clientSocketConfig {
  struct sockaddr_in* addr;
  int sckt;
  char* nodeIp[1] = {"127.0.0.1"};
  int nodeNum = 1;
};

void sendDataUdp(clientSocketConfig, std::string *);

clientSocketConfig initializeClientSocket();

void killConfigSocket(clientSocketConfig);

int createNodeServerSocket();

int nodeServerThread(int sckt);


#endif
