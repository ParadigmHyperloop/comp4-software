#include <cstring>
#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include "NodeEthernet.h"
#include "NodeStructs.h"
#include "adc.h"

///////// User Configurable //////////
const NodeType	NODE_TYPE   = REAR;
const NodeNum	NODE_NUM    = PRIMARY;

uint8_t numAdcsToEnable = 1;
uint8_t adcsToEnable[] = {5};

bool serialDebug = true;

///////// Globals ///////////
// UDP TX Parameters
UDPParams IPPara;

// UDP RX Parameters
UDPClass udp(PIN_SPI_SS, IPAddress(IPPara.REAR_NODE_IP), IPPara.NODE_RX_UDP_PORT, NODE_TYPE, NODE_NUM);

// Struct of values for current Node
RearNodeTxPkg 	nodeValues = {0, {0}, 0};
NodeRxPkg		lastRecvdPkg = {NODE_TYPE, 0, 0, 0, 0};
SPIClass 		adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
SPISettings 	adcSPISettings (20000000, MSBFIRST, SPI_MODE0);
ADS7953 		adc(adcSPI, adcSPISettings);

///////// Inline Functions //////////
void buildRearTxPacket(){
	nodeValues.packetNum = udp.getTxPacketNum();
	nodeValues.adcValues[0] = *adc.getiADCData();
	memcpy(udp.iPacketSendBuffer, &nodeValues, min(UDP_TX_PACKET_MAX_SIZE, sizeof(nodeValues)));
}

///////// Program Start //////////
void setup() {
	Serial.begin(9600); // Keeping serial included for plug and debug

	// Initialize udp/adc channels
	udp.init();
    adc.init();
    for (uint8_t adcNum = 0; adcNum < numAdcsToEnable; adcNum++) {
    	adc.enableChannel(adcsToEnable[adcNum]);
    }
}

void loop() {
	// Check for incoming packet from BBB
	if (udp.readPacket()) {
		if (!udp.parseRxPacket(lastRecvdPkg)){
			nodeValues.errCode |= RX_PARSE_ERR;
		}
		else {
			// Add handling of overrideCodes here, to turn on/off Adc's,
			// change Tx intervals, reboot, etc.
		}
	}

	adc.readActiveChannels();

	buildRearTxPacket();

	if (!udp.sendPacket(IPAddress(IPPara.BBB_IP), IPPara.BBB_UDP_PORT)) {
		Serial.println("error on UDP tx num : " + udp.getTxPacketNum());
	}
}

