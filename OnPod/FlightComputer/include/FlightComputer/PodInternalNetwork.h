#ifndef PODINTERNALNETWORK_H

#define PODINTERNALNETWORK_H

struct socketConfig {
  struct sockaddr_in* addr;
  int sckt;
};

void sendState(char* [], int);

void sendState2(char* [], int, socketConfig);

socketConfig initializeClientSocket();

void killClientSocket();

#endif
