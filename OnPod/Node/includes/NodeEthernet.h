#ifndef NODEETHERNET_H
#define NODEETHERNET_H

#include "NodeStructs.h"

class UDPClass {
private:
    const uint8_t ETHERNET_SS_PIN;
    const IPAddress NODE_IP;
    const uint16_t NODE_PORT;
    const uint8_t NODE_TYPE;
    const uint8_t NODE_NUM;
    EthernetUDP Udp;
    uint32_t rxPacketNum = 0;
    uint32_t txPacketNum = 1;
    uint32_t bbbState    = 0;

public:
    UDPClass(uint8_t ethernetSSPin, IPAddress nodeIp,
            uint16_t nodePort, uint8_t nodeType, uint8_t nodeNum);
    void init();
    bool readPacket();
    bool sendPacket(IPAddress iDestinationIP, uint16_t iDestinationPort);
    void parseRearPacket();
    // UDP_TX_PACKET_MAX_SIZE is 24B - this can be made much larger
    char iPacketRecvBuffer[UDP_TX_PACKET_MAX_SIZE];
    char iPacketSendBuffer[UDP_TX_PACKET_MAX_SIZE];
    uint32_t getTxPacketNum(){return txPacketNum;};
    uint32_t getRxPacketNum(){return rxPacketNum;};
};

#endif
