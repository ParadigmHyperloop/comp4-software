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

using namespace std;

// Factory for initializing the telemetry struct
PodValues createTelemetryStruct(){
    PodValues sPodValues = {};
    // Vectors
    sPodValues.sensorFlags = std::vector<int32_t>(TOTAL_SENSOR_COUNT, 1);
    sPodValues.connectionFlags = std::vector<int32_t>(TOTAL_CONNECTION_COUNT,1);
    // States
    sPodValues.podState = std::move(PodState::createState(psStandby));
    sPodValues.controlsInterfaceState = ciNone;
    sPodValues.brakeNodeState = bnsNone;
    sPodValues.lvdcNodeState = lvdcNone;
    // Flight Profile
    sPodValues.motorTorque = 0;
    sPodValues.flightDistance = 0;
    sPodValues.maxFlightTime = 0;
    // Manual Control
    sPodValues.manualSolenoidConfiguration = std::vector<bool>(4,false);
    sPodValues.manualBrakeNodeState = bnsNone;
    sPodValues.manualPodState = psStandby;
    sPodValues.manualLvdcNodeState = lvdcNone;
    sPodValues.automaticTransitions = true;
    return sPodValues;
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
            flightConfigurationParameters = configurationServer->runServer(); //Comment out to use the default network values in the proto obj
        } catch (exception& e)
        {
            LOG(INFO) << "Error Receiving Flight Configuration: "<< e.what(); //TODO Hardware reset?
            configurationServer->closePorts();
            configReceived = false;
        }
	}


    // Create Shared Memory
    PodNetwork sPodNetworkValues = {};
    PodValues sPodValues = createTelemetryStruct();

    // Network Configs
    initializer->updatePodNetworkValues(sPodNetworkValues, flightConfigurationParameters);



    /*
    //CAN Thread
    TelemetryManager canBusAccessCard = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread tCan(canThread, canBusAccessCard);
    */

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


    return 0;
}
