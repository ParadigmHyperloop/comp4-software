#ifndef NODEETHERNET_H
#define NODEETHERNET_H

#include <Ethernet.h>
#include <EthernetUdp.h>

#define RX_BUFFER_SIZE 100
#define TX_BUFFER_SIZE 100

class UDPClass {
private:
    const uint8_t SS_PIN;
    const IPAddress NODE_IP;
    const uint16_t NODE_PORT;
    const uint8_t NODE_TYPE;

    EthernetUDP Udp;
public:
    UDPClass(uint8_t SS_PIN, IPAddress NODE_IP, uint16_t NODE_PORT,
        uint8_t NODE_TYPE);
    void init();
    bool readPacket();
    bool sendPacket(IPAddress ipDestinationIP, uint16_t uDestinationPort,
        uint16_t uMessageLength);

    uint8_t uRecvBuffer[RX_BUFFER_SIZE];
    uint8_t uSendBuffer[TX_BUFFER_SIZE];
};

#endif
