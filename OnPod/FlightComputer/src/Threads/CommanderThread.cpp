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

int8_t convertFlag(bool state){
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
    for (int i = 0; i < 5; ++i) {
        if(podCommand.sensoroverrideconfiguration(i)){
            Pod->telemetry->nodeSensorFlags[i] = convertFlag(podCommand.sensoroverrideconfiguration(i));
        }
    }
    if(podCommand.sensoroverrideconfiguration(5)){
        Pod->setConnectionFlag(2,BMS_HEARTBEAT_INDEX);
        for(auto &flag : Pod->telemetry->bmsSensorFlags){
            flag = convertFlag(flag);
        }
    }
    if(podCommand.sensoroverrideconfiguration(6)){
        int8_t mode = convertFlag(podCommand.sensoroverrideconfiguration(6));
        Pod->setInverterHeartbeat(mode);
        for(auto &flag : Pod->telemetry->inverterSensorFlags){
            flag = mode;
        }
    }
    if(podCommand.sensoroverrideconfiguration(7)){
        Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(7)),BRAKE_NODE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(8)){
        Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(8)),LVDC_NODE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(9)){
        Pod->setConnectionFlag(convertFlag(podCommand.sensoroverrideconfiguration(9)),ENCLOSURE_HEARTBEAT_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(10)){
        Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(10)),ENCLOSURE_PRESSURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(11)){
        Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(11)),ENCLOSURE_TEMPERATURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(12)){
        Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(12)),COOLING_PRESSURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(13)){
        Pod->setNodeSensorFlag(convertFlag(podCommand.sensoroverrideconfiguration(13)),COOLING_TEMPERATURE_INDEX);
    }
    if(podCommand.sensoroverrideconfiguration(14)){
       Pod->telemetry->checkNodeStates = (podCommand.sensoroverrideconfiguration(14)) ? false : true;
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
                if (pulse.expired()) {
                    Pod.setConnectionFlag(0, INTERFACE_HEARTBEAT_INDEX);
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




