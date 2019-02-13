#include "FlightComputer/PodInternalNetwork.h"


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
  char buffer[300];
  while (1) {
    bzero(&buffer, sizeof buffer);
    printf("Waiting to recieve on socket: %i \n", sckt);

    recvfrom(sckt, buffer, 300, 0, nullptr, nullptr);

    fc::brakeNodeData nodeUpdate2;

    bool parsed = nodeUpdate2.ParseFromArray(&buffer, 44);
    printf("Parse status: %i \n", parsed );
    printf("Contents are: \n%s \n", nodeUpdate2.DebugString().c_str());

  }
  close(sckt);
  return 0;
}

