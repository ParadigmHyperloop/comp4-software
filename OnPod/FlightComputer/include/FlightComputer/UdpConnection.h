#include "FlightComputer/Heartbeat.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/Network.h>

class UdpConnection {
    public:
        virtual ~UdpConnection() = default;

        explicit UdpConnection(Pod pod) {
            this->pod = pod;
            struct sockaddr_in sAddr = {0};
            this->sDestAddr = sAddr;
        };

        void setRecvBufferSize(int32_t iSize){
            int32_t s = setsockopt(this->iInboundSocket, SOL_SOCKET, SO_RCVBUF, &iSize, sizeof(iSize));
        }

        void configureClient(const std::string &strIp, int32_t iDestPort, int32_t iOutboundSocket) {
            struct sockaddr_in sAddr = {0};
            this->iOutboundSocket = iOutboundSocket;
            this->sDestAddr = sAddr;
            this->sDestAddr = createGenericNodeAddr();
            this->sDestAddr.sin_port = htons(iDestPort);
            this->sDestAddr.sin_addr.s_addr = inet_addr(strIp.c_str());
        }

        void configureServer(int32_t iServerPort, int32_t timeoutMilis ) {
            this->iServerPort = iServerPort;
            try {
                this->createServerSocket();
            }
            catch (std::runtime_error &e) {
                throw e;
            }
        }

        void getUpdate() {
            char cBuffer[100] = {0};
            bzero(&cBuffer, sizeof cBuffer);
            ssize_t iReceivedPacketSize = recvfrom(this->iInboundSocket, cBuffer, 100, 0, nullptr, nullptr);
            if (iReceivedPacketSize != -1) {
                LOG(INFO) << iReceivedPacketSize << "Bytes received on socket:" << this->strConnectionName << ": " << cBuffer;
                try {
                    this->parseUpdate(cBuffer, (int32_t)iReceivedPacketSize);
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
                LOG(DEBUG) << "No Packet Received on " << this->strConnectionName;
            }
        }

        void giveUpdate() {
            google::protobuf::Message* pPacket;
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

        virtual bool parseUpdate(char cBuffer[], int32_t iMessageSize) { return false; };

        virtual google::protobuf::Message* getProtoUpdateMessage(){
            auto protoMessage = new defaultFcToNode();
            protoMessage->set_podstate(psArmed);
            return protoMessage;
        };

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

class PdsConnection : UdpConnection {
    public:
        ~PdsConnection() override = default;

        explicit PdsConnection(Pod pod) : UdpConnection(pod){
            this->strConnectionName = "Controls Interface Data : ";
        };

        //TODO can we do this without putting the proto on the heap?
        google::protobuf::Message* getProtoUpdateMessage() override {
                auto protoMessage = new telemetry();
                protoMessage->set_ilowpressure1(pod.sPodValues->iLowPressure1);
                protoMessage->set_ihighpressure(pod.sPodValues->iHighPressure);
                protoMessage->set_bsolenoid1(pod.sPodValues->bSolenoid1);
                protoMessage->set_bsolenoid2(pod.sPodValues->bSolenoid2);
                protoMessage->set_pressurevesseltemperature(pod.sPodValues->iPressureVesselTemperature);
                protoMessage->set_railtemperature(pod.sPodValues->iRailTemperature);
                return protoMessage;
        }
};




class BrakeNodeConnection : public UdpConnection {
public:

    ~BrakeNodeConnection() override = default;

    explicit BrakeNodeConnection(Pod pod) : UdpConnection(pod) {
        this->strConnectionName = "Brake Node Data : ";
    };

    void setConnectionStatus(bool bStatus) override {
        this->pod.sPodValues->cConnectionsArray[0] = bStatus;
    }

    google::protobuf::Message* getProtoUpdateMessage() override {
        auto protoMessage = new fcToBrakeNode();
        protoMessage->set_podstate(psArmed);
        protoMessage->set_manualnodestate(bnsArmed);
        return protoMessage;
    }

    bool parseUpdate(char cBuffer[], int32_t iMessageSize) override {
        dtsNodeToFc protoMessage = dtsNodeToFc();
        if(! protoMessage.ParseFromArray(cBuffer, iMessageSize))
        {
            std::string strError = "Failed to parse Update from Brake Node";
            throw std::invalid_argument(strError);
        }
        this->pod.sPodValues->bSolenoid1 = protoMessage.brakesolenoidstate();
        this->pod.sPodValues->bSolenoid2 = protoMessage.ventsolenoidstate();
        this->pod.sPodValues->iRailTemperature = protoMessage.rotortemperature();
        this->pod.sPodValues->iPressureVesselTemperature = protoMessage.pressuretemperature();
        this->pod.sPodValues->iHighPressure = protoMessage.highpressure();
        this->pod.sPodValues->iLowPressure1 = protoMessage.lowpressure();
        return true;
    }

};

