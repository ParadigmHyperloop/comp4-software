#include "FlightComputer/UdpConnection.h"


UdpConnection::UdpConnection(Pod){
    this->pod = pod;
    struct sockaddr_in sAddr = {0};
    this->sDestAddr = sAddr;
};

void UdpConnection::setRecvBufferSize(int32_t iSize) {
    int32_t s = setsockopt(this->iInboundSocket, SOL_SOCKET, SO_RCVBUF, &iSize, sizeof(iSize));
}


void UdpConnection::configureClient(const std::string &strIp, int32_t iDestPort, int32_t iOutboundSocket) {
    struct sockaddr_in sAddr = {0};
    this->iOutboundSocket = iOutboundSocket;
    this->sDestAddr = sAddr;
    this->sDestAddr = createGenericNodeAddr();
    this->sDestAddr.sin_port = htons(iDestPort);
    this->sDestAddr.sin_addr.s_addr = inet_addr(strIp.c_str());
}


void UdpConnection::configureServer(int32_t iServerPort, int32_t iTimeoutMilis) {
    this->pulse = Heartbeat(iTimeoutMilis);
    this->iServerPort = iServerPort;
    try {
        this->createServerSocket();
    }
    catch (std::runtime_error &e) {
        throw e;
    }
}


void UdpConnection::getUpdate() {
    if (this->iInboundSocket == -1) {
        std::string strError = "getUpdate() : Server socket invalid";
        throw std::runtime_error(strError);
    }
    char cBuffer[200] = {0};
    bzero(&cBuffer, sizeof cBuffer);
    ssize_t iReceivedPacketSize = recvfrom(this->iInboundSocket, cBuffer, 200, 0, nullptr, nullptr);
    if (iReceivedPacketSize != -1) {
        LOG(INFO) << iReceivedPacketSize << " Bytes received on " << this->strConnectionName << cBuffer;
        try {
            this->parseUpdate(cBuffer, (int32_t) iReceivedPacketSize);
        }
        catch (const std::invalid_argument &e) {
            std::string strError = "getUpdate() : " + std::string(e.what());
            throw std::runtime_error(strError);
        }
        this->pulse.feed();
        this->setConnectionStatus(true);
    } else {
        if (this->pulse.expired()) {
            this->setConnectionStatus(false);
        }
    }
}


void UdpConnection::giveUpdate() {
    google::protobuf::Message *pPacket;
    ssize_t iPayloadSize;
    pPacket = this->getProtoUpdateMessage();
    iPayloadSize = pPacket->ByteSizeLong();
    unsigned char cPayload[iPayloadSize];

    if (!pPacket->SerializeToArray(cPayload, static_cast<int>(iPayloadSize))) {
        std::string strError = std::string("Error Creating Proto packet on  ") + this->strConnectionName;
        delete pPacket;
        throw std::runtime_error(strError);
    }
    ssize_t sent = sendto(this->iOutboundSocket, cPayload, iPayloadSize, 0,
                          (struct sockaddr *) &this->sDestAddr, sizeof(this->sDestAddr));
    if (sent == -1) {
        std::string strError = std::string("Error Sending ") + this->strConnectionName + std::strerror(errno);
        delete pPacket;
        throw std::runtime_error(strError);
    }
    delete pPacket;
}

void UdpConnection::closeConnection() {
    close(this->iInboundSocket);
    close(this->iOutboundSocket);
}


virtual google::protobuf::Message* UdpConnection::getProtoUpdateMessage() {
    auto protoMessage = new defaultFcToNode();
    protoMessage->set_podstate(psArmed);
    return protoMessage;
};

bool UdpConnection::createServerSocket() {
    int32_t iSocketfd;
    struct sockaddr_in SocketAddrStruct = {0};

    iSocketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (iSocketfd < 0) {
        std::string sError = std::string(" Creating Server socket: ") + std::strerror(errno);
        throw std::runtime_error(sError);
    }

    int flags = fcntl(iSocketfd, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(iSocketfd, F_SETFL, flags);

    SocketAddrStruct.sin_family = AF_INET;
    SocketAddrStruct.sin_port = htons(this->iServerPort);
    SocketAddrStruct.sin_addr.s_addr = INADDR_ANY;
    int32_t iBind = bind(iSocketfd, (struct sockaddr *) &SocketAddrStruct, sizeof(SocketAddrStruct));
    if (iBind < 0) {
        std::string sError = std::string("Binding Server Socket: ") + std::strerror(errno);
        throw std::runtime_error(sError);
    }
    this->iInboundSocket = iSocketfd;
    return true;
}


PdsConnection::PdsConnection(Pod pod) : UdpConnection(pod) {
    this->strConnectionName = "Controls Interface Data : ";
};

//TODO can we do this without putting the proto on the heap?
google::protobuf::Message* PdsConnection::getProtoUpdateMessage(){
    auto protoMessage = new telemetry();
    protoMessage->set_podstate(pod.sPodValues->ePodState);
    protoMessage->set_ilowpressure1(pod.sPodValues->iLowPressure1);
    protoMessage->set_ihighpressure(pod.sPodValues->iHighPressure);
    protoMessage->set_bsolenoid1(pod.sPodValues->bSolenoid1);
    protoMessage->set_bsolenoid2(pod.sPodValues->bSolenoid2);
    protoMessage->set_pressurevesseltemperature(pod.sPodValues->iPressureVesselTemperature);
    protoMessage->set_railtemperature(pod.sPodValues->iRailTemperature);
    protoMessage->set_ihvbatterypackvoltage(pod.sPodValues->fHvBatteryPackVoltage);
    protoMessage->set_ihvbatterypackcurrent(pod.sPodValues->iHvBatteryPackCurrent);
    protoMessage->set_ihvbatterypackmaxcellvoltage(pod.sPodValues->iHvBatteryPackMaxCellVoltage);
    protoMessage->set_ihvbatterypackminimumcellvoltage(pod.sPodValues->iHvBatteryPackMinimumCellVoltage);
    return protoMessage;
}




BrakeNodeConnection::BrakeNodeConnection(Pod pod) : UdpConnection(pod) {
    this->strConnectionName = "Brake Node : ";
};

void BrakeNodeConnection::setConnectionStatus(bool bStatus){
    this->pod.sPodValues->cConnectionsArray[0] = bStatus;
}

google::protobuf::Message* BrakeNodeConnection::getProtoUpdateMessage() {
    auto protoMessage = new FcToBrakeNode();
    protoMessage->set_podstate(psArmed);
    protoMessage->set_manualnodestate(bnsArmed);
    return protoMessage;
}

bool BrakeNodeConnection::parseUpdate(char cBuffer[], int32_t iMessageSize){
    DtsNodeToFc protoMessage = DtsNodeToFc();
    if (!protoMessage.ParseFromArray(cBuffer, iMessageSize)) {
        std::string strError = "Failed to parse Update from Brake Node";
        throw std::invalid_argument(strError);
    }
    this->pod.sPodValues->bSolenoid1 = protoMessage.brakesolenoidstate();
    this->pod.sPodValues->bSolenoid2 = protoMessage.ventsolenoidstate();
    this->pod.sPodValues->iRailTemperature = protoMessage.rotortemperature();
    this->pod.sPodValues->iPressureVesselTemperature = protoMessage.pneumatictemperature();
    this->pod.sPodValues->iHighPressure = protoMessage.tankpressure();
    this->pod.sPodValues->iLowPressure1 = protoMessage.brakepressure();
    return true;
}



