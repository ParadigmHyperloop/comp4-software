#include "node_ethernet.h"

UDPClass::UDPClass(uint8_t uSSPin, IPAddress nodeIP, uint16_t uNodePort,
                    uint8_t eNodeType) :
    SS_PIN(uSSPin),
    NODE_IP(nodeIP),
    NODE_PORT(uNodePort),
    NODE_TYPE(eNodeType)
    {}

void UDPClass::init() {
    uint8_t uMac[] = {0xDE, 0xFF, 0xFF, 0xFF, NODE_TYPE};
    Ethernet.init(SS_PIN);
    Ethernet.begin(uMac, NODE_IP);
    Udp.begin(NODE_PORT);
}

bool UDPClass::readPacket() {
    if (Udp.parsePacket() != 0) {
        memset(uRecvBuffer, 0, RX_BUFFER_SIZE); // zero the receive buffer
        Udp.read(uRecvBuffer, RX_BUFFER_SIZE);  // read a packet into the receive buffer
        return true;
    }
    return false;
}

bool UDPClass::sendPacket(IPAddress ipDestinationIP, uint16_t uDestinationPort,
    uint16_t uMessageLength) {
    if (!Udp.beginPacket(ipDestinationIP, uDestinationPort)) {
        return false; // return false if there was an error resolving the IP or port
    }
    Udp.write(uSendBuffer, uMessageLength);
    memset(uSendBuffer, 0, TX_BUFFER_SIZE); // zero the transmit buffer
    if (!Udp.endPacket()) {
        return false; // return false if there was an error sending the packet
    }
    return true;
}
