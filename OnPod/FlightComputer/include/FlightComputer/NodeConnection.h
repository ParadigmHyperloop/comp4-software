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

        bool configure(const std::string &strIp, int32_t iDestPort, int32_t iServerPort, int32_t timeoutMilis,
                       int32_t iOutboundSocket) {

            this->iServerPort = iServerPort;
            struct sockaddr_in sAddr = {0};

            this->iOutboundSocket = iOutboundSocket;
            iInboundSocket = -1;
            try {
                this->createServerSocket();
            }
            catch (std::runtime_error &e) {
                throw e;
            }

            this->sDestAddr = sAddr;
            this->sDestAddr = createGenericNodeAddr();
            this->sDestAddr.sin_port = htons(iDestPort);
            this->sDestAddr.sin_addr.s_addr = inet_addr(strIp.c_str());

            return true;
        }

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
            //TODO move to own function
            int32_t iRecvBufSize = 20; // Keep buffer size small so we dont queue up old telemetry
            int32_t s = setsockopt(iSocketfd, SOL_SOCKET, SO_RCVBUF, &iRecvBufSize, sizeof(iRecvBufSize));

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

        bool giveUpdate() {
            const char *cPayload = this->getUpdateMessage();
            ssize_t sent = sendto(this->iOutboundSocket, cPayload, strlen(cPayload), 0,
                                  (struct sockaddr *) &this->sDestAddr, sizeof(this->sDestAddr));
            if (sent == -1) {
                std::string strError = std::string("Sending node data: ") + std::strerror(errno);
                throw std::runtime_error(strError);
            }
            return true;
        }

        void closeConnection() {
            close(this->iInboundSocket);
            close(this->iOutboundSocket);
        }

        virtual const char *getUpdateMessage(){return "";};


    protected:
        Pod pod;
        int32_t iOutboundSocket = -1;
        int32_t iInboundSocket = -1;
        int32_t iServerPort = -1;
        Heartbeat pulse = Heartbeat(0);
        std::string strConnectionName;
        struct sockaddr_in sDestAddr;

};


class NodeConnection : public UdpConnection {
public:

    ~NodeConnection() override = default;

    explicit NodeConnection(Pod pod):UdpConnection(pod){};

    const char *getUpdateMessage() override {
        // A temporary plain text message for easy debugging on the node sim side
        PodStates PodState = this->pod.getPodState();
        switch (PodState) {
            case psBooting:
                return "Booting";
            case psStandby:
                return "StandBy";
            case psArming:
                return "Arming";
            case psArmed:
                return "Armed";
            case psPreFlight:
                return "PreFlight";
            case psAcceleration:
                return "Acceleration";
            case psCoasting:
                return "Coasting";
            case psBraking:
                return "Braking";
            case psDisarming:
                return "Disarming";
            case psRetrieval:
                return "Retrieval";
            default:
                return "Emergency";
        }
    }



    void getUpdate() {
        char cBuffer[30] = {0};
        bzero(&cBuffer, sizeof cBuffer);
        ssize_t iReceivedPacketSize = recvfrom(this->iInboundSocket, cBuffer, 30, 0, nullptr, nullptr);
        if (iReceivedPacketSize != -1) {
            LOG(INFO) << iReceivedPacketSize << "Bytes received on socket:" << this->strConnectionName << ": " << cBuffer;
            try {
                this->parseUpdate(cBuffer, (int32_t)iReceivedPacketSize);
            }
            catch (const std::invalid_argument &e) {
                std::string strError = "getUpdate " + this->strConnectionName + ": " + e.what();
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
        return;
    }




    virtual bool parseUpdate(char cBuffer[], int32_t iMessageSize) { return false; };

    virtual void setConnectionStatus(bool) {};

};


class BrakeNodeConnection : public NodeConnection {
public:

    ~BrakeNodeConnection() override = default;

    explicit BrakeNodeConnection(Pod pod) : NodeConnection(pod) {
        this->strConnectionName = "Brake Node";
    };

    void setConnectionStatus(bool bStatus) override {
        this->pod.sPodValues->cConnectionsArray[0] = bStatus;
    }

    bool parseUpdate(char cBuffer[], int32_t iMessageSize) override { return true; }

};

