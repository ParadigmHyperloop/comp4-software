/* Board: Node
 *
 * Brake Node main program and state machine.
 * Program that controls the sensors and brake solenoids on the pod.
 * - Communicates with the Flight Computer over UDP.
 * - Has Heartbeat timeout, TODO: Watchdog timer
 * - Drives solenoids, ir_temp, thermocouples and hp/lp transducers.
 */

#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>
#include <WDTZero.h>
#include "../../pod_internal_network.h"
#include "Paradigm.pb.h"

#include "drivers/adc_ADS7953.h"
#include "drivers/node_ethernet.h"
#include "drivers/solenoid_driver_DRV8806.h"
#include "drivers/solenoid.h"
#include "sensors/thermocouple.h"
#include "sensors/hp_transducer_U5374.h"
#include "sensors/lp_transducer_PX2300P.h"

const NodeType NODE_TYPE = BRAKE;
WDTZero internalWatchdog;
Timer txTimer;
Timer brakingToStandbyTimer;
bool brakingToStandbyTimerActive = false;
const uint16_t HEARTBEAT_INTERVAL = 100;
const uint16_t MINIMUM_BRAKE_TIMER = 25000;
const uint16_t BRAKING_TO_STANDBY_INTERVAL = 30000;
Timer fcHeartbeatTimer;
bool heartBeatExpired = false;
int8_t timerEventNumber = 0;
Timer minimumBrakeTimer;
bool brakingTimerActive = false;
bool firstTimeBraking = true;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI, SS1, POWER_SEQ_ADC);
U5374 hpTransducer(&adc, 6);
PX2300P lpTransducer1(&adc, 8);
PX2300P lpTransducer2(&adc, 11);
PX2300P lpTransducer3(&adc, 10);
PX2300P lpTransducerCommon(&adc, 9);
TypeKThermo pneumaticThermo(&adc, 1);
TypeKThermo coolingThermo(&adc, 0);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);
Solenoid branch1Solenoid(&solenoidDriver, 2, NATURALLY_OPEN);
Solenoid branch2Solenoid(&solenoidDriver, 3, NATURALLY_OPEN);
Solenoid branch3Solenoid(&solenoidDriver, 4, NATURALLY_OPEN);
Solenoid ventSolenoid(&solenoidDriver, 5, NATURALLY_CLOSED);

// instantiate a UDP class
UDPClass udp (PIN_SPI_SS, BRAKE_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);

// instantiate protobuf objects
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
BrakeNodeToFc pBrakeNodeTelemetry = BrakeNodeToFc_init_default;

void expireHeartbeat(void*) {
    heartBeatExpired = true;
}

void expireBrakeTimer(void*) {
    brakingTimerActive = false;
}

void expireStandbyTimer(void*) {
    brakingToStandbyTimerActive = false;
    pBrakeNodeTelemetry.state = BrakeNodeStates_bnsStandby;
}

void sendToFlightComputer(void*) {
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, BrakeNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(FC_IP, FC_BRAKE_NODE_PORT, outStream.bytes_written);
}

void setup() {
    Serial.begin(9600);
    // initialize hardware
    pinMode(INVERTER_EN, OUTPUT);
    adc.init();
    hpTransducer.init();
    lpTransducer1.init();
    lpTransducer2.init();
    lpTransducer3.init();
    lpTransducerCommon.init();
    pneumaticThermo.init();
    coolingThermo.init();
    solenoidDriver.init();

    pBrakeNodeTelemetry.state = BrakeNodeStates_bnsBooting;
    udp.init();
    txTimer.every(BRAKE_NODE_TO_FC_INTERVAL, &sendToFlightComputer, (void*)0);
//    internalWatchdog.setup(WDT_HARDCYCLE1S);
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        heartBeatExpired = false;
        fcHeartbeatTimer.stop(timerEventNumber);
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);

        switch (pBrakeNodeTelemetry.state) {
            case BrakeNodeStates_bnsBooting: {
                if (pFcCommand.nodeState == BrakeNodeStates_bnsStandby ||
                    pFcCommand.nodeState == BrakeNodeStates_bnsBraking) {
                    pBrakeNodeTelemetry.state = pFcCommand.nodeState;
                    break;
                }
            }
            case BrakeNodeStates_bnsStandby: {
                if (pFcCommand.nodeState == BrakeNodeStates_bnsFlight ||
                    pFcCommand.nodeState == BrakeNodeStates_bnsSolenoidControl) {
                    pBrakeNodeTelemetry.state = pFcCommand.nodeState;
                    break;
                }
            }
            case BrakeNodeStates_bnsFlight: {
                if (pFcCommand.nodeState == BrakeNodeStates_bnsBraking) {
                    pBrakeNodeTelemetry.state = pFcCommand.nodeState;
                    break;
                }
            }
            case BrakeNodeStates_bnsBraking: {
                if (pFcCommand.nodeState == BrakeNodeStates_bnsStandby) {
                    pBrakeNodeTelemetry.state = pFcCommand.nodeState;
                    break;
                }
            }
            case BrakeNodeStates_bnsSolenoidControl: {
                if (pFcCommand.nodeState == BrakeNodeStates_bnsStandby) {
                    pBrakeNodeTelemetry.state = pFcCommand.nodeState;
                    break;
                }
            }
        }
        if (brakingTimerActive) {
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsBraking;
        }
        timerEventNumber = fcHeartbeatTimer.after(HEARTBEAT_INTERVAL, expireHeartbeat, (void*)0);
    }

    // perform state-specific operations
    switch (pBrakeNodeTelemetry.state) {
        case BrakeNodeStates_bnsBooting: {
            firstTimeBraking = true;
            digitalWrite(INVERTER_EN, false);
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsBooting;
            branch1Solenoid.open();
            branch2Solenoid.open();
            branch3Solenoid.open();
            ventSolenoid.close();
            break;
        }

        case BrakeNodeStates_bnsStandby: {
            firstTimeBraking = true;
            digitalWrite(INVERTER_EN, true);
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsStandby;
            branch1Solenoid.close();
            branch2Solenoid.close();
            branch3Solenoid.close();
            ventSolenoid.open();
            break;
        }

        case BrakeNodeStates_bnsFlight: {
            firstTimeBraking = true;
            digitalWrite(INVERTER_EN, true);
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsFlight;
            branch1Solenoid.close();
            branch2Solenoid.close();
            branch3Solenoid.close();
            ventSolenoid.close();
            if (heartBeatExpired) {
                pBrakeNodeTelemetry.state = BrakeNodeStates_bnsBraking;
                pinMode(INVERTER_EN, OUTPUT);
                digitalWrite(INVERTER_EN, false);
                branch1Solenoid.open();
                branch2Solenoid.open();
                branch3Solenoid.open();
                ventSolenoid.close();
            }
            break;
        }

        case BrakeNodeStates_bnsBraking: {
            digitalWrite(INVERTER_EN, false);
            if (firstTimeBraking) {
                minimumBrakeTimer.after(5000, expireBrakeTimer, (void*)0);
                brakingTimerActive = true;
                firstTimeBraking = false;
            }
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsBraking;
            branch1Solenoid.open();
            branch2Solenoid.open();
            branch3Solenoid.open();
            ventSolenoid.close();
            if (heartBeatExpired && !brakingToStandbyTimerActive) {
                brakingToStandbyTimerActive = true;
                brakingToStandbyTimer.after(BRAKING_TO_STANDBY_INTERVAL, expireStandbyTimer, (void*)0);
            }
            break;
        }

        case BrakeNodeStates_bnsSolenoidControl: {
            digitalWrite(INVERTER_EN, false);
            pBrakeNodeTelemetry.state = BrakeNodeStates_bnsSolenoidControl;
            if (pFcCommand.solenoid1Config) {
                branch1Solenoid.open();
            } else {
                branch1Solenoid.close();
            }
            if (pFcCommand.solenoid2Config) {
                branch2Solenoid.open();
            } else {
                branch2Solenoid.close();
            }
            if (pFcCommand.solenoid3Config) {
                branch3Solenoid.open();
            } else {
                branch3Solenoid.close();
            }
            if (pFcCommand.solenoid4Config) {
                ventSolenoid.open();
            } else {
                ventSolenoid.close();
            }
            break;
        }
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    adc.readActiveChannels();
    pBrakeNodeTelemetry.highPressure = hpTransducer.read();
    pBrakeNodeTelemetry.lowPressure1 = lpTransducer1.read();
    pBrakeNodeTelemetry.lowPressure2 = lpTransducer2.read();
    pBrakeNodeTelemetry.lowPressure3 = lpTransducer3.read();
    pBrakeNodeTelemetry.lowPressureCommon = lpTransducerCommon.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticThermo.read();
    pBrakeNodeTelemetry.coolantTemperature = coolingThermo.read();
    // update solenoid values
    pBrakeNodeTelemetry.solenoid1 = branch1Solenoid.bState;
    pBrakeNodeTelemetry.solenoid2 = branch2Solenoid.bState;
    pBrakeNodeTelemetry.solenoid3 = branch3Solenoid.bState;
    pBrakeNodeTelemetry.solenoid4 = ventSolenoid.bState;

    txTimer.update(); // send to FC if interval has expired
    fcHeartbeatTimer.update(); // expire heartbeat if no UDP
    minimumBrakeTimer.update();
    brakingToStandbyTimer.update();
    //internalWatchdog.clear();
}
