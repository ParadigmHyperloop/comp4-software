#include <cstring>
#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "NodeEthernet.h"
#include "NodeStructs.h"
#include "adc.h"

///////// Node User Configurable //////////
const NodeType	NODE_TYPE   = REAR;
const NodeNum	NODE_NUM    = PRIMARY;

// UDP TX Parameters
UDPParams IPPara;

// UDP RX Parameters
UDPClass 		udp(PIN_SPI_SS, IPAddress(IPPara.REAR_NODE_IP), IPPara.NODE_RX_UDP_PORT, NODE_TYPE, NODE_NUM);

// ADC Parameters
uint8_t adcsEnabled[] = {5}; // we may want all dflt on, setting only 5 for starters
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
		udp.parseRearPacket();
	}

	adc.readActiveChannels();
	// dac.read();
	buildRearTxPacket();

	if (!udp.sendPacket(IPAddress(IPPara.BBB_IP), IPPara.BBB_UDP_PORT)) {
		Serial.println("eror on UDP tx num : " + udp.getTxPacketNum());
	}
}

void buildRearTxPacket(){
	NodeValues.packetNum = udp.getTxPacketNum();
	NodeValues.adcValues[0] = *adc.getiADCData();
	NodeValues.errCode = udp.getRxPacketNum();
	memcpy(udp.iPacketSendBuffer, &NodeValues, sizeof(NodeValues));
}
