
#include <stdio.h>
#include <stdlib.h>

#include "FlightComputer/PodInternalNetwork.h"




int main(int argc, char *argv[]) {

  socketConfig nodeSockets = initializeClientSocket();
  sendState(nodeSockets);
  killConfigSocket(nodeSockets);



  return 0;
}


