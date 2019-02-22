#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

//Logging System

#include "EasyLogger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include "FlightComputer/PodInternalNetwork.h"
#include "FlightComputer/nodeSim.h"

int main(int argc, char** argv)
{

    // TODO: Get this to something along the lines of a realtive path. I tried "../include/EasyLogger/logging.conf" but no luck
    el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
    el::Loggers::reconfigureAllLoggers(conf);
    el::Helpers::setThreadName("main");

    LOG(INFO)<<"Hello World!";

    PodValues* pvPodValues;
	int iServerSocket = createNodeServerSocket();
	clientSocketConfig* iClientSocket = initializeClientSocket();
	std::thread tServer(nodeServerThread, iServerSocket, pvPodValues);
	std::thread tSim(runNodeSimulator, iClientSocket);
	tServer.join();
	tSim.join();


	return 0;
}
