#include <Arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <SPI.h>

#include "Sodaq_wdt.h"
#include "Timer.h"

#include "NodeStructs.h"
#include "Paradigm.pb.h"

#include "drivers/node_ethernet.h"
#include "drivers/adc.h"
#include "sensors/thermocouple.h"

const NodeType NODE_TYPE = REAR;

// timer
Timer txTimer;
const uint32_t TX_INTERVAL_MS = 50;

// sensors
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI);
/*
typeKThermo thermo(&adc, 0);

// comms
UDPParams IPPara;  // keep IPs and ports in one location
UDPClass udp(PIN_SPI_SS, IPAddress(IPPara.REAR_NODE_IP), IPPara.NODE_RX_PORT, NODE_TYPE);
RearNodeToFc pRearNodeTelemetry = RearNodeToFc_init_default;  // protobuf message

void sendToFlightComputer(void*) {
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, RearNodeToFc_fields, &pRearNodeTelemetry);
    udp.sendPacket(IPAddress(IPPara.BBB_IP), IPPara.BBB_PORT, outStream.bytes_written);
}
*/
void setup() {
    //udp.init();
    adc.init();

    //txTimer.every(TX_INTERVAL_MS, &sendToFlightComputer, (void*)0);
}

void loop() {
    /*
	adc.readActiveChannels();  // update sensor data
    // store sensor data in the protobuf message object
    pRearNodeTelemetry.coolantTemperature = thermo.read();
    pRearNodeTelemetry.coolantPressure1 = 0;
    pRearNodeTelemetry.coolantPressure1 = 0;
    pRearNodeTelemetry.coolantPressure1 = 0;
    pRearNodeTelemetry.tubePressure = 0;
    txTimer.update();  // check to see if it's time to send another packet
*/
Serial.println(adc.readSingleChannel(11));
delay(200);
}
