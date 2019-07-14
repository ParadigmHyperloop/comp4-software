#include "UdpConnection.h"
#include "../Constants/SensorConfig.h"

UdpConnection::UdpConnection(TelemetryManager pod){
    this->pod = pod;
    struct sockaddr_in socketAddress = {0};
    this->_destSockAddr = socketAddress;
};

void UdpConnection::setRecvBufferSize(int32_t bufferSize) {
    int32_t status = setsockopt(this->_inboundSocket, SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize));
    if(status < 0){
        std::string error = std::string("Failed to set recv buffer size ") + std::strerror(errno);
        throw std::runtime_error(error);
    }
}

void UdpConnection::configureClient(const std::string &destIp, int32_t destPortNum, int32_t outboundSocket, int32_t update_freq) {
    struct sockaddr_in sAddr = {0};
    this->_outboundSocket = outboundSocket;
    this->_destSockAddr = sAddr;
    this->_destSockAddr = createGenericNodeAddr();
    this->_destSockAddr.sin_port = htons(destPortNum);
    this->_destSockAddr.sin_addr.s_addr = inet_addr(destIp.c_str());
    this->_update_freq = Heartbeat(update_freq);
}

void UdpConnection::configureServer(int32_t serverPort, int32_t connectionTimeoutMilis) {
    this->_pulse = Heartbeat(connectionTimeoutMilis);
    this->_serverPort = serverPort;
    try {
        this->_createServerSocket();
    }
    catch (std::runtime_error &e) {
        throw e;
    }
}

int32_t UdpConnection::getNewPacketId() {
    return this->_lastPacketSentId++;
}

int32_t UdpConnection::checkPacketId(int32_t id) {
    if(id < this->_lastPacketReceivedId){
        return false;
    }
    else{
        this->_lastPacketReceivedId = id;
        return true;
    }
}

void UdpConnection::getUpdate() {
    if (this->_inboundSocket == UNCONFIGURED) {
        return;
    }
    if (this->_inboundSocket == -1) {
        std::string error = this->_connectionName + "getUpdate() : Server socket invalid";
        throw std::runtime_error(error);
    }

    bool opStatus = false;

    char buffer[200] = {0};
    bzero(&buffer, sizeof buffer);
    ssize_t receivedPacketSize = recvfrom(this->_inboundSocket, buffer, 200, 0, nullptr, nullptr);
    if (receivedPacketSize != -1) {
        //LOG(INFO) << receivedPacketSize << " Bytes received on " << this->_connectionName << buffer;
        try {
            opStatus = this->parseUpdate(buffer, (int32_t) receivedPacketSize);
        }
        catch (const std::invalid_argument &e) {
            std::string error = "getUpdate() : " + std::string(e.what());
            this->pod.setConnectionFlag(false, this->getConnectionIndex());
            throw std::runtime_error(error);
        }
        if(opStatus){
            this->_pulse.feed();
        }
        this->pod.setConnectionFlag(true, this->getConnectionIndex());
    } else {
        if (this->_pulse.expired()) {
            this->pod.setConnectionFlag(false, this->getConnectionIndex());
        }
    }
}

void UdpConnection::giveUpdate() {
    if(!this->_update_freq.expired() || this->_outboundSocket == UNCONFIGURED){
        return;
    }
    this->_update_freq.feed();

    ssize_t payloadSize;
    std::unique_ptr<google::protobuf::Message> protoPacket(this->getProtoUpdateMessage());
    payloadSize = protoPacket->ByteSizeLong();
    unsigned char payload[payloadSize];

    if (!protoPacket->SerializeToArray(payload, static_cast<int>(payloadSize))) {
        std::string error = std::string("Error Creating Proto packet on  ") + this->_connectionName;
        throw std::runtime_error(error);
    }
    ssize_t sent = sendto(this->_outboundSocket, payload, payloadSize, 0,
                          (struct sockaddr *) &this->_destSockAddr, sizeof(this->_destSockAddr));
    if (sent == -1) {
        std::string strError = std::string("Error Sending ") + this->_connectionName + std::strerror(errno);
        throw std::runtime_error(strError);
    }
}

void UdpConnection::closeConnection() {
    close(this->_inboundSocket);
    close(this->_outboundSocket);
}

std::unique_ptr<google::protobuf::Message> UdpConnection::getProtoUpdateMessage() {
    std::unique_ptr<DefaultFcToNode> protoMessage (new DefaultFcToNode());
    protoMessage->set_podstate(psArmed);
    return protoMessage; //Shift ownership of unique ptr to caller
};

bool UdpConnection::_createServerSocket() {
    int32_t serverSocket;
    struct sockaddr_in SocketAddr = {0};

    serverSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (serverSocket < 0) {
        std::string error = std::string(" Creating Server socket: ") + std::strerror(errno);
        throw std::runtime_error(error);
    }

    int flags = fcntl(serverSocket, F_GETFL);
    flags |= O_NONBLOCK;
    fcntl(serverSocket, F_SETFL, flags);

    SocketAddr.sin_family = AF_INET;
    SocketAddr.sin_port = htons(this->_serverPort);
    SocketAddr.sin_addr.s_addr = INADDR_ANY;
    int32_t status = bind(serverSocket, (struct sockaddr *) &SocketAddr, sizeof(SocketAddr));
    if (status < 0) {
        std::string sError = this->_connectionName + std::string("Binding Server Socket: ") + std::strerror(errno);
        throw std::runtime_error(sError);
    }
    this->_inboundSocket = serverSocket;
    return true;
}

int32_t UdpConnection::getConnectionIndex(){
    return this->_connectionStatusIndex;
}



// *  ***************** PDS Connection *******************


PdsConnection::PdsConnection(TelemetryManager pod) : UdpConnection(pod) {
    this->_connectionName = "Controls Interface Data : ";
    this->_connectionStatusIndex = -1;
};

std::unique_ptr<google::protobuf::Message> PdsConnection::getProtoUpdateMessage() {
    std::unique_ptr<Telemetry> protoMessage (new Telemetry());
    protoMessage->set_podstate(pod.getPodStateValue());
    protoMessage->set_brakenodestate(pod.telemetry->receivedBrakeNodeState);
    protoMessage->set_lvdcnodestate(lvdcNone);


    // Brake Node
    protoMessage->set_lowpressure1(pod.telemetry->lowPressure1);
    protoMessage->set_lowpressure2(pod.telemetry->lowPressure2);
    protoMessage->set_lowpressure3(pod.telemetry->lowPressure3);
    protoMessage->set_lowpressure4(pod.telemetry->lowPressure4);
    protoMessage->set_highpressure(pod.telemetry->highPressure);
    protoMessage->set_coolanttemperature(pod.telemetry->coolingTemperature);
    protoMessage->set_solenoid1(pod.telemetry->solenoid1);
    protoMessage->set_solenoid2(pod.telemetry->solenoid2);
    protoMessage->set_solenoid3(pod.telemetry->solenoid3);
    protoMessage->set_solenoid4(pod.telemetry->solenoid4);
    protoMessage->set_pressurevesseltemperature(pod.telemetry->pressureVesselTemperature);

    // HV
    protoMessage->set_hvbatterypackvoltage(pod.telemetry->hvBatteryPackVoltage);
    protoMessage->set_hvbatterypackcurrent(pod.telemetry->hvBatteryPackCurrent);
    protoMessage->set_hvbatterypackmaxcellvoltage(pod.telemetry->hvBatteryPackMaxCellVoltage);
    protoMessage->set_hvbatterypackminimumcellvoltage(pod.telemetry->hvBatteryPackMinimumCellVoltage);

    // LV
    protoMessage->set_lv1batterypackcelltemperature(pod.telemetry->lv1BatteryPackCellTemperature);
    protoMessage->set_lv1batterypackstateofcharge(pod.telemetry->lv1BatteryPackStateOfCharge);
    protoMessage->set_lv1batterypackvoltage(pod.telemetry->lv1BatteryPackVoltage);
    protoMessage->set_lv2batterypackcelltemperature(pod.telemetry->lv2BatteryPackCellTemperature);
    protoMessage->set_lv2batterypackstateofcharge(pod.telemetry->lv2BatteryPackStateOfCharge);
    protoMessage->set_lv2batterypackvoltage(pod.telemetry->lv2BatteryPackVoltage);

    // Motor Controller
    protoMessage->set_maxigbttemperature(pod.telemetry->maxIgbtTemperature);
    protoMessage->set_gatedrivertemperature(pod.telemetry->gateDriverTemperature);
    protoMessage->set_invertercontrolboardtemperature(pod.telemetry->inverterControlBoardTemperature);
    protoMessage->set_motortemperature(pod.telemetry->motorTemperature);
    protoMessage->set_inverterbusvoltage(pod.telemetry->inverterBusVoltage);
    protoMessage->set_hvbatterypackmaxcelltemperature(pod.telemetry->hvBatteryPackMaxCellTemperature);
    protoMessage->set_hvbatterypackstateofcharge(pod.telemetry->hvBatteryPackStateOfCharge);
    protoMessage->set_inverterheartbeat(pod.telemetry->inverterHeartbeat);


    // Flight Profile
    protoMessage->set_maxflighttime(pod.telemetry->maxFlightTime);
    protoMessage->set_flightdistance(pod.telemetry->flightDistance);
    protoMessage->set_starttorque(pod.telemetry->startTorque);
    protoMessage->set_motortorque(pod.telemetry->motorTorque);
    protoMessage->set_accelerationtime(pod.telemetry->accelerationTime);
    protoMessage->set_taxi(pod.telemetry->taxi);

    // Navigation
    protoMessage->set_motorspeed(pod.telemetry->motorSpeed);
    protoMessage->set_podposition(pod.telemetry->podPosition);
    protoMessage->set_podvelocity(pod.telemetry->podVelocity);

    // Enclosure
    protoMessage->set_coolantpressure1(pod.telemetry->coolingLinePressure);
    protoMessage->set_enclosurepressure(pod.telemetry->enclosurePressure);
    protoMessage->set_enclosuretemperature(pod.telemetry->enclosureTemperature);

    if(pod.telemetry->updates.size() > 0){
        std::lock_guard<std::mutex> lock(this->pod.telemetry->updatesLock);
        for(std::string& update : this->pod.telemetry->updates){
            protoMessage->add_updatemessages(update);
        }
        this->pod.telemetry->updates.clear();
    }
    return protoMessage;
}


 // *  ***************** Brake Node Connection *******************


BrakeNodeConnection::BrakeNodeConnection(TelemetryManager pod) : UdpConnection(pod) {
    this->_connectionName = "Brake Node : ";
    this->_connectionStatusIndex = CONNECTION_FLAGS::BRAKE_NODE_HEARTBEAT_INDEX;
}

std::unique_ptr<google::protobuf::Message> BrakeNodeConnection::getProtoUpdateMessage() {
    std::unique_ptr<FcToBrakeNode> protoMessage (new FcToBrakeNode());
    protoMessage->set_packetnum(this->getNewPacketId());
    BrakeNodeStates manualState = this->pod.telemetry->manualBrakeNodeState;
    if(manualState == bnsNone){
        protoMessage->set_nodestate(this->pod.telemetry->commandedBrakeNodeState);
        return protoMessage;
    }
    if(manualState == bnsSolenoidControl){
        protoMessage->set_solenoid1config(this->pod.telemetry->manualSolenoidConfiguration[0]);
        protoMessage->set_solenoid2config(this->pod.telemetry->manualSolenoidConfiguration[1]);
        protoMessage->set_solenoid3config(this->pod.telemetry->manualSolenoidConfiguration[2]);
        protoMessage->set_solenoid4config(this->pod.telemetry->manualSolenoidConfiguration[3]);
    }
    protoMessage->set_nodestate(this->pod.telemetry->manualBrakeNodeState);
    //LOG(INFO)<<protoMessage->DebugString();
    return protoMessage;
}

bool BrakeNodeConnection::parseUpdate(char buffer[], int32_t messageSize){
    BrakeNodeToFc protoMessage = BrakeNodeToFc();
    if (!protoMessage.ParseFromArray(buffer, messageSize)) {
        std::string strError = "Failed to parse Update from Brake Node";
        throw std::invalid_argument(strError);
    }
    this->pod.setSolenoid(protoMessage.solenoid1(),SOL1_INDEX);
    this->pod.setSolenoid(protoMessage.solenoid2(), SOL2_INDEX);
    this->pod.setSolenoid(protoMessage.solenoid3(), SOL3_INDEX);
    this->pod.setSolenoid(protoMessage.solenoid4(), SOL4_INDEX);
    this->pod.setPressureVesselTemperature(protoMessage.pneumatictemperature());
    this->pod.setHighPressure(protoMessage.highpressure());
    this->pod.setLowPressure(protoMessage.lowpressure1(), LP1_INDEX);
    this->pod.setLowPressure(protoMessage.lowpressure2(), LP2_INDEX);
    this->pod.setLowPressure(protoMessage.lowpressure3(), LP3_INDEX);
    this->pod.setLowPressure(protoMessage.lowpressurecommon(), LP4_INDEX);
    this->pod.setPressureVesselTemperature(protoMessage.pneumatictemperature());
    this->pod.setCoolantTemperature(protoMessage.coolanttemperature());
    this->pod.setRecievedBrakeNodeState(protoMessage.state());
    return true;
}


// *  ***************** Enclosure Node Connection *******************

EnclosureNodeConnection::EnclosureNodeConnection(TelemetryManager pod) : UdpConnection(pod) {
    this->_connectionName = "Enclosure Node : ";
    this->_connectionStatusIndex = ENCLOSURE_HEARTBEAT_INDEX;
}

bool EnclosureNodeConnection::parseUpdate(char buffer[], int32_t messageSize){
    EnclosureNodeToFc protoMessage = EnclosureNodeToFc();
    if (!protoMessage.ParseFromArray(buffer, messageSize)) {
        std::string strError = "Failed to parse Update from Enclosure";
        throw std::invalid_argument(strError);
    }
    this->pod.setEnclosurePressure(protoMessage.enclosurepressure());
    this->pod.setEnclosureTemperature(protoMessage.enclosuretemperature());
    this->pod.setCoolantLinePressure(protoMessage.coolantpressure1());
    return true;
}


 // *  ***************** LVDC Node Connection *******************


LvdcNodeConnection::LvdcNodeConnection(TelemetryManager pod) : UdpConnection(pod) {
    this->_connectionName = "Lvdc Node : ";
    this->_connectionStatusIndex = LVDC_NODE_HEARTBEAT_INDEX;
}

bool LvdcNodeConnection::parseUpdate(char buffer[], int32_t messageSize){

    return true; //todo
}
