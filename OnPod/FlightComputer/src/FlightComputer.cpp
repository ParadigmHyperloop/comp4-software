#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

//Logging System

#include "EasyLogger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/nodeSim.h"
#include "FlightComputer/MemoryAccess.h"

int main(int argc, char** argv)
{
    el::Helpers::setThreadName("main");
    el::Loggers::reconfigureAllLoggers(conf);
    el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
    LOG(INFO)<<"Hello World!";
  PodValues pvPodValues;
  MemoryAccess* Pod = new MemoryAccess(pvPodValues);


  clientSocketConfig* iClientSocket = initializeClientSocket();

  std::thread tServer(nodeServerThread, Pod);

  std::thread tSim(runNodeSimulator, iClientSocket);
  tServer.join();
  tSim.join();
  return 0;
}
