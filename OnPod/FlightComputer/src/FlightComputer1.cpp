
#include <stdio.h>
#include <stdlib.h>
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
  char *address[3] = {"127.0.0.1", "127.0.0.1", "127.0.0.1"};

  sendState(address, 3);
  return 0;
}
