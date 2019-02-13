#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H

#include <string>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>


struct clientSocketConfig {
  sockaddr_in addr;
  int sckt;
};


void sendDataUdp(clientSocketConfig*);

clientSocketConfig* initializeClientSocket();

void killConfigSocket(clientSocketConfig*);

int createNodeServerSocket();

int nodeServerThread(int sckt);

#endif
