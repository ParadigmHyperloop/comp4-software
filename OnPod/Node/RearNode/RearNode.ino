#include <cstring>
#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "NodeEthernet.h"
#include "NodeStructs.h"
#include "adc.h"

/////// Node User Configurable //////////
const NodeType	NODE_TYPE   = REAR;
const NodeNum	NODE_NUM    = PRIMARY;

// UDP TX Parameters
const uint8_t	UDP_TX_IP[] = {192, 168, 0, 1};
const uint16_t	UDP_TX_Port = 8888;

// UDP RX Parameters
UDPClass 		udp(PIN_SPI_SS, IPAddress(192, 168, 2, 50), 777, NODE_TYPE, NODE_NUM);

// ADC Parameters
uint8_t adcsEnabled[] = {5}; //we may want all dflt on, setting only 5 for starters
uint8_t numAdcsEnabled = 1;

/////// End User Configurable ///////////

// Initialize global classes
RearNodeTxPkg 	NodeValues = {0, {0}, {0}, 0};
SPIClass 		adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
SPISettings 	adcSPISettings (20000000, MSBFIRST, SPI_MODE0);
ADS7953 		adc(adcSPI, adcSPISettings);

void setup() {
	Serial.begin(9600); // Keeping serial included for plug and debug

	// Initialize udp/adc channels
	udp.init();
    adc.init();
    for(uint8_t i = 0; i < numAdcsEnabled; i ++){
    	adc.enableChannel(adcsEnabled[i]);
    }
}

void loop() {
	// Check for incoming packet from BBB
	if (udp.readPacket()) {
		// the data can be accessed at udp.iPacketRecvBuffer
		udp.parseRearPacket();
	}

	// Read adc's
	adc.readActiveChannels();


	NodeValues.packetNum = udp.getTxPacketNum();
	NodeValues.adcValues = adc.iADCData;
	NodeValues.errCode = udp.getRxPacketNum();

	memcpy(udp.iPacketSendBuffer, &NodeValues, sizeof(NodeValues));
	if (udp.sendPacket(IPAddress(UDP_TX_IP), UDP_TX_Port)) {
		// the data in the buffer has been sent successfully
	}
	else {
		// there was an error in sending the packet
	}
}

void buildValuesPacket()
