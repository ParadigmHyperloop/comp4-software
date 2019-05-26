#include <stdio.h>
#include <thread>
#include <fstream>
#include <iostream>
#include "Common.h"
INITIALIZE_EASYLOGGINGPP


#include "CoreControlThread.h"
#include "FlightComputerInitializer.h"
#include "FlightConfigServer.h"
#include "CommanderThread.h"
#include "UdpTelemetryThread.h"
#include "CanBusThread.h"
#include "Constants/Constants.h"

using namespace std;

int main( int32_t argc, char** argv)
{
	// Initialize Logger Logger;
	FlightComputerInitializer* initializer = FlightComputerInitializer::GetInstance();
	initializer->importLoggerLibrary();

	LOG(INFO)<<"Main Thread is Started";
    std::unique_ptr<FlightConfigServer> configurationServer(FlightConfigServer::getServer(NetworkConstants::iCONFIG_SERVER_PORT));
	flightConfig flightConfigurationParameters;
    try {
       // flightConfigurationParameters = configurationServer->runServer(); //Comment out to use the default network values in the proto obj
    } catch (exception& e)
    {
        LOG(ERROR) << "Error Receiving Flight Configuration: "<< e.what(); //TODO Hardware reset?
    }

    // Create Shared Memory
    PodNetwork sPodNetworkValues = {};
    PodValues sPodValues = {};

    // Network Configs
    initializer->updatePodNetworkValues(sPodNetworkValues, flightConfigurationParameters);

    sPodValues.podState = std::move(PodState::createState(psBooting));

    //TelemetryManager Internal Network Thread
    TelemetryManager canBusAccessCard = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread tCan(canThread, canBusAccessCard);

    /*

    //TelemetryManager Internal Network Thread
    TelemetryManager pPodInternalNetwork = TelemetryManager(&sPodValues, &sPodNetworkValues);
    pPodInternalNetwork.bWriteBreakNodeState = true;
    std::thread tServer(udpTelemetryThread, pPodInternalNetwork);

	// Controls Interface Connection Thread
	TelemetryManager pCommanderThread = TelemetryManager(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteManualStates = 1;
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread tControlsInterfaceConnection(commanderThread, pCommanderThread);

 	tControlsInterfaceConnection.join();
    tServer.join();

    */
    tCan.join();


    return 0;
}
