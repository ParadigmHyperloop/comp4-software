#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/nodeSim.h"

int main(int argc, char** argv) {
  int sckt = createNodeServerSocket();

  clientSocketConfig* sckt2 = initializeClientSocket();

  std::thread server(nodeServerThread, sckt);

  std::thread sim(runNodeSimulator, sckt2);

  server.join();

  sim.join();

  return 0;
}
