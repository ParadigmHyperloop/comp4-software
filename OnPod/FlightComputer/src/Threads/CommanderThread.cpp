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

void parseOverrides(PodCommand podCommand, TelemetryManager *Pod) {
    Pod->sendUpdate("Parsing Overrides");
    // This is gross... dont look! ;)
    int32_t flag;
    for (int i = 0; i < 5; ++i) {
        if(podCommand.sensoroverrideconfiguration(i)){
            Pod->telemetry->nodeSensorFlags[i] = 2;
        }
    }
    if(podCommand.sensoroverrideconfiguration(5)){
        Pod->setConnectionFlag(2,2);
        for(auto &flag : Pod->telemetry->bmsSensorFlags){
            flag = 2;
        }
    }
    if(podCommand.sensoroverrideconfiguration(6)){
        Pod->setInverterHeartbeat(2);
        for(auto &flag : Pod->telemetry->inverterSensorFlags){
            flag = 2;
        }
    }
    if(podCommand.sensoroverrideconfiguration(7)){
        Pod->setConnectionFlag(2,BRAKE_NODE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(8)){
        Pod->setConnectionFlag(2,LVDC_NODE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(9)){
        Pod->setConnectionFlag(2,ENCLOSURE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(10)){
        Pod->setNodeSensorFlag(2,ENCLOSURE_PRESSURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(11)){
        Pod->setNodeSensorFlag(2,ENCLOSURE_TEMPERATURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(12)){
        Pod->setNodeSensorFlag(2,COOLING_PRESSURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(13)){
        Pod->setNodeSensorFlag(2,COOLING_TEMPERATURE_INDEX);
    }
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
        }
        Pod->setManualPodState(podCommand.manualpodstate());
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
                if (errno == 11) //Erno 11 means no message available on non blocking socket
                {
                    if (pulse.expired()) {
                        LOG(INFO) << "ERROR: Controls Interface Connection Timeout";
                        break;
                    }
                } else {
                    LOG(INFO) << "ERROR: Receveiving message : " << errno;
                }
            }
            if (messageSize == 0) {
                LOG(INFO) << "Controls Interface Connection Closed";
                break;
            } else if (messageSize > 0) {
                pulse.feed();
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
                        LOG(INFO) << "Controls Interface Connection Closed";
                    } else {
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




