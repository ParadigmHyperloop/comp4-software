#include <stdio.h>
#include <thread>
#include <fstream>


#include <iostream>
//Logging System


#include "EasyLogger/easylogging++.h"

INITIALIZE_EASYLOGGINGPP


#include "FlightComputer/NetworkHelpers.h"
#include "FlightComputer/CommanderThread.h"
#include "FlightComputer/CoreControl.h"
#include "FlightComputer/CanManager.h"
#include "FlightComputer/UdpTelemetryThread.h"

using namespace std;

int main(int32_t argc, char **argv) {
    // Logger
    el::Helpers::setThreadName("Main Thread");
    std::ifstream infile("/home/debian/logging.conf");
    if (infile.good()) {
        el::Configurations conf("/home/debian/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);

    } else {
        //el::Configurations conf("/home/liam/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
        el::Configurations conf("/Users/liamwaghorn/Development/comp4-software/OnPod/FlightComputer/include/EasyLogger/logging.conf");
        el::Loggers::reconfigureAllLoggers(conf);
    }
    LOG(INFO) << "Main Thread is Started";

    // Create Shared Memory
    PodNetwork sPodNetworkValues = {};
    PodValues sPodValues = {};

    // Network Configs
    string cNodeIpAddrs[] = {"192.168.0.50"};
    sPodNetworkValues.cNodeIpAddrs.assign(begin(cNodeIpAddrs), end(cNodeIpAddrs)); // Node IPs

    sPodNetworkValues.iBrakeNodePort = 5555; // Port # that Nodes are listening on
    sPodNetworkValues.iNodeTimeoutMili = 3000;
    sPodNetworkValues.iBrakeNodeServerPortNumber = 5555; // Port # to receive UDP from Nodes

    sPodNetworkValues.iCommaderTimeoutMili = 400000; // Timeout for heartbeat to Control Interface


    sPodNetworkValues.iCommanderPortNumber = 5007; //Port # for TCP Commander

    sPodNetworkValues.iPdsTelemeteryPort = 6000; // Port # to send telemetry
    sPodNetworkValues.strPdsIpAddr = "127.0.0.1"; // Ip Addr of PDS.

    sPodNetworkValues.iActiveNodes[0] = 1; // Set brake node active




    //CAN Thread
    Pod pCanManager = Pod(&sPodValues, &sPodNetworkValues);
    std::thread tCanManager(CanThread, pCanManager);

      //Node & PDS Telemetry Thread
    Pod pPodInternalNetwork = Pod(&sPodValues, &sPodNetworkValues);
    pPodInternalNetwork.bWriteBreakNodeState = true;
    std::thread tServer(udpTelemetryThread, pPodInternalNetwork);

    /*
    // Core Control Loop Thread
    Pod pCoreControlLoop = Pod(&sPodValues, &sPodNetworkValues);
    pCoreControlLoop.bWritePodState = true;
    std::thread tControlLoop(coreControlLoop, pCoreControlLoop);

	// Controls Interface Connection Thread
	Pod pCommanderThread = Pod(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteManualStates = 1;
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);
*/



//	tControlsInterfaceConnection.join();
//    tControlLoop.join();
    tCanManager.join();
//    tServer.join();

    return 0;
}
