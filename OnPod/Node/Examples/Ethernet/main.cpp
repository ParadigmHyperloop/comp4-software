#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "ethernet.h"

// here as an example - this will be moved to a node struct header file
enum NodeType {
	REAR = 0,
	BRAKE = 1,
	LVDC = 2
};
const NodeType NODE_TYPE = REAR;

// instantiate an ethernet object
UDPClass udp(PIN_SPI_SS, IPAddress(192, 168, 2, 50), 777, NODE_TYPE);

void setup() {
	udp.init();	// initialize the ethernet object
}

void loop() {
	// READING DATA
	if (udp.readPacket()) {
		// the data can be accessed at udp.iPacketRecvBuffer
	}
	else {
		// read failed or no available data
	}


	// SENDING DATA
	// first place the data to be sent into udp.iPacketSendBuffer
	udp.iPacketSendBuffer[0] = 0x01; // example of setting data
	// example send to an arbitrary IP and port
	if (udp.sendPacket(IPAddress(192, 168, 0, 1), 8888)) {
		// the data in the buffer has been sent successfully
	}
	else {
		// there was an error in sending the packet
	}
}
