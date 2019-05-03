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
        }

        void giveUpdate() {
            const char *cPayload = this->getUpdateMessage();
            ssize_t sent = sendto(this->iOutboundSocket, cPayload, strlen(cPayload), 0,
                                  (struct sockaddr *) &this->sDestAddr, sizeof(this->sDestAddr));
            if (sent == -1) {
                std::string strError = std::string("Error Sending ") + this->strConnectionName + std::strerror(errno);
                throw std::runtime_error(strError);
            }
        }

        void closeConnection() {
            close(this->iInboundSocket);
            close(this->iOutboundSocket);
        }

        virtual bool parseUpdate(char cBuffer[], int32_t iMessageSize) { return false; };

        virtual const char *getUpdateMessage(){return "";};

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

        const char *getUpdateMessage() override {

            // Big Fuckin jesus proto message with all the values

            //TODO So when we call getUpdateMessage it returns a pointer to an array that holds our update message.
            // this memory is locally allocted so the memory that pointer points to is basicly hanging. We either need
            // but this on the heap and delete it once we're done. Or just return a copy instead of a pointer.

            return "";};
};




class BrakeNodeConnection : public UdpConnection {
public:

    ~BrakeNodeConnection() override = default;

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

    explicit BrakeNodeConnection(Pod pod) : UdpConnection(pod) {
        this->strConnectionName = "Brake Node Data : ";
    };

    void setConnectionStatus(bool bStatus) override {
        this->pod.sPodValues->cConnectionsArray[0] = bStatus;
    }

    bool parseUpdate(char cBuffer[], int32_t iMessageSize) override { return true; }

};

