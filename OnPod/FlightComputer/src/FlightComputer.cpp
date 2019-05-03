#include <stdio.h>
#include <thread>
#include <fstream>


#include <iostream>
//Logging System


#include "EasyLogger/easylogging++.h"

INITIALIZE_EASYLOGGINGPP


#include <FlightComputer/Network.h>
#include "FlightComputer/CoreControl.h"

using namespace std;

int main(int32_t argc, char **argv) {
    // Logger
    el::Helpers::setThreadName("main");
    std::ifstream infile("/home/debian/logging.conf");
    if (infile.good()) {
        el::Configurations conf("/home/debian/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);

    } else {
        //el::Configurations conf("/home/lwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
        el::Configurations conf(
                "/Users/liamwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);
    }
    LOG(INFO) << "Main Thread is Started";
    LOG(INFO) << std::thread::hardware_concurrency();


    // Create Shared Memory
    PodNetwork sPodNetworkValues;
    PodValues sPodValues;

    // Network Configs
    string cNodeIpAddrs[] = {"127.0.0.1"};
    sPodNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    sPodNetworkValues.iBrakeNodePort = 5000; // Port # that Nodes are listening on
    sPodNetworkValues.iNodeTimeoutMili = 3000;
    sPodNetworkValues.iBrakeNodeServerPortNumber = 5001; // Port # to receive UDP from Nodes

    sPodNetworkValues.iCommaderTimeoutMili = 30000; // Timeout for heartbeat to Control Interface
    sPodNetworkValues.iCommanderPortNumber = 5005; //Port # for TCP Commander

    sPodNetworkValues.iPdsTelemeteryPort = 6000; // Port # to send telemetry
    sPodNetworkValues.strPdsIpAddr = "127.0.0.1"; // Ip Addr of PDS.

    sPodNetworkValues.iActiveNodes[0] = 1; // Set brake node active


    //Pod Internal Network Thread
    Pod pPodInternalNetwork = Pod(&sPodValues, &sPodNetworkValues);
    pPodInternalNetwork.bWriteBreakNodeState = true;
    std::thread tServer(podInternalNetworkThread, pPodInternalNetwork);


    // Core Control Loop Thread
    Pod pCoreControlLoop = Pod(&sPodValues, &sPodNetworkValues);
    pCoreControlLoop.bWritePodState = true;
    std::thread tControlLoop(coreControlLoop, pCoreControlLoop);
/*
	// Controls Interface Connection Thread
	Pod pCommanderThread = Pod(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteManualStates = 1;
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);
*/
//	tControlsInterfaceConnection.join();

    tControlLoop.join();

    tServer.join();

    return 0;
}
