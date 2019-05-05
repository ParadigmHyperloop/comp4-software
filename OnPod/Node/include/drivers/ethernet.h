#ifndef ETHERNET_H
#define ETHERNET_H

#include <Ethernet.h>
#include <EthernetUdp.h>

#define BUFFER_SIZE 100

class UDPClass {
private:
    const uint8_t ETHERNET_SS_PIN = PIN_SPI_SS;
    const IPAddress NODE_IP;
    const uint16_t NODE_PORT;
    const uint8_t NODE_TYPE;
    EthernetUDP Udp;
public:
    UDPClass(IPAddress NODE_IP, uint16_t NODE_PORT, uint8_t NODE_TYPE);
    void init();
    bool readPacket();
    bool sendPacket(IPAddress ipDestinationIP, uint16_t uDestinationPort);

    // create recv and send buffers
    char cRecvBuffer[BUFFER_SIZE];
    char cSendBuffer[BUFFER_SIZE];
};

#endif
