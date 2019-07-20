#include <stdio.h>
#include <thread>
#include <fstream>
#include <signal.h>
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
#include "SevenSegStateDisplay.hpp"
#include "NavigationThread.h"

using namespace std;

void signal_callback_handler(int signum){

    LOG(INFO) << "Caught signal SIGPIPE ";
}

// Factory for initializing the telemetry struct
void initializeTelemetryStruct(PodValues &telemetry){
    telemetry.nodeSensorFlags = std::vector<int32_t>(NODE_FLAGS::NODE_SENSOR_COUNT, 0);
    telemetry.inverterSensorFlags = std::vector<int32_t>(INVERTER_FLAGS::INVERTER_SENSOR_COUNT, 0);
    telemetry.bmsSensorFlags = std::vector<int32_t>(BMS_FLAGS::BMS_SENSOR_COUNT, 0);
    telemetry.connectionFlags = std::vector<int32_t>(CONNECTION_FLAGS::TOTAL_CONNECTION_COUNT,0);
    telemetry.manualSolenoidConfiguration = std::vector<bool>(4,false);
    telemetry.cellVoltages = std::vector<float>(GENERAL_CONSTANTS::HV_CELL_COUNT, 0);
}

void TestSevenSegment(SevenSegStateDisplay* stateDisplay)
{
  for (int i = 0; i < 10; i++)
  {
    stateDisplay->TestDisplay(i);

    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
  }

}


int main(int32_t argc, char** argv)
{
    signal(SIGPIPE, signal_callback_handler);

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

    SevenSegStateDisplay* stateDisplay = initializer -> GetSevenSegStateDisplay();
    TestSevenSegment(stateDisplay); // TODO: remove after testing.
  TelemetryManager controlTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues, stateDisplay );

//    sPodValues.stateDisplay = std::move(stateDisplay);


    // Initiate Threads.
    controlTelemManager.setPodState(psBooting, (std::string)"Booting..."); // Set Pod state to booting
    std::thread coreControlThread(coreControlLoopThread, controlTelemManager);

    //CAN Thread
    TelemetryManager canTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues, stateDisplay);
    std::thread canThread(canNetworkThread, canTelemManager);


   // Navigation Thread
    TelemetryManager navTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues, stateDisplay);
    std::thread navThread(NavigationThread, navTelemManager);


    //TelemetryManager Internal Network Thread
    TelemetryManager pinTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues, stateDisplay);
    std::thread pinThread(udpTelemetryThread, pinTelemManager);

	// Controls Interface Connection Thread
	TelemetryManager pCommanderThread = TelemetryManager(&sPodValues, &sPodNetworkValues, stateDisplay);
	std::thread controlsInterfaceThread(commanderThread, pCommanderThread);

	coreControlThread.join();
    canThread.join();
    pinThread.join();
    controlsInterfaceThread.join();

    return 0;
}
