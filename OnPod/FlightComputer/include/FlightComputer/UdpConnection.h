#include "FlightComputer/Heartbeat.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>

/**
 *
 *
 */
class UdpConnection {
public:
    virtual ~UdpConnection() = default;

    explicit UdpConnection(Pod pod) {
        this->pod = pod;
        struct sockaddr_in sAddr = {0};
        this->sDestAddr = sAddr;
    };

    /** Set the buffer size for the receiving buffer.
     *
     * @param iSize : Size of buffer
     */
    void setRecvBufferSize(int32_t iSize) {
        int32_t s = setsockopt(this->iInboundSocket, SOL_SOCKET, SO_RCVBUF, &iSize, sizeof(iSize));
    }

    /**
     * Since UDP clients can share an outbound socket, it is
     * passed as a paramater.
     *
     * This simply pre configures the sockaddr_in struct required by sendto()
     *
     * @param strIp : IP of the destination address
     * @param iDestPort : Port of the destination address
     * @param iOutboundSocket : Socket to use when sending data
     */
    void configureClient(const std::string &strIp, int32_t iDestPort, int32_t iOutboundSocket) {
        struct sockaddr_in sAddr = {0};
        this->iOutboundSocket = iOutboundSocket;
        this->sDestAddr = sAddr;
        this->sDestAddr = createGenericNodeAddr();
        this->sDestAddr.sin_port = htons(iDestPort);
        this->sDestAddr.sin_addr.s_addr = inet_addr(strIp.c_str());
    }


    /**
     *
     * @param iServerPort : Port to listen for UDP on
     * @param timeoutMilis : How long to wait for incoming data before
     * announcing the connection inactive
     *
     * @throws : Any runtime error from createServerSocket
     */
    void configureServer(int32_t iServerPort, int32_t iTimeoutMilis) {
        this->pulse = Heartbeat(iTimeoutMilis);
        this->iServerPort = iServerPort;
        try {
            this->createServerSocket();
        }
        catch (std::runtime_error &e) {
            throw e;
        }
    }

    /**
     * Checks the buffer for data, and prases it if present.
     *
     * @requires: ConfigServer() has been called
     *
     * @throws: Runtime error if parsing the proto message fails.
     *
     * //TODO throw error if config server hasnt been set
     */
    void getUpdate() {
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

    /**
     * Send update
     *
     * @requires: configClient has been filled
     *
     * @throws: runtime errors if protobuf fails to serialize or the socket is invalid
     */
    void giveUpdate() {
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

    void closeConnection() {
        close(this->iInboundSocket);
        close(this->iOutboundSocket);
    }

    /**
     * Each derived class will implement its own function to parse the proto
     * packet it receives
     *
     * @param cBuffer : Contains serialized proto message
     * @param iMessageSize : Size of the message in bytes
     * @return If it successfully parsed
     */
    virtual bool parseUpdate(char cBuffer[], int32_t iMessageSize) { return false; };

    virtual google::protobuf::Message *getProtoUpdateMessage() {
        auto protoMessage = new defaultFcToNode();
        protoMessage->set_podstate(psArmed);
        return protoMessage;
    };

    /**
     * Each derived class will be responsible for implementing how it notifies the shared memeory
     * the status of its connection.
     */
    virtual void setConnectionStatus(bool) {};

protected:
    bool createServerSocket() {
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

    Pod pod;
    int32_t iOutboundSocket = -1;
    int32_t iInboundSocket = -1;
    int32_t iServerPort = -1;
    Heartbeat pulse = Heartbeat(0);
    std::string strConnectionName = "";
    struct sockaddr_in sDestAddr{};
};

class PdsConnection : public UdpConnection {
public:
    ~PdsConnection() override = default;

    explicit PdsConnection(Pod pod) : UdpConnection(pod) {
        this->strConnectionName = "Controls Interface Data : ";
    };

    //TODO can we do this without putting the proto on the heap?
    google::protobuf::Message *getProtoUpdateMessage() override {
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
};


class BrakeNodeConnection : public UdpConnection {
public:

    ~BrakeNodeConnection() override = default;

    explicit BrakeNodeConnection(Pod pod) : UdpConnection(pod) {
        this->strConnectionName = "Brake Node : ";
    };

    void setConnectionStatus(bool bStatus) override {
        this->pod.sPodValues->cConnectionsArray[0] = bStatus;
    }

    google::protobuf::Message *getProtoUpdateMessage() override {
        auto protoMessage = new FcToBrakeNode();
        protoMessage->set_podstate(psArmed);
        protoMessage->set_manualnodestate(bnsArmed);
        return protoMessage;
    }

    bool parseUpdate(char cBuffer[], int32_t iMessageSize) override {
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

};

