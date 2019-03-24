#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "NodeEthernet.h"

UDPClass::UDPClass(uint8_t ETHERNET_SS_PIN, IPAddress NODE_IP,
                    uint16_t NODE_PORT, uint8_t NODE_TYPE):
    ETHERNET_SS_PIN(ETHERNET_SS_PIN),
    NODE_IP(NODE_IP),
    NODE_PORT(NODE_PORT),
    NODE_TYPE(NODE_TYPE)
    {}

void UDPClass::init() {
    uint8_t mac[] = {NODE_TYPE, 0xFF, 0xFF, 0xFF, 0xFF};
    Ethernet.init(ETHERNET_SS_PIN);
    Ethernet.begin(mac, NODE_IP);
    // blocks until W5500 responds and an ethernet cable is connected
    while (Ethernet.linkStatus() == LinkOFF ||
           Ethernet.hardwareStatus() == EthernetNoHardware) {
        delay(100);
    }
    Udp.begin(NODE_PORT);
}

bool UDPClass::readPacket() {
    Udp.parsePacket();
    // return false if no data is available
    if (!Udp.available()) {
        return false;
    }
    // zero the receive buffer
    memset(&iPacketRecvBuffer, 0, UDP_TX_PACKET_MAX_SIZE);
    // read a packet into the receive buffer
    Udp.read(iPacketRecvBuffer, UDP_TX_PACKET_MAX_SIZE);
    return true;
}

bool UDPClass::sendPacket(IPAddress iDestinationIP, uint16_t iDestinationPort) {
    // return false if there was an error resolving the IP or port
    if (!Udp.beginPacket(iDestinationIP, iDestinationPort)){
        return false;
    }
    Udp.write(iPacketSendBuffer);
    // return false if there was an error sending the packet
    if (Udp.endPacket()){
        return false;
    }
    txPacketNum ++;
    return true;
}

void UDPClass::parseRearPacket(){
	NodeType type = (NodeType)iPacketRecvBuffer[0];
	uint32_t packet = iPacketRecvBuffer[sizeof(NodeType)];
	if (type == REAR && packet > rxPacketNum){ // check if recent packet
		rxPacketNum = packet;

		// can memcopy packet to a struct here but not needed for rear node at this time
	}
}
