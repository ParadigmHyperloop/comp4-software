#include <cerrno>

#include "NetworkHelpers.h"
#include "Common.h"
#include "Heartbeat.h"
#include "TelemetryManager.h"
#include "Constants/SensorConfig.h"


// Get manual state change commands. Get Estop command

int32_t createCommanderServerSocket(int32_t serverPortNumber) {
    int32_t serverSock;
    int8_t operationStatus;
    struct sockaddr_in serverSockAddr;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock < 0) {
        LOG(INFO) << (std::string)"ERROR opening commander socket" + std::strerror(errno);;
        return -1;
    }
    bzero((char *) &serverSockAddr, sizeof(serverSockAddr));
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = INADDR_ANY;
    serverSockAddr.sin_port = htons(serverPortNumber);
    operationStatus = bind(serverSock, (struct sockaddr *) &serverSockAddr, sizeof(serverSockAddr));
    if (operationStatus < 0) {
        LOG(INFO) << (std::string)"ERROR binding commander socket : " + std::strerror(errno);
        return -1;
    }
    //Queue 3 requests before rejecting
    listen(serverSock, 3);
    return serverSock;
}

int32_t convertFlag(bool state){
    if(state){
        return 2;
    } else{
        return 0;
    }
}

void parseOverrides(PodCommand podCommand, TelemetryManager *Pod) {
    Pod->sendUpdate("Parsing Overrides");
    // This is gross... dont look! ;)
    int32_t flag;
    for (int i = 0; i <= 5; ++i) {
        Pod->telemetry->nodeSensorFlags[i] = convertFlag(podCommand.sensoroverrideconfiguration(i));
    }

    Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(6)), CONNECTION_FLAGS::BMS_HEARTBEAT_INDEX);
    for(auto &flag : Pod->telemetry->bmsSensorFlags){
        flag = convertFlag(podCommand.sensoroverrideconfiguration(6));
    }

    int8_t mode = convertFlag(podCommand.sensoroverrideconfiguration(7));
    Pod->setInverterHeartbeat(mode);
    for(auto &flag : Pod->telemetry->inverterSensorFlags){
        flag = mode;
    }


    Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(8)), CONNECTION_FLAGS::BRAKE_NODE_HEARTBEAT_INDEX);

    Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(9)), CONNECTION_FLAGS::LVDC_NODE_HEARTBEAT_INDEX);

    Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(10)), CONNECTION_FLAGS::ENCLOSURE_HEARTBEAT_INDEX);

    Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(11)), NODE_FLAGS::ENCLOSURE_PRESSURE_INDEX);
    Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(12)), NODE_FLAGS::ENCLOSURE_TEMPERATURE_INDEX);

    Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(13)), NODE_FLAGS::COOLING_PRESSURE_INDEX);

    Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(14)), NODE_FLAGS::COOLING_TEMPERATURE_INDEX);

    Pod->telemetry->checkNodeStates = !podCommand.sensoroverrideconfiguration(15);

    Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(16)), CONNECTION_FLAGS::NAVIGATION_HEARTBEAT_INDEX);

}

void parseProtoCommand(PodCommand podCommand, TelemetryManager *Pod) {
    if(!podCommand.has_hascommand()){
        return;
    }
    if (podCommand.has_controlsinterfacestate()) {
        Pod->setControlsInterfaceState(podCommand.controlsinterfacestate());
    }
    if(podCommand.has_maxflighttime()){

        Pod->telemetry->maxFlightTime = podCommand.maxflighttime();
        Pod->telemetry->motorTorque = podCommand.motortorque();
        Pod->telemetry->flightDistance = podCommand.flightdistance();
        Pod->telemetry->accelerationTime = podCommand.accelerationtime();
        Pod->telemetry->startTorque = podCommand.starttorque();
        Pod->telemetry->taxi = podCommand.taxi();
        Pod->telemetry->expectedTubePressure = podCommand.expectedtubepressure();
        Pod->telemetry->maxVelocity = podCommand.maxvelocity();
        Pod->telemetry->maxRPM = (podCommand.maxvelocity() * 60)/podCommand.maxvelocity();
        Pod->telemetry->brakeDistance = podCommand.brakingdistance();
        if(!podCommand.taxi()) {
            Pod->telemetry->maxStripCount = ( (Pod->telemetry->flightDistance - Pod->telemetry->brakeDistance) /
                                  GENERAL_CONSTANTS::STRIP_DISTANCE ) + 1;
        }
        else{
            Pod->telemetry->maxStripCount = GENERAL_CONSTANTS::DEFAULT_MAX_STRIP_COUNT; 
        }
    }
    if (podCommand.has_manualbrakenodestate()){
        BrakeNodeStates state = podCommand.manualbrakenodestate();
        if(state == bnsSolenoidControl){
            std::fill(Pod->telemetry->manualSolenoidConfiguration.begin(), Pod->telemetry->manualSolenoidConfiguration.end(), false);
        }
        Pod->setManualBrakeNodeState(podCommand.manualbrakenodestate());
    }
    if (podCommand.has_manuallvdcnodestate()) {
        Pod->setManualLvdcNodeState(podCommand.manuallvdcnodestate());
    }
    if (podCommand.has_manualpodstate()) {
        if(podCommand.manualpodstate() == psNone){
            Pod->setAutomaticTransitions(true);
            Pod->setManualPodState(psStandby);
        } else{
            Pod->setManualPodState(podCommand.manualpodstate());
            Pod->setAutomaticTransitions(false);
        }
    }
    if (podCommand.solenoidconfiguration_size() >= 4){
        for(int i = 0 ; i < 4 ; i++){
            Pod->telemetry->manualSolenoidConfiguration[i] = podCommand.solenoidconfiguration(i);
        }
    }
    if (podCommand.sensoroverrideconfiguration_size() >= 1){
        parseOverrides(podCommand, Pod);
    }
}

int32_t unserializeProtoMessage(TelemetryManager *Pod, char buffer[], int32_t messageSize) {
    PodCommand pPodCommand;
    bool operationStatus;
    operationStatus = pPodCommand.ParseFromArray(buffer, messageSize);
    if (operationStatus) {
        parseProtoCommand(pPodCommand, Pod);
       // LOG(INFO) << "Command/HeartBeat Received";
        return operationStatus;
    } else {
        LOG(ERROR) << "Error Parsing Command";
        return operationStatus;
    }
}


int32_t commanderThread(TelemetryManager Pod) {
    //Logging
    el::Helpers::setThreadName("Commander Thread");
    LOG(INFO) << "Starting Commander Thread";

    //Sockets and buffers
    int32_t connectionSock, operationStatus;
    ssize_t messageSize;
    int32_t serverSock = createCommanderServerSocket(Pod.sPodNetworkValues->commanderPortNumber);
    char buffer[256] = {0};
    if (serverSock < 0) {
        // Restart thread?
        LOG(INFO) << std::string("ERROR initializing commader thread : ") + std::strerror(errno);
        return 0;
    }

    //Watchdog
    Heartbeat pulse = Heartbeat(Pod.sPodNetworkValues->commaderTimeoutMili);

    //pod state != shutdown
    while (Pod.getPodStateValue() != psShutdown) {

        /* Accepted connection gets put iNewSockfd,
        * thread will hang here until a connection is recieved.
        */
        connectionSock = accept(serverSock, nullptr, nullptr);
        if (connectionSock < 0) {
            LOG(INFO) << (std::string)"ERROR on accept() on Commander socket : " + std::strerror(errno);
        }
        operationStatus = fcntl(connectionSock, F_SETFL, fcntl(connectionSock, F_GETFL, 0) | O_NONBLOCK);
        if(operationStatus == -1){
            LOG(INFO) << (std::string)"ERROR on making commander socket non-blocking : " + std::strerror(errno);
        }

        LOG(INFO) << "Controls Interface Connected";
        pulse.feed();
        while (Pod.getPodStateValue() != psShutdown) {
            messageSize = read(connectionSock, buffer, 255);
            if (messageSize < 0) {
                if (pulse.expired()) {
                    Pod.setConnectionFlag(0, CONNECTION_FLAGS::INTERFACE_HEARTBEAT_INDEX);
                    LOG(INFO) << "ERROR: Controls Interface Connection Timeout";
                    break;
                }
                if (errno == 11) //Erno 11 means no message available on non blocking socket
                {
                } else {
                    LOG(INFO) << "ERROR: Receveiving message : " << errno;
                }
            }
            if (messageSize == 0) {
                LOG(INFO) << "Controls Interface Connection Closed";
                Pod.setConnectionFlag(0, CONNECTION_FLAGS::INTERFACE_HEARTBEAT_INDEX);
                break;
            } else if (messageSize > 0) {
                pulse.feed();
                Pod.setConnectionFlag(1, CONNECTION_FLAGS::INTERFACE_HEARTBEAT_INDEX);
                operationStatus = unserializeProtoMessage(&Pod, buffer, messageSize);
                memset(buffer, 0, sizeof buffer);
                if (operationStatus) {
                    //Return 1 to confirm reception
                    messageSize = write(connectionSock, "1", 1);
                } else {
                    //Return 0 indicating bad message
                    messageSize = write(connectionSock, "0", 1);
                }
                if (messageSize < 0) {
                    if (errno == 104) {
                        Pod.setConnectionFlag(0, CONNECTION_FLAGS::INTERFACE_HEARTBEAT_INDEX);
                        LOG(INFO) << "Controls Interface Connection Closed";
                    } else {
                        Pod.setConnectionFlag(0, CONNECTION_FLAGS::INTERFACE_HEARTBEAT_INDEX);
                        LOG(INFO) << "ERROR writing to socket: " << errno;
                    }
                    break;
                }
            }
        }
        close(connectionSock);
    }
    close(serverSock);
}




