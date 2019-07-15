#include "Common.h"
#include "UdpTelemetryThread.h"

UdpConnection *getBrakeNodeConnection(TelemetryManager Pod) {
    auto BrakeNode = new BrakeNodeConnection(Pod);
    try {
        BrakeNode->configureClient(Pod.sPodNetworkValues->cNodeIpAddrs[0], Pod.sPodNetworkValues->nodePort,
                                   Pod.sPodNetworkValues->nodeClientSocket, Pod.sPodNetworkValues->brakeNodeUpdateFreq);
        BrakeNode->configureServer(Pod.sPodNetworkValues->brakeNodeServerPortNumber,
                                   Pod.sPodNetworkValues->nodeTimeoutMili);
        BrakeNode->setRecvBufferSize(100); // Small recv buffer keeps parsed data fresh.
    }
    catch (std::runtime_error &e) {
        throw e;
    }
    return BrakeNode;
}

UdpConnection *getEnclosureNodeConnection(TelemetryManager Pod) {
    auto enclosureNode = new EnclosureNodeConnection(Pod);
    try {
        enclosureNode->configureServer(Pod.sPodNetworkValues->enclosureNodeServerNumber,
                                   Pod.sPodNetworkValues->nodeTimeoutMili);
        enclosureNode->setRecvBufferSize(100); // Small recv buffer keeps parsed data fresh.
    }
    catch (std::runtime_error &e) {
        throw e;
    }
    return enclosureNode;
}

UdpConnection *getLvdcNodeConnection(TelemetryManager Pod) {
    auto lvdcNode = new LvdcNodeConnection(Pod);
    try {
        lvdcNode->configureServer(Pod.sPodNetworkValues->lvdcNodeServerNumber,
                                       Pod.sPodNetworkValues->nodeTimeoutMili);
        lvdcNode->setRecvBufferSize(100); // Small recv buffer keeps parsed data fresh.
    }
    catch (std::runtime_error &e) {
        throw e;
    }
    return lvdcNode;
}



UdpConnection *getPdsConnection(TelemetryManager Pod){
    auto connection = new PdsConnection(Pod);
    connection->configureClient(Pod.sPodNetworkValues->strPdsIpAddr, Pod.sPodNetworkValues->pdsTelemeteryPort,
                                    Pod.sPodNetworkValues->nodeClientSocket, Pod.sPodNetworkValues->pdsUpdateFreq);
    return connection;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 *Wait on socket, parse the received message into a protobuf and hand it off.
 */
int32_t udpTelemetryThread(TelemetryManager Pod) {

  TIMED_FUNC(timeObj);
    //Logging
    el::Helpers::setThreadName("UDP");
    LOG(INFO) << "Starting TelemetryManager Internal Network Thread";

    try {
        Pod.sPodNetworkValues->nodeClientSocket = createUdpClientSocket();
    }
    catch (std::runtime_error &e) {
        LOG(INFO) << e.what();
        return -1;
    }

    std::vector<UdpConnection *> nodes; // Vector containing all Node Connections
    try {
        UdpConnection *brakeNode = getBrakeNodeConnection(Pod);
        nodes.push_back(brakeNode);
        UdpConnection* paradigmDataShuffle = getPdsConnection(Pod);
        nodes.push_back(paradigmDataShuffle);
        UdpConnection* enclosureNode = getEnclosureNodeConnection(Pod);
        nodes.push_back(enclosureNode);
        UdpConnection* lvdcNode = getLvdcNodeConnection(Pod);
        nodes.push_back(lvdcNode);
    }
    catch (std::runtime_error &e) {
        LOG(INFO) << e.what();
    }

    while (Pod.getPodStateValue() != psShutdown) {

      TIMED_SCOPE(timeObj, "UdpTelemetryThread");
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
    }
    for (auto &&node: nodes) {
        node->closeConnection();
        delete node;
    }
    close(Pod.sPodNetworkValues->nodeClientSocket);
    return 1;
}

#pragma clang diagnostic pop