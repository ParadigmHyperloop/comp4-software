#include "FlightComputer/Heartbeat.h"
#include "EasyLogger/easylogging++.h"
#include <FlightComputer/NetworkHelpers.h>

/**
 *
 *
 */
class UdpConnection {
public:
    virtual ~UdpConnection() = default;

    explicit UdpConnection(Pod pod);

    /** Set the buffer size for the receiving buffer.
     *
     * @param iSize : Size of buffer
     */
    void setRecvBufferSize(int32_t);

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
    void configureClient(const std::string &strIp, int32_t iDestPort, int32_t iOutboundSocket);



    /**
     *
     * @param iServerPort : Port to listen for UDP on
     * @param timeoutMilis : How long to wait for incoming data before
     * announcing the connection inactive
     *
     * @throws : Any runtime error from createServerSocket
     */
    void configureServer(int32_t iServerPort, int32_t iTimeoutMilis);


    /**
     * Checks the buffer for data, and prases it if present.
     *
     * @requires: ConfigServer() has been called
     *
     * @throws: Runtime error if parsing the proto message fails.
     *
     * //TODO throw error if config server hasnt been set
     */
    void getUpdate();

    /**
     * Send update
     *
     * @requires: configClient has been filled
     *
     * @throws: runtime errors if protobuf fails to serialize or the socket is invalid
     */
    void giveUpdate();

    void closeConnection();

    /**
     * Each derived class will implement its own function to parse the proto
     * packet it receives
     *
     * @param cBuffer : Contains serialized proto message
     * @param iMessageSize : Size of the message in bytes
     * @return If it successfully parsed
     */
    virtual bool parseUpdate(char cBuffer[], int32_t iMessageSize) { return false; };

    virtual google::protobuf::Message *getProtoUpdateMessage();

    /**
     * Each derived class will be responsible for implementing how it notifies the shared memeory
     * the status of its connection.
     */
    virtual void setConnectionStatus(bool) {};

protected:
    bool createServerSocket();
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

    explicit PdsConnection(Pod pod);

    //TODO can we do this without putting the proto on the heap?
    google::protobuf::Message *getProtoUpdateMessage() override;
};


class BrakeNodeConnection : public UdpConnection {
public:

    ~BrakeNodeConnection() override = default;

    explicit BrakeNodeConnection(Pod pod);

    void setConnectionStatus(bool bStatus);

    google::protobuf::Message* getProtoUpdateMessage() override;

    bool parseUpdate(char cBuffer[], int32_t iMessageSize) override;

};

