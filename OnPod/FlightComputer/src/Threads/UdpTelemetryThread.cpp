#include "Common.h"
#include "UdpTelemetryThread.h"





UdpConnection *getBrakeNodeConnection(Pod Pod) {
    auto BrakeNode = new BrakeNodeConnection(Pod);
    try {
        BrakeNode->configureClient(Pod.sPodNetworkValues->cNodeIpAddrs[0], Pod.sPodNetworkValues->iBrakeNodePort,
                                   Pod.sPodNetworkValues->iNodeClientSocket);
        BrakeNode->configureServer(Pod.sPodNetworkValues->iBrakeNodeServerPortNumber,
                                   Pod.sPodNetworkValues->iNodeTimeoutMili);
        BrakeNode->setRecvBufferSize(100); // Small recv buffer keeps parsed data fresh.
    }
    catch (std::runtime_error &e) {
        throw e;
    }

    return BrakeNode;
}

UdpConnection *getRearNodeConnection(Pod Pod) {
    auto BrakeNode = new BrakeNodeConnection(Pod);
    return BrakeNode;
}

UdpConnection *getPdsConnection(Pod Pod){
    auto connection = new PdsConnection(Pod);
    connection->configureClient(Pod.sPodNetworkValues->strPdsIpAddr, Pod.sPodNetworkValues->iPdsTelemeteryPort,
                                    Pod.sPodNetworkValues->iNodeClientSocket);
    return connection;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 *Wait on socket, parse the received message into a protobuf and hand it off.
 */
int32_t udpTelemetryThread(Pod Pod) {

    //Logging
    el::Helpers::setThreadName("Pod Internal Network Thread");
    LOG(INFO) << "Starting Pod Internal Network Thread";

    try {
        Pod.sPodNetworkValues->iNodeClientSocket = createUdpClientSocket();
    }
    catch (std::runtime_error &e) {
        LOG(INFO) << e.what();
        return -1;
    }

    UdpConnection* paradigmDataShuffle = getPdsConnection(Pod);

    std::vector<UdpConnection *> nodes; // Vector containing all Node Connections
    if (Pod.sPodNetworkValues->iActiveNodes[0]) //Check if brake node is active
    {
        try {
            UdpConnection *brakeNode = getBrakeNodeConnection(Pod);
            nodes.push_back(brakeNode);
        }
        catch (std::runtime_error &e) {
            LOG(INFO) << e.what();
        }
    }
    if (Pod.sPodNetworkValues->iActiveNodes[1]) {

    }
    while (Pod.telemetry->podState->getStateValue() != psShutdown) {
        // Give and get update for each node
        for (auto &&node: nodes) {
            try {
                node->giveUpdate();
                node->getUpdate();
            }
            catch (std::runtime_error &e) {
                LOG(INFO) << e.what();
            }
        }
        paradigmDataShuffle->giveUpdate(); //Send telemetry packet to PDS
        std::this_thread::sleep_for(std::chrono::milliseconds(50));}
    for (auto &&node: nodes) {
        node->closeConnection();
        delete node;
    }
    close(Pod.sPodNetworkValues->iNodeClientSocket);
    delete paradigmDataShuffle;
    return 1;
}

#pragma clang diagnostic pop


