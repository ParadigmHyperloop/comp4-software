#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

//Logging System

#include "EasyLogger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include <Network.h>
#include "FlightComputer/nodeSim.h"
#include "FlightComputer/Pod.h"
#include "FlightComputer/CoreControl.h"


int main( int32_t argc, char** argv)
{
	// Logger
	el::Helpers::setThreadName("main");
	el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	LOG(INFO)<<"Main Thread Started";

	// Create Shared Memory
	PodValues pvPodValues;

	// Pod Internal Network Thread
	Pod pPodInternalNetwork = Pod(&pvPodValues);
	pPodInternalNetwork.bWriteBreakNodeState = true;
	std::thread tServer(podInternalNetworkThread, pPodInternalNetwork);

	// Core Control Loop Thread
	Pod pCoreControlLoop = Pod(&pvPodValues);
	pCoreControlLoop.bWritePodState = true;
	std::thread tControlLoop(coreControlLoop, pCoreControlLoop);

	// Controls Interface Connection Thread
	Pod pCommanderThread = Pod(&pvPodValues);
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);

	tControlsInterfaceConnection.join();
	tServer.join();
	tControlLoop.join();

	return 0;
}
