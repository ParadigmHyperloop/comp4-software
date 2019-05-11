#ifndef FLIGHTCOMPUTER_UDPCONNECTION_H
#define FLIGHTCOMPUTER_UDPCONNECTION_H


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
     * @param destIp : IP of the destination address
     * @param destPortNum : Port of the destination address
     * @param outboundSocket : Socket to use when sending data
     */
    void configureClient(const std::string &destIp, int32_t destPortNum, int32_t outboundSocket);



    /**
     *
     * @param iServerPort : Port to listen for UDP on
     * @param timeoutMilis : How long to wait for incoming data before
     * announcing the connection inactive
     *
     * @throws : Any runtime error from createServerSocket
     */
    void configureServer(int32_t serverPort, int32_t connectionTimeoutMilis);


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
    bool _createServerSocket();
    Pod pod;
    int32_t _outboundSocket = -1;
    int32_t _inboundSocket = -1;
    int32_t _serverPort = -1;
    Heartbeat _pulse = Heartbeat(0);
    std::string _connectionName = "";
    struct sockaddr_in _destSockAddr{};
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

    bool parseUpdate(char buffer[], int32_t messageSize) override;

};

#endif //FLIGHTCOMPUTER_UDPCONNECTION_H
