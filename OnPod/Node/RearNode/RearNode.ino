#include <cstring>

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "NodeEthernet.h"
#include "NodeStructs.h"

const NodeType NODE_TYPE = REAR;
const NodeNum  NODE_NUM  = PRIMARY;
RearNodeTxPkg NodeValues;

// instantiate an ethernet object
UDPClass udp(PIN_SPI_SS, IPAddress(192, 168, 2, 50), 777, NODE_TYPE);

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
	//NodeValues.adcValues = 0;
	//NodeValues.dacValues = 0;
	memcpy(udp.iPacketSendBuffer, &NodeValues, sizeof(NodeValues));
	// example send to an arbitrary IP and port
	if (udp.sendPacket(IPAddress(192, 168, 0, 1), 8888)) {
		// the data in the buffer has been sent successfully
	}
	else {
		// there was an error in sending the packet
	}
}
