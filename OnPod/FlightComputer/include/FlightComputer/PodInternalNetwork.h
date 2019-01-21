#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H

struct socketConfig {
  struct sockaddr_in* addr;
  int sckt;
};

void sendState(char* [], int, socketConfig);

socketConfig initializeClientSocket();

void killConfigSocket(socketConfig);

#endif
