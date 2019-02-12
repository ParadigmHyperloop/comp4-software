#include <NodeServer.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/ProtoStructs.pb.h"

int createNodeServerSocket() {
  int port = 5008;
  int sockfd;
  struct sockaddr_in si_me;
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  memset(&si_me, '\0', sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = inet_addr("127.0.0.1");
  bind(sockfd, (struct sockaddr*)&si_me, sizeof(si_me));
  return sockfd;
}

int nodeServerThread(int sckt) {
  char buffer[1024];
  while (1) {
    bzero(&buffer, sizeof buffer);
    printf("Waiting to recieve /n");
    recvfrom(sckt, buffer, 1024, 0, nullptr, nullptr);
    printf(buffer);

    // std::string payload = buffer;
    // fc::brakeNodeData nodeUpdate;
    // nodeUpdate.ParseFromString(payload);
    // printf(nodeUpdate.DebugString().c_str());
  }
  close(sckt);
  return 0;
}
