#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

//Logging System

#include "EasyLogger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include <Client.h>
#include "FlightComputer/nodeSim.h"
#include "FlightComputer/Pod.h"

int main( int32_t argc, char** argv)
{
	el::Helpers::setThreadName("main");
	el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
	el::Loggers::reconfigureAllLoggers(conf);
	LOG(INFO)<<"Main Thread Started";

	PodValues pvPodValues;
	Pod* pPodInternalNetwork = new Pod(&pvPodValues);
	pPodInternalNetwork->bWriteBreakNodeState = true;

	std::thread tServer(podInternalNetworkThread, pPodInternalNetwork);

	tServer.join();

	return 0;
}
