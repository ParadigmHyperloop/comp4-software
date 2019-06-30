/* Board: Arduino Mega
 *
 * Encolsure Node program that transmits data to
 * the Flight Computer over UDP.
 */
#include <Arduino.h>
#include <pb_encode.h>
#include <pb_decode.h>

#include "Timer.h"

#include "../../pod_internal_network.h"
#include "Paradigm.pb.h"

#include "drivers/node_ethernet.h"
#include "sensors/atmo_transducer_HSC30P.h"
#include "sensors/atmo_temperature_TC74.h"
#include "sensors/lp_transducer_PX250P_arduino.h"

const NodeType NODE_TYPE = ENCLOSURE;

Timer txTimer;
HSC30P atmoTransducer(0x28);
TC74 atmoTempSensor(0x4D);
ArduinoPX250P coolantTransducer1(A2);
ArduinoPX250P coolantTransducer2(A3);

// comms
UDPClass udp(10, ENCLOSURE_NODE_IP, ENCLOSURE_NODE_PORT, NODE_TYPE);
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
    pEnclosureNodeTelemetry.enclosurePressure = atmoTransducer.read();
    pEnclosureNodeTelemetry.enclosureTemperature = atmoTempSensor.read();
    pEnclosureNodeTelemetry.coolantPressure1 = coolantTransducer1.read();
    pEnclosureNodeTelemetry.coolantPressure2 = coolantTransducer2.read();
    txTimer.update();  // check to see if it's time to send another packet
}
