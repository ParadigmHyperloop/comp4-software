#include <stdio.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "Common.h"
INITIALIZE_EASYLOGGINGPP


#include "CoreControlThread.h" //todo make this a thread file
#include "FlightComputerInitializer.h"
#include "FlightConfigServer.h"
#include "CommanderThread.h"
#include "UdpTelemetryThread.h"
#include "Constants/Constants.h"

using namespace std;

int main( int32_t argc, char** argv)
{
	// Initialize Logger Logger;
	FlightComputerInitializer* initializer = FlightComputerInitializer::GetInstance();
	initializer->importLoggerLibrary();

	LOG(INFO)<<"Main Thread is Started";
	FlightConfigServer* configServer = FlightConfigServer::getServer(NetworkConstants::iCONFIG_SERVER_PORT);
	flightConfig flightConfig;
    char controlLaptopAddr[NI_MAXHOST] = {0};
    try {
        flightConfig = (*configServer)(controlLaptopAddr);
    } catch (...)
    {
        LOG(ERROR) << "Error Receiving Config: " << errno;
    }

    // Create Shared Memory
    PodNetwork sPodNetworkValues = {};
    PodValues sPodValues = {};


    // Network Configs
    initializer->updatePodNetworkValues(sPodNetworkValues, flightConfig, controlLaptopAddr);

    //Pod Internal Network Thread
    Pod pPodInternalNetwork = Pod(&sPodValues, &sPodNetworkValues);
    pPodInternalNetwork.bWriteBreakNodeState = true;
    std::thread tServer(udpTelemetryThread, pPodInternalNetwork);


    // Core Control Loop Thread
    Pod pCoreControlLoop = Pod(&sPodValues, &sPodNetworkValues);
    pCoreControlLoop.bWritePodState = true;
    std::thread tControlLoop(coreControlLoop, pCoreControlLoop);

	// Controls Interface Connection Thread
	Pod pCommanderThread = Pod(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteManualStates = 1;
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);






	tControlsInterfaceConnection.join();
    tControlLoop.join();
 	tControlsInterfaceConnection.join();
    tControlLoop.join();
   // tCanManager.join();
    tServer.join();
    int i;
    return 0;
}
