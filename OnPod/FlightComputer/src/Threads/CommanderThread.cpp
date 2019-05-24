#include <cerrno>

#include "NetworkHelpers.h"
#include "Common.h"
#include "Heartbeat.h"

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

void parseProtoCommand(PodCommand podCommand, Pod *Pod) {
    if (podCommand.has_controlsinterfacestate()) {
        Pod->setControlsInterfaceState(podCommand.controlsinterfacestate());
    }
    if (podCommand.has_automaticstatetransitions()) {
        Pod->setAutomaticTransitions(podCommand.automaticstatetransitions());
    }
    if (podCommand.has_manualbrakenodestate()) {
       // LOG(INFO) << podCommand.manualbrakenodestate();
        Pod->setManualBrakeNodeState(podCommand.manualbrakenodestate());
    }
    if (podCommand.has_manuallvdcnodestate()) {
        Pod->setManualLvdcNodeState(podCommand.manuallvdcnodestate());
    }
    if (podCommand.has_manualpodstate()) {
        Pod->setManualPodState(podCommand.manualpodstate());
    }
    return;
}

int32_t unserializeProtoMessage(Pod *Pod, char buffer[], int32_t messageSize) {
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



int32_t commanderThread(Pod Pod) {
    //Logging
    el::Helpers::setThreadName("Commander Thread");
    LOG(INFO) << "Starting Commander Thread";

    //Sockets and buffers
    int32_t connectionSock, operationStatus;
    ssize_t messageSize;
    int32_t serverSock = createCommanderServerSocket(Pod.sPodNetworkValues->iCommanderPortNumber);
    char buffer[256] = {0};
    if (serverSock < 0) {
        // Restart thread?
        LOG(INFO) << std::string("ERROR initializing commader thread : ") + std::strerror(errno);
        return 0;
    }

    //Watchdog
    Heartbeat pulse = Heartbeat(Pod.sPodNetworkValues->iCommaderTimeoutMili);

    //pod state != shutdown
    while (Pod.telemetry->podState->getStateValue() != psShutdown) {

        /* Accepted connection gets put iNewSockfd,
        * thread will hang here until a connection is recieved.
        */
        connectionSock = accept(serverSock, nullptr, nullptr);
        if (connectionSock < 0) {
            LOG(INFO) << (std::string)"ERROR on acception Commander connection" + std::strerror(errno);
        }
        operationStatus = fcntl(connectionSock, F_SETFL, fcntl(connectionSock, F_GETFL, 0) | O_NONBLOCK);
        if(operationStatus == -1){
            //todo throw error and string errno
        }

        LOG(INFO) << "Controls Interface Connected";
        pulse.feed();
        while (Pod.telemetry->podState->getStateValue() != psShutdown) {
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




