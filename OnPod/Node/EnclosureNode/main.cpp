#include <Arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "Timer.h"

#include "../../pod_internal_network.h"
#include "Paradigm.pb.h"

#include "drivers/node_ethernet.h"

const NodeType NODE_TYPE = ENCLOSURE;

Timer txTimer;

// comms
UDPClass udp(PIN_SPI_SS, ENCLOSURE_NODE_IP, ENCLOSURE_NODE_PORT, NODE_TYPE);
EnclosureNodeToFc pEnclosureNodeTelemetry = EnclosureNodeToFc_init_default;
uint32_t txPacketNum = 0;

void sendToFlightComputer(void*) {
    pEnclosureNodeTelemetry.packetNum = txPacketNum;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(
        udp.uSendBuffer, sizeof(udp.uSendBuffer)
    );
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, EnclosureNodeToFc_fields, &pEnclosureNodeTelemetry);
    if (udp.sendPacket(FC_IP, FC_ENCLOSURE_NODE_PORT, outStream.bytes_written)) {
        txPacketNum++;
    }
}

void setup() {
    Serial.begin(9600);
    udp.init();
    txTimer.every(ENCLOSURE_NODE_TO_FC_INTERVAL, &sendToFlightComputer, (void*)0);
}

void loop() {
    txTimer.update();  // check to see if it's time to send another packet

}
