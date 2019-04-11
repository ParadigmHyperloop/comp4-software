#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>
#include <fstream>


#include <iostream>
//Logging System

#include "EasyLogger/easylogging++.h"
INITIALIZE_EASYLOGGINGPP


#include <FlightComputer/Network.h>
#include "FlightComputer/nodeSim.h"
#include "FlightComputer/Pod.h"
#include "FlightComputer/CoreControl.h"

using namespace std;

int main( int32_t argc, char** argv)
{
	// Logger
	el::Helpers::setThreadName("main");
	std::ifstream infile("/home/debian/logging.conf");
	if(infile.good())
	{
		el::Configurations conf("/home/debian/logging.conf");
		el::Loggers::reconfigureAllLoggers(conf);

	}
	else
	{
		el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
		//el::Configurations conf("/Users/liamwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
		el::Loggers::reconfigureAllLoggers(conf);
	}
	LOG(INFO)<<"Main Thread Started";


	// Create Shared Memory
	PodNetwork sPodNetworkValues;
	PodValues sPodValues;

	string cNodeIpAddrs[] =  {"192.168.7.1","127.0.0.1"};
	sPodNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs
	sPodNetworkValues.iCommanderPortNumber = 5005; //Port # for TCP Commander
	sPodNetworkValues.iNodePort = 5000; // Port # that Nodes are listening on
	sPodNetworkValues.iNodeServerPortNumber = 5010; // Port # to recieve UDP from Nodes



	// Pod Internal Network Thread
	//Pod pPodInternalNetwork = Pod(&sPodValues, &sPodNetworkValues);
	//pPodInternalNetwork.bWriteBreakNodeState = true;
	//std::thread tServer(podInternalNetworkThread, pPodInternalNetwork);


	// Core Control Loop Thread
	Pod pCoreControlLoop = Pod(&sPodValues, &sPodNetworkValues);
	pCoreControlLoop.bWritePodState = true;
	std::thread tControlLoop(coreControlLoop, pCoreControlLoop);

	// Controls Interface Connection Thread
	Pod pCommanderThread = Pod(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);

	tControlsInterfaceConnection.join();

	tControlLoop.join();

	//tServer.join();

	return 0;
}
