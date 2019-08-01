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
#include "sensors/atmo_transducer_HSC60P.h"
#include "sensors/atmo_temperature_TC74.h"
#include "sensors/lp_transducer_PX250P_arduino.h"

const NodeType NODE_TYPE = ENCLOSURE;

// timer for sending telemetry to the flight computer
Timer txTimer;

// instantiate all sensor objects
HSC60P atmoTransducer(0x38);
TC74 atmoTempSensor(0x4D);
ArduinoPX250P coolantTransducer1(A1);

UDPClass udp(10, ENCLOSURE_NODE_IP, ENCLOSURE_NODE_PORT, NODE_TYPE);
EnclosureNodeToFc pEnclosureNodeTelemetry = EnclosureNodeToFc_init_default;

void sendToFlightComputer(void*) {
    // read sensor values and store them in a protobuf object
    pEnclosureNodeTelemetry.enclosurePressure = atmoTransducer.read();
    pEnclosureNodeTelemetry.enclosureTemperature = atmoTempSensor.read();
    pEnclosureNodeTelemetry.coolantPressure1 = coolantTransducer1.read();
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, EnclosureNodeToFc_fields, &pEnclosureNodeTelemetry);
    udp.sendPacket(FC_IP, FC_ENCLOSURE_NODE_PORT, outStream.bytes_written);
}

void setup() {
    Serial.begin(9600);
    pinMode(53, OUTPUT);
    pinMode(10, OUTPUT);
    udp.init();

    txTimer.every(ENCLOSURE_NODE_TO_FC_INTERVAL, &sendToFlightComputer, (void*)0);
}

void loop() {
    // check to see if it's time to send another packet
    txTimer.update();
}
