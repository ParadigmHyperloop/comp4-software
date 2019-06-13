#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>

#include "drivers/adc_ADS7953.h"
#include "sensors/voltage_divider.h"
#include "sensors/current_sensor_ACS711.h"
#include "drivers/node_ethernet.h"

#include "Paradigm.pb.h"
#include "../../pod_internal_network.h"

const NodeType NODE_TYPE = LVDC;
Timer txTimer;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI(&PERIPH_SPI1, ADC_MISO, ADC_SCK, ADC_MOSI, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI, ADC_SS);
VoltageDivider hpBatVSense(&adc, 9, 900, 100);
VoltageDivider lpBatVSense(&adc, 8, 900, 100);
VoltageDivider lp24VSense(&adc, 12, 900, 120);
VoltageDivider hp12VSense(&adc, 13, 900, 250);
VoltageDivider lp12VSense(&adc, 11, 900, 250);
VoltageDivider lp5VSense(&adc, 10, 900, 620);
ACS711 hpBatCurrentSensor(&adc, 15);
ACS711 lpBatCurrentSensor(&adc, 14);
ACS711 lp12VCurrentSensor(&adc, 7);
ACS711 nodeCurrentSensor(&adc, 5);
ACS711 lp5VCurrentSensor(&adc, 4);
ACS711 coolingCurrentSensor1(&adc, 3);
ACS711 coolingCurrentSensor2(&adc, 2);
ACS711 inverterCurrentSensor(&adc, 6);

// instantiate a UDP class
UDPClass udp (W5500_SS, LVDC_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);
uint32_t txPacketNum = 0;
uint32_t rxPacketNum = 0;

// instantiate protobuf objects
FcToLvdcNode pFcCommand = FcToLvdcNode_init_default;
LvdcNodeToFc pLvdcNodeTelemetry = LvdcNodeToFc_init_default;
LvdcNodeStates lvdcNodeState = LvdcNodeStates_lvdcBooting;

void sendToFlightComputer(void*) {
    pLvdcNodeTelemetry.packetNum = txPacketNum;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, LvdcNodeToFc_fields, &pLvdcNodeTelemetry);
    if (udp.sendPacket(FC_IP, FC_LVDC_NODE_PORT, outStream.bytes_written)) {
        txPacketNum++;
    }
}

void setup() {
    // initialize hardware
    adc.init();
    udp.init();

    pinMode(LP5V_EN, OUTPUT);
    pinMode(LP12V_EN, OUTPUT);
    pinMode(HP12V_EN, OUTPUT);
    pinMode(LP24V_EN, OUTPUT);
    digitalWrite(LP5V_EN, true);
    digitalWrite(LP12V_EN, true);
    digitalWrite(HP12V_EN, true);
    digitalWrite(LP24V_EN, true);
    pinMode(INV_CTRL, OUTPUT);
    pinMode(PUMP1_CTL, OUTPUT);
    pinMode(PUMP2_CTL, OUTPUT);
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToLvdcNode_fields, &pFcCommand);
        if ((uint32_t)pFcCommand.packetNum > rxPacketNum) {
            pLvdcNodeTelemetry.state = pFcCommand.manualNodeState;
            rxPacketNum = pFcCommand.packetNum;
        }
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    adc.readActiveChannels();
    pLvdcNodeTelemetry.highPowerPackVoltage = hpBatVSense.read();
    pLvdcNodeTelemetry.lowPowerPackVoltage = lpBatVSense.read();
    pLvdcNodeTelemetry.highPowerPackCurrent = hpBatCurrentSensor.read();
    pLvdcNodeTelemetry.lowPowerPackCurrent = lpBatCurrentSensor.read();
    pLvdcNodeTelemetry.lowPower5Voltage = lp5VSense.read();
    pLvdcNodeTelemetry.lowPower12Voltage = lp12VSense.read();
    pLvdcNodeTelemetry.highPower12Voltage = hp12VSense.read();
    pLvdcNodeTelemetry.lowPower24Voltage = lp24VSense.read();
    pLvdcNodeTelemetry.lowPower5Current = lp5VCurrentSensor.read();
    pLvdcNodeTelemetry.lowPower12Current = lp12VCurrentSensor.read();
    pLvdcNodeTelemetry.nodeCurrent = nodeCurrentSensor.read();
    pLvdcNodeTelemetry.inverterCurrent = inverterCurrentSensor.read();
    pLvdcNodeTelemetry.cooling1Current = coolingCurrentSensor1.read();
    pLvdcNodeTelemetry.cooling2Current = coolingCurrentSensor2.read();

    // perform state-specific operations
    switch (pLvdcNodeTelemetry.state) {
        case LvdcNodeStates_lvdcBooting: {
            digitalWrite(INV_CTRL, false);
            digitalWrite(PUMP1_CTL, false);
            digitalWrite(PUMP2_CTL, false);
            break;
        }
        case LvdcNodeStates_lvdcStandby: {
            digitalWrite(INV_CTRL, false);
            digitalWrite(PUMP1_CTL, false);
            digitalWrite(PUMP2_CTL, false);
            break;
        }
        case LvdcNodeStates_lvdcFlight: {
            digitalWrite(INV_CTRL, true);
            digitalWrite(PUMP1_CTL, true);
            digitalWrite(PUMP2_CTL, true);
            break;
        }
    }

    // send to FC is interval has expired
    txTimer.update();
}
