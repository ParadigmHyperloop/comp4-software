#include "FlightComputer/Pod.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>
#include <FlightComputer/NodeConnection.h>


void parseBrakeNodeUpdate(Pod *Pod, char cUpdate[]) {
    BrakeNodeStates bnsBrakeNodeStates[9] = {bnsBooting,
                                             bnsStandby,
                                             bnsArming,
                                             bnsArmed,
                                             bnsFlight,
                                             bnsBraking,
                                             bnsVenting,
                                             bnsRetrieval,
                                             bnsError};
    std::string sNodeState(cUpdate);
    int32_t iStateNumber = std::stoi(sNodeState);
    BrakeNodeStates eBrakeNodeState = bnsBrakeNodeStates[iStateNumber];
    Pod->setBrakeNodeState(eBrakeNodeState);
    return;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 *Wait on socket, parse the received message into a protobuf and hand it off.
 */
int32_t podInternalNetworkThread(Pod Pod) {
    int32_t iClientSocket = -1;
    iClientSocket = createUdpClientSocket();

    if (iClientSocket < 0) {
        LOG(INFO) << std::string("Error creating client socket for Node Server: ") + std::strerror(errno);
        return -1;
    }

    std::string sNodeIp = "127.0.0.1";
    BrakeNodeConnection BrakeNode = BrakeNodeConnection(Pod);

    try {
        BrakeNode.configure(Pod.sPodNetworkValues->cNodeIpAddrs[0], Pod.sPodNetworkValues->iBrakeNodePort,
                            Pod.sPodNetworkValues->iBrakeNodeServerPortNumber, Pod.sPodNetworkValues->iNodeTimeoutMili,
                            iClientSocket);
    }
    catch (std::runtime_error &e) {
        LOG(INFO) << e.what();
    }

    while (1) {
        BrakeNode.giveUpdate();
        BrakeNode.getUpdate();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    //close sockets

}

#pragma clang diagnostic pop


