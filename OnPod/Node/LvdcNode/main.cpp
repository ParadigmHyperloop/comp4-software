/* Board: LVDC Node
 *
 * LVDC Node program with state machine.
 * Receives commands from Flight Computer and
 * transmits back LVDC sensor values over UDP.
 */
#include <Arduino.h>

#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>
#include <WDTZero.h>
#include "../../pod_internal_network.h"
#include "Paradigm.pb.h"

#include "drivers/adc_ADS7953.h"
#include "sensors/voltage_divider.h"
#include "sensors/current_sensor_ACS711.h"
#include "drivers/node_ethernet.h"

const NodeType NODE_TYPE = LVDC;

Timer txTimer;
Timer fcHeartbeatTimer;
bool heartBeatExpired = false;
const uint16_t HEARTBEAT_INTERVAL = 100;
int8_t timerEventNumber = 0;
WDTZero internalWatchdog;

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

// instantiate protobuf objects
FcToLvdcNode   pFcCommand         = FcToLvdcNode_init_default;
LvdcNodeToFc   pLvdcNodeTelemetry = LvdcNodeToFc_init_default;
LvdcNodeStates lvdcNodeState      = LvdcNodeStates_lvdcBooting;

void expireHeartbeatTimer(void*) {
    heartBeatExpired = true;
}

void sendToFlightComputer(void*) {

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    pLvdcNodeTelemetry.highPowerPackVoltage = (float)analogRead(AIN_3)/1023*33;
    pLvdcNodeTelemetry.lowPowerPackVoltage = (float)analogRead(AIN_2)/1023*33;
    if (((float)analogRead(AIN_1)/1023*3.267-1.633)/1.633*31-0.1 < 0.3) {
        pLvdcNodeTelemetry.highPowerPackCurrent = 0;
    }
    else {
        pLvdcNodeTelemetry.highPowerPackCurrent = ((float)analogRead(AIN_1)/1023*3.267-1.633)/1.633*31-0.1;
    }
    pLvdcNodeTelemetry.lowPowerPackCurrent = ((float)analogRead(AIN_0)/1023*3.267-1.633)/1.633*31;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, LvdcNodeToFc_fields, &pLvdcNodeTelemetry);
    udp.sendPacket(FC_IP, FC_LVDC_NODE_PORT, outStream.bytes_written);
}

void setup() {
    // initialize hardware
    adc.init();
    udp.init();

    hpBatVSense.init();
    lpBatVSense.init();
    lp24VSense.init();
    hp12VSense.init();
    lp12VSense.init();
    lp5VSense.init();
    hpBatCurrentSensor.init();
    lpBatCurrentSensor.init();
    lp12VCurrentSensor.init();
    nodeCurrentSensor.init();
    lp5VCurrentSensor.init();
    coolingCurrentSensor1.init();
    coolingCurrentSensor2.init();
    inverterCurrentSensor.init();

    pinMode(AIN_0, INPUT);
    pinMode(AIN_1, INPUT);
    pinMode(AIN_2, INPUT);
    pinMode(AIN_3, INPUT);

    pinMode(LP5V_EN, OUTPUT);
    pinMode(LP12V_EN, OUTPUT);
    pinMode(HP12V_EN, OUTPUT);
    pinMode(LP24V_EN, OUTPUT);
    digitalWrite(LP5V_EN, true);
    digitalWrite(LP12V_EN, true);
    digitalWrite(HP12V_EN, true);
    digitalWrite(LP24V_EN, true);
    pinMode(INV_CTL, OUTPUT);
    pinMode(PUMP1_CTL, OUTPUT);
    pinMode(PUMP2_CTL, OUTPUT);

    //internalWatchdog.setup(WDT_HARDCYCLE1S);
    pLvdcNodeTelemetry.state = LvdcNodeStates_lvdcBooting;
    txTimer.every(LVDC_NODE_TO_FC_INTERVAL, sendToFlightComputer, (void*)0);
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        heartBeatExpired = false;
        fcHeartbeatTimer.stop(timerEventNumber);
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToLvdcNode_fields, &pFcCommand);
        pLvdcNodeTelemetry.state = pFcCommand.nodeState;
        timerEventNumber = fcHeartbeatTimer.after(HEARTBEAT_INTERVAL, expireHeartbeatTimer, (void*)0);
    }


    // perform state-specific operations
    switch (pLvdcNodeTelemetry.state) {
        case LvdcNodeStates_lvdcBooting: {
            digitalWrite(INV_CTL, false);
            digitalWrite(PUMP1_CTL, false);
            digitalWrite(PUMP2_CTL, false);
            break;
        }
        case LvdcNodeStates_lvdcStandby: {
            digitalWrite(INV_CTL, false);
            digitalWrite(PUMP1_CTL, false);
            digitalWrite(PUMP2_CTL, false);
            break;
        }
        case LvdcNodeStates_lvdcFlight: {
            digitalWrite(INV_CTL, true);
            digitalWrite(PUMP1_CTL, true);
            digitalWrite(PUMP2_CTL, true);
            if (heartBeatExpired) {
                pLvdcNodeTelemetry.state = LvdcNodeStates_lvdcStandby;
            }
            break;
        }
    }
    // send to FC is interval has expired
    txTimer.update();
    fcHeartbeatTimer.update();
    //internalWatchdog.clear();
}
