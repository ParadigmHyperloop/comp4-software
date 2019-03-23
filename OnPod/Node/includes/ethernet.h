#ifndef ETHERNET_H
#define ETHERNET_H

class UDPClass {
private:
    const uint8_t ETHERNET_SS_PIN;
    const IPAddress NODE_IP;
    const uint16_t NODE_PORT;
    const uint8_t NODE_TYPE;
    EthernetUDP Udp;
public:
    UDPClass(uint8_t ETHERNET_SS_PIN, IPAddress NODE_IP,
             uint16_t NODE_PORT, uint8_t NODE_TYPE);
    void init();
    bool readPacket();
    bool sendPacket(IPAddress iDestinationIP, uint16_t iDestinationPort);
    // UDP_TX_PACKET_MAX_SIZE is 24B - this can be made much larger
    char iPacketRecvBuffer[UDP_TX_PACKET_MAX_SIZE];
    char iPacketSendBuffer[UDP_TX_PACKET_MAX_SIZE];
};

#endif
