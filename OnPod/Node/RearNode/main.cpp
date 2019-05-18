#include <Arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include <SPI.h>

#include "Sodaq_wdt.h"
#include "Timer.h"

#include "../../pod_internal_network.h"
#include "Paradigm.pb.h"

#include "drivers/node_ethernet.h"
#include "drivers/adc.h"
#include "sensors/thermocouple.h"

const NodeType NODE_TYPE = REAR;

Timer txTimer;

// sensors
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI);
<<<<<<< HEAD

=======
/*
>>>>>>> a2456829b838471dc614ac9505197677f8f1af36
typeKThermo thermo(&adc, 0);

// comms
UDPClass udp(PIN_SPI_SS, REAR_NODE_IP, REAR_NODE_PORT, NODE_TYPE);
RearNodeToFc pRearNodeTelemetry = RearNodeToFc_init_default;  // protobuf message
uint32_t txPacketNum = 0;

void sendToFlightComputer(void*) {
    pRearNodeTelemetry.packetNum = txPacketNum;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, RearNodeToFc_fields, &pRearNodeTelemetry);
    if (udp.sendPacket(FC_IP, FC_PORT, outStream.bytes_written)) {
        txPacketNum++;
    }
}
*/
void setup() {
    //udp.init();
    adc.init();

<<<<<<< HEAD
    thermo.init();

    txTimer.every(REAR_NODE_TO_FC_INTERVAL, &sendToFlightComputer, (void*)0);
=======
    //txTimer.every(TX_INTERVAL_MS, &sendToFlightComputer, (void*)0);
>>>>>>> a2456829b838471dc614ac9505197677f8f1af36
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
