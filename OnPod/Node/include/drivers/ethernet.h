#ifndef ETHERNET_H
#define ETHERNET_H

#include <Ethernet.h>
#include <EthernetUdp.h>
#include <pb_decode.h>
#include <pb_encode.h>

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

    // create recv and send buffers, then create NanoPB streams over them
    // this allows PB messages to be sent with Arduino's Ethernet library
    char cRecvBuffer[BUFFER_SIZE];
    pb_istream_t inStream = pb_istream_from_buffer((uint8_t*)cRecvBuffer, sizeof(cRecvBuffer));
    char cSendBuffer[BUFFER_SIZE];
    pb_ostream_t outStream = pb_ostream_from_buffer((uint8_t*)cSendBuffer, sizeof(cSendBuffer));
};

#endif
