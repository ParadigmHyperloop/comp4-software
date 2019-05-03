#include "FlightComputer/Pod.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>
#include <FlightComputer/UdpConnection.h>



UdpConnection* getBrakeNodeConnection(Pod Pod)
{
    auto BrakeNode = new BrakeNodeConnection(Pod);

    try {
        BrakeNode->configureClient(Pod.sPodNetworkValues->cNodeIpAddrs[0], Pod.sPodNetworkValues->iBrakeNodePort, Pod.sPodNetworkValues->iNodeClientSocket);
        //TODO throw error if this isnt set
        BrakeNode->configureServer(Pod.sPodNetworkValues->iBrakeNodeServerPortNumber, Pod.sPodNetworkValues->iNodeTimeoutMili);
        BrakeNode->setRecvBufferSize(20); // Small recv buffer keeps parsed data fresh.
    }
    catch (std::runtime_error &e) {
        throw e;
    }

    return  BrakeNode;
}

UdpConnection* getRearNodeConnection(Pod Pod)
{
    auto BrakeNode = new BrakeNodeConnection(Pod);
    return  BrakeNode;
}



#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

/**
 *Wait on socket, parse the received message into a protobuf and hand it off.
 */
int32_t podInternalNetworkThread(Pod Pod) {

    try {
        Pod.sPodNetworkValues->iNodeClientSocket = createUdpClientSocket();
    }
    catch (std::runtime_error &e) {
        LOG(INFO) << e.what();
        return -1;
    }

    std::vector<UdpConnection*> nodes;
    if(Pod.sPodNetworkValues->iActiveNodes[0])
    {
        try {
            UdpConnection* brakeNode = getBrakeNodeConnection(Pod);
            nodes.push_back(brakeNode);
        }
        catch (std::runtime_error &e) {
            LOG(INFO)<< e.what();
        }
    }

    if(Pod.sPodNetworkValues->iActiveNodes[1])
    {

    }


    while (Pod.sPodValues->ePodState != psShutdown ) {
        // Give and get update for each node
        for(auto&& node: nodes )
        {
            try {
                node->giveUpdate();
                node->getUpdate();
            }
            catch (std::runtime_error &e) {
                LOG(INFO)<< e.what();
            }
        }

    }

    for(auto&& node: nodes )
    {
        node->closeConnection();
        delete node;
    }
    close(Pod.sPodNetworkValues->iNodeClientSocket);

    return 1;
}

#pragma clang diagnostic pop


