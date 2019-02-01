
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/ProtoStructs.pb.h"

using namespace fc;

void temp() {
  int port = 5006;
  int sockfd;
  struct sockaddr_in si_me, si_other;
  char buffer[1024];
  socklen_t addr_size;

  sockfd = socket(AF_INET, SOCK_DGRAM, 0);

  memset(&si_me, '\0', sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(port);
  si_me.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sockfd, (struct sockaddr *)&si_me, sizeof(si_me));
  addr_size = sizeof(si_other);
  recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&si_other, &addr_size);
  printf("[+]Data Received: %s", buffer);
}

int main(int argc, char *argv[]) {
  // socketConfig nodeSockets = initializeClientSocket();
  // sendState(nodeSockets);
  // killConfigSocket(nodeSockets);

  brakeNodeData nodeUpdate;
  nodeUpdate.set_id(1);
  nodeUpdate.set_state(brakeNodeData::bnVenting);
  // Pressure Transducers
  nodeUpdate.set_hp(3);
  nodeUpdate.set_lp1(2);
  nodeUpdate.set_lp2(1);
  nodeUpdate.set_lp3(7);
  nodeUpdate.set_lp4(5);
  // Solenoids
  nodeUpdate.set_sol1(1);
  nodeUpdate.set_sol2(1);
  nodeUpdate.set_sol3(1);
  nodeUpdate.set_sol4(1);
  nodeUpdate.set_sol5(1);
  nodeUpdate.set_sol6(1);

  std::cout << nodeUpdate.DebugString();

  return 0;
}
