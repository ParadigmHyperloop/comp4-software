#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H

struct socketConfig {
  struct sockaddr_in* addr;
  int sckt;
  char* nodeIp[3];
  int nodeNum = 3;
};

void sendState(socketConfig);

socketConfig initializeClientSocket();

void killConfigSocket(socketConfig);

#endif
