#include "ethernet.h"

UDPClass::UDPClass(IPAddress NODE_IP, uint16_t NODE_PORT, uint8_t NODE_TYPE):
    NODE_IP(NODE_IP),
    NODE_PORT(NODE_PORT),
    NODE_TYPE(NODE_TYPE)
    {}

void UDPClass::init() {
    uint8_t uMac[] = {NODE_TYPE, 0xFF, 0xFF, 0xFF, 0xFF};
    Ethernet.init(ETHERNET_SS_PIN);
    Ethernet.begin(uMac, NODE_IP);
    // blocks until W5500 responds and an ethernet cable is connected
    while (Ethernet.linkStatus() == LinkOFF ||
           Ethernet.hardwareStatus() == EthernetNoHardware) {
        delay(100);
    }
    Udp.begin(NODE_PORT);
}

bool UDPClass::readPacket() {
    Udp.parsePacket();
    if (!Udp.available()) {
        return false;  // return false if no data is available
    }
    Udp.read(cRecvBuffer, BUFFER_SIZE);  // read a packet into the receive buffer
    return true;
}

bool UDPClass::sendPacket(IPAddress ipDestinationIP, uint16_t uDestinationPort,
    uint16_t uMessageLength) {
    if (!Udp.beginPacket(ipDestinationIP, uDestinationPort)) {
        return false; // return false if there was an error resolving the IP or port
    }
    Udp.write(cSendBuffer, uMessageLength);
    memset(cSendBuffer, 0, BUFFER_SIZE);
    if (!Udp.endPacket()) {
        return false; // return false if there was an error sending the packet
    }

    return true;
}
