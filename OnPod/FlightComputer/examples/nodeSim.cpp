///   This file was a temp node simulator and Protobuf playground. Now it holds old protobuf examples and useful functions.


#include <Client.h>
#include "../src/nodeSim.h"
#include "ProtoBuffer/NodeTelem.pb.h"
#include "EasyLogger/easylogging++.h"

#include <chrono>
#include <cstdlib>
#include <thread>

using namespace fc;

void sendPacket(brakeNodeData pNodeUpdate, clientSocketConfig *cscSocket) {
    std::string sPayload = "";
    int32_t iProtoPacketSize = pNodeUpdate.ByteSizeLong();
    pNodeUpdate.SerializeToString(&sPayload);
    void *vBuffer = malloc(iProtoPacketSize);
    pNodeUpdate.SerializeToArray(vBuffer, iProtoPacketSize);
    sendDataUdp(cscSocket, vBuffer, (int32_t) iProtoPacketSize);
}

void runNodeSimulator(clientSocketConfig *cscSocket) {
    el::Helpers::setThreadName("NodeSimulator");
    brakeNodeData::breakNodeState StateSequence[] = {brakeNodeData::bnsBooting, brakeNodeData::bnsStandby,
                                                     brakeNodeData::bnsArming, brakeNodeData::bnsArmed};


    for (int iIndex = 0; iIndex < 2; iIndex++) {
        brakeNodeData pNodeUpdate;
        pNodeUpdate.set_id(1);
        pNodeUpdate.set_state(brakeNodeData::bnsVenting);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    killConfigSocket(cscSocket);
    return;
}


void parseBrakeNodeProto(Pod *Pod, char cBuffer, int iRecievedPacketSize) {
    fc::brakeNodeData pNodeUpdate;
    bool bProtoPacketParsed = pNodeUpdate.ParseFromArray(&cBuffer, iRecievedPacketSize);
    if (bProtoPacketParsed) {
        LOG(INFO) << "Packet Recieved";
        parseBreakNodePacket(pNodeUpdate, *Pod);
    } else {
        LOG(ERROR) << "Error Parsing Protobuf packet";
    }
}

