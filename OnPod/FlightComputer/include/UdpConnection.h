#ifndef FLIGHTCOMPUTER_UDPCONNECTION_H
#define FLIGHTCOMPUTER_UDPCONNECTION_H


#include "Heartbeat.h"
#include "Common.h"
#include "TelemetryManager.h"
#include <NetworkHelpers.h>

/**
 *
 *
 */
class UdpConnection {
public:
    virtual ~UdpConnection() = default;

    explicit UdpConnection(TelemetryManager pod);

    /** Set the buffer size for the receiving buffer.
     *
     * @param iSize : Size of buffer
     */
    void setRecvBufferSize(int32_t bufferSize);

    /**
     * Since UDP clients can share an outbound socket, it is
     * passed as a paramater.
     *
     * This simply pre configures the sockaddr_in struct required by sendto()
     *
     * @param destIp : IP of the destination address
     * @param destPortNum : Port of the destination address
     * @param outboundSocket : Socket to use when sending data
     */
    void configureClient(const std::string &destIp, int32_t destPortNum, int32_t outboundSocket, int32_t update_freq);



    /**
     *
     * @param iServerPort : Port to listen for UDP on
     * @param timeoutMilis : How long to wait for incoming data before
     * announcing the connection inactive
     *
     * @throws : Any runtime error from createServerSocket
     */
    void configureServer( int32_t serverPort, int32_t connectionTimeoutMilis);


    /**
     * Checks the buffer for data, and prases it if present.
     *
     * @requires: ConfigServer() has been called
     *
     * @throws: Runtime error if parsing the proto message fails or socket is broken.
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

    virtual std::unique_ptr<google::protobuf::Message> getProtoUpdateMessage();


    /**
     * Returns the index that represents this connection in the "ConnectionFlags" vector used for the
     * main control loop
     *
     * @return
     */
    int32_t getConnectionIndex();

    int32_t getNewPacketId();

    int32_t checkPacketId(int32_t);

protected:
    bool _createServerSocket();
    TelemetryManager pod;
    uint32_t _lastPacketSentId = 0;
    uint32_t _lastPacketReceivedId = 0;
    int32_t _outboundSocket = -1;
    int32_t _inboundSocket = -1;
    int32_t _serverPort = -1;
    Heartbeat _pulse = Heartbeat(0);
    Heartbeat _update_freq;
    std::string _connectionName = "";
    struct sockaddr_in _destSockAddr{};
    int32_t _connectionStatusIndex;
};

class PdsConnection : public UdpConnection {
public:
    ~PdsConnection() override = default;

    explicit PdsConnection(TelemetryManager pod);

    //TODO can we do this without putting the proto on the heap?
    std::unique_ptr<google::protobuf::Message> getProtoUpdateMessage() override;
};


class BrakeNodeConnection : public UdpConnection {
public:

    ~BrakeNodeConnection() override = default;

    explicit BrakeNodeConnection(TelemetryManager pod);

    std::unique_ptr<google::protobuf::Message> getProtoUpdateMessage() override;

    bool parseUpdate(char buffer[], int32_t messageSize) override;

};

class EnclosureNodeConnection : public UdpConnection{

    public:
    ~EnclosureNodeConnection() override = default;

    explicit EnclosureNodeConnection(TelemetryManager pod);

    bool parseUpdate(char buffer[], int32_t messageSize) override;

};

#endif //FLIGHTCOMPUTER_UDPCONNECTION_H
