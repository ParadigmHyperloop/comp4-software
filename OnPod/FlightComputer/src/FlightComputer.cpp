
#include <stdio.h>
#include <stdlib.h>

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include "FlightComputer/PodInternalNetwork.h"

struct pod {
  // States
  unsigned char podS = 0;
  unsigned char terminalS = 0;
  unsigned char motorS = 0;
  unsigned char rearNodeS;
  // Navigation
  float distance = 0;
  float velocity = 0;
  // Rear Node
  float gpioVals;
};

int main(int argc, char *argv[]) {
  socketConfig socketInfo = initializeClientSocket();
  char *nodeIp[3] = {"127.0.0.1", "127.0.0.1", "127.0.0.1"};

  sendState(nodeIp, 3, socketInfo);
  killConfigSocket(socketInfo);

  return 0;
}
