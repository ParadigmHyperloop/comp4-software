#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "NodeEthernet.h"

UDPClass::UDPClass(uint8_t ethernetSSPin, IPAddress nodeIp,
                    uint16_t nodePort, uint8_t nodeType, uint8_t nodeNum):
    ETHERNET_SS_PIN(ethernetSSPin),
    NODE_IP(nodeIp),
    NODE_PORT(nodePort),
    NODE_TYPE(nodeType),
	NODE_NUM(nodeNum)
    {}

void UDPClass::init() {
    uint8_t mac[] = {NODE_TYPE, NODE_NUM, 0xFF, 0xFF, 0xFF};
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

bool UDPClass::parseRxPacket(NodeRxPkg &pkg){
	// check if packet type matches this objects type and is most current
	if (((NodeType)iPacketRecvBuffer[0] == NODE_TYPE) &&
			(iPacketRecvBuffer[sizeof(pkg.nodeType)] > rxPacketNum)){
		// copy data, create switch case here if different behavior required
		memcpy(&pkg, iPacketRecvBuffer, sizeof(NodeRxPkg));
	}
	return true;
}

