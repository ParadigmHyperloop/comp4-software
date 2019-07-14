#include <stdio.h>
#include <thread>
#include <fstream>
#include <signal.h>
#include <iostream>
#include <ThreadMonitorWrapper.h>
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
#include "IThreadMonitorManager.h"
#include "ThreadMonitorManager.h"

using namespace std;

void signal_callback_handler(int signum){

    LOG(INFO) << "Caught signal SIGPIPE ";
}

// Factory for initializing the telemetry struct
void initializeTelemetryStruct(PodValues &telemetry){
    telemetry.nodeSensorFlags = std::vector<int8_t>(NODE_SENSOR_COUNT, 0);
    telemetry.inverterSensorFlags = std::vector<int8_t>(INVERTER_SENSOR_COUNT, 0);
    telemetry.bmsSensorFlags = std::vector<int8_t>(BMS_SENSOR_COUNT, 0);
    telemetry.connectionFlags = std::vector<int8_t>(TOTAL_CONNECTION_COUNT,0);
    telemetry.manualSolenoidConfiguration = std::vector<bool>(4,false);
}


int main( int32_t argc, char** argv)
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
          // flightConfigurationParameters = configurationServer->runServer(); //Comment out to use the default network values in the proto obj
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



  //==================================================================================
  //==================================================================================
  //==================================================================================
  // TODO:
  //  - Instantiate ThreadMonitoringWrapper and pass to Threads
  //  - Periodically Check status in Main Thread IE: threadMWrapper.GetStatus()


    std::shared_ptr<ThreadMonitorWrapper> canTelemetryThreadMonitorWrapper =
        std::make_shared<ThreadMonitorWrapper>(ThreadMonitorFactory());

    std::unique_ptr<ThreadMonitorManager> threadMonitorManagerInstance =
        std::make_unique<ThreadMonitorManager>(canTelemetryThreadMonitorWrapper);

  /*
   * Use std::ref for refs b/c:
   *    - https://stackoverflow.com/questions/5116756/difference-between-pointer-and-reference-as-thread-parameter
   */

  //Core Control Loop Thread
    TelemetryManager controlTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    controlTelemManager.setPodState(psBooting, (std::string)"Booting..."); // Set Pod state to booting
    std::thread coreControlThread(coreControlLoopThread, controlTelemManager, std::ref(threadMonitorManagerInstance) );
//    std::thread coreControlThreadX(coreControlLoopThread, controlTelemManager, );

    //  CAN Thread
    TelemetryManager canTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    //std::thread canThread(canNetworkThread, canTelemManager, std::ref(canTelemetryThreadMonitorWrapper));
//
    try {
//      canThread.join();
      coreControlThread.join();
    } catch (const std::exception& e)
    {
      LOG(ERROR) << "... what?";
    }

    //==================================================================================
    //==================================================================================
    //==================================================================================


    /*
    //Navigation Thread
    TelemetryManager navTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread navThread(NavigationThread, navTelemManager);


    //TelemetryManager Internal Network Thread
    TelemetryManager pinTelemManager = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread pinThread(udpTelemetryThread, pinTelemManager);

    // Controls Interface Connection Thread
    TelemetryManager pCommanderThread = TelemetryManager(&sPodValues, &sPodNetworkValues);
    std::thread controlsInterfaceThread(commanderThread, pCommanderThread);

	  coreControlThread.join();
    canThread.join();
    pinThread.join();
    controlsInterfaceThread.join();
   */


  return 0;
}
