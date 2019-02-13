#include <thread>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "FlightComputer/nodeSim.h"
#include "FlightComputer/PodInternalNetwork.h"

int main(int argc, char **argv) {

   int sckt = createNodeServerSocket();

  clientSocketConfig* sckt2 = initializeClientSocket();

  std::thread server(nodeServerThread, sckt);

  std::thread sim(runNodeSimulator, sckt2);

  server.join();

  sim.join();

  return 0;
}
