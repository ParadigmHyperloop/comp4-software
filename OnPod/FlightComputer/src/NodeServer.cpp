#include "Pod.h"
#include "EasyLogger/easylogging++.h"
#include "Network.h"
#include "UdpConnection.h"



NodeConnection getBrakeNodeConnection(Pod Pod)
{
    BrakeNodeConnection BrakeNode = BrakeNodeConnection(Pod);

    try {
        BrakeNode.configure(Pod.sPodNetworkValues->cNodeIpAddrs[0], Pod.sPodNetworkValues->iBrakeNodePort,
                            Pod.sPodNetworkValues->iBrakeNodeServerPortNumber, Pod.sPodNetworkValues->iNodeTimeoutMili,
                            Pod.sPodNetworkValues->iNodeClientSocket);
    }
    catch (std::runtime_error &e) {
        throw e;
    }

    return  BrakeNode;
}

NodeConnection getRearNodeConnection(Pod Pod)
{
    BrakeNodeConnection BrakeNode = BrakeNodeConnection(Pod);
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

    std::vector<NodeConnection> nodes;
    if(Pod.sPodNetworkValues->iActiveNodes[0])
    {
        try {
            nodes.push_back(getBrakeNodeConnection(Pod));
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
            node.giveUpdate();
            node.getUpdate();
        }

    }

    for(auto&& node: nodes )
    {
        node.closeConnection();
    }
    close(Pod.sPodNetworkValues->iNodeClientSocket);

    return 1;
}

#pragma clang diagnostic pop


