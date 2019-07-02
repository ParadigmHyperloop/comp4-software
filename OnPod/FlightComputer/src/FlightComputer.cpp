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
#include "Constants/SensorConfig.h"
#include "NavigationThread.h"

using namespace std;

// Factory for initializing the telemetry struct
void initializeTelemetryStruct(PodValues &telemetry){
    telemetry.nodeSensorFlags = std::vector<int32_t>(NODE_SENSOR_COUNT, 2);
    telemetry.inverterSensorFlags = std::vector<int32_t>(INVERTER_SENSOR_COUNT, 2);
    telemetry.bmsSensorFlags = std::vector<int32_t>(BMS_SENSOR_COUNT, 2);
    telemetry.connectionFlags = std::vector<int32_t>(TOTAL_CONNECTION_COUNT,2);
    telemetry.manualSolenoidConfiguration = std::vector<bool>(4,false);
}


int main( int32_t argc, char** argv)
{
	// Initialize Logger Logger;
	FlightComputerInitializer* initializer = FlightComputerInitializer::GetInstance();
	initializer->importLoggerLibrary();

	LOG(INFO)<<"Main Thread is Started";
    std::unique_ptr<FlightConfigServer> configurationServer(FlightConfigServer::getServer(NetworkConstants::iCONFIG_SERVER_PORT));
	FlightConfig flightConfigurationParameters;
    bool configReceived = false;
	while(!configReceived){
	    configReceived = true;
        try {
          // flightConfigurationParameters = configurationServer->runServer(); //Comment out to use the default network values in the proto obj
        } catch (exception& e)
        {
            LOG(INFO) << "Error Receiving Flight Configuration: "<< e.what(); //TODO Hardware reset?
            configurationServer->closePorts();
            configReceived = false;
        }
	}


    // Create Shared Memory
    PodNetwork sPodNetworkValues = {};
    PodValues sPodValues = {};
    initializeTelemetryStruct(sPodValues);

    // Network Configs
    initializer->updatePodNetworkValues(sPodNetworkValues, flightConfigurationParameters);

    //Core Control Loop Thread
    TelemetryManager controlTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    controlTelemManager.bWritePodState = true;
    controlTelemManager.setPodState(psBooting, (std::string)"Booting..."); // Set Pod state to booting
    std::thread coreControlThread(coreControlLoopThread, controlTelemManager);

    //CAN Thread
    TelemetryManager canTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    canTelemManager.bWriteInverter = true;
    canTelemManager.bWriteBms = true;
    std::thread canThread(canNetworkThread, canTelemManager);


    //Navigation Thread
    TelemetryManager navTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread navThread(NavigationThread, navTelemManager);


    //TelemetryManager Internal Network Thread
    TelemetryManager pinTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread pinThread(udpTelemetryThread, pinTelemManager);

	// Controls Interface Connection Thread
	TelemetryManager pCommanderThread = TelemetryManager(&sPodValues, &sPodNetworkValues);
	pCommanderThread.bWriteManualStates = 1;
	pCommanderThread.bWriteControlsInterfaceState = 1;
	std::thread controlsInterfaceThread(commanderThread, pCommanderThread);

	coreControlThread.join();
    canThread.join();
    pinThread.join();
    controlsInterfaceThread.join();

    return 0;
}
