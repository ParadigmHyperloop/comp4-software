#include <cstring>

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "NodeEthernet.h"
#include "NodeStructs.h"

/////// Node User Configurable //////////
const NodeType NODE_TYPE   = REAR;
const NodeNum  NODE_NUM    = PRIMARY;

const uint8_t  UDP_TX_IP[] = {192, 168, 0, 1};
const uint16_t UDP_TX_Port = 8888;
UDPClass udp(PIN_SPI_SS, IPAddress(192, 168, 2, 50), 777, NODE_TYPE); // Rx info

/////// End User Configurable ///////////
RearNodeTxPkg NodeValues = {0, {0}, {0}, 0};

// instantiate an ethernet object


void setup() {
	udp.init();	
}

void loop() {
	// Check for incoming packet from BBB
	if (udp.readPacket()) {
		// the data can be accessed at udp.iPacketRecvBuffer
		udp.parseRearPacket();
	}

	NodeValues.packetNum = udp.getTxPacketNum();
	NodeValues.errCode = udp.getRxPacketNum();
	memcpy(udp.iPacketSendBuffer, &NodeValues, sizeof(NodeValues));
	// example send to an arbitrary IP and port
	if (udp.sendPacket(IPAddress(UDP_TX_IP), UDP_TX_Port)) {
		// the data in the buffer has been sent successfully
	}
	else {
		// there was an error in sending the packet
	}
}
