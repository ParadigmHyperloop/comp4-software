#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/nodeSim.h"
#include "FlightComputer/MemoryAccess.h"

int main(int argc, char** argv)
{
  PodValues pvPodValues;
  MemoryAccess* Pod = new MemoryAccess(pvPodValues);


  clientSocketConfig* iClientSocket = initializeClientSocket();

  std::thread tServer(nodeServerThread, Pod);

  std::thread tSim(runNodeSimulator, iClientSocket);
  tServer.join();
  tSim.join();
  return 0;
}
