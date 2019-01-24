
#include <stdio.h>
#include <stdlib.h>

#include "FlightComputer/PodInternalNetwork.h"

struct pod {
  // States
  unsigned char podS;
  unsigned char terminalS;
  unsigned char motorS;
  unsigned char rearNodeS;
  // Navigation
  float distance;
  float velocity;
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
