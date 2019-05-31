#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>

#include "drivers/adc_ADS7953.h"
#include "drivers/node_ethernet.h"
#include "drivers/solenoid_driver_DRV8806.h"
#include "drivers/solenoid.h"

#include "sensors/ir_temp_OS101E.h"
#include "sensors/thermocouple.h"
#include "sensors/hp_transducer_U5374.h"
#include "sensors/lp_transducer_PX2300P.h"

#include "Paradigm.pb.h"
#include "../../pod_internal_network.h"

const NodeType NODE_TYPE = BRAKE;
Timer txTimer;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI, SS1, POWER_SEQ_ADC);
U5374 hpTransducer(&adc, 6);
PX2300P coolantTransducer(&adc, 11);
PX2300P lpTransducer1(&adc, 8);
PX2300P lpTransducer2(&adc, 9);
PX2300P lpTransducer3(&adc, 10);
PX2300P lpTransducerCommon(&adc, 11);
TypeKThermo pneumaticThermo(&adc, 0);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);
Solenoid branch1Solenoid(&solenoidDriver, 0, NATURALLY_OPEN);
Solenoid branch2Solenoid(&solenoidDriver, 1, NATURALLY_OPEN);
Solenoid branch3Solenoid(&solenoidDriver, 7, NATURALLY_OPEN);
Solenoid ventSolenoid(&solenoidDriver, 8, NATURALLY_CLOSED);

// instantiate a UDP class
UDPClass udp (PIN_SPI_SS, BRAKE_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);
uint32_t txPacketNum = 0;
uint32_t rxPacketNum = 0;

// instantiate protobuf objects
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
BrakeNodeToFc pBrakeNodeTelemetry = BrakeNodeToFc_init_default;

void sendToFlightComputer(void*) {
    pBrakeNodeTelemetry.packetNum = txPacketNum;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, BrakeNodeToFc_fields, &pBrakeNodeTelemetry);
    if (udp.sendPacket(FC_IP, FC_BRAKE_NODE_PORT, outStream.bytes_written)) {
        txPacketNum++;
    }
}

void setup() {
    // initialize hardware
    adc.init();
    hpTransducer.init();
    coolantTransducer.init();
    lpTransducer1.init();
    lpTransducer2.init();
    lpTransducer3.init();
    lpTransducerCommon.init();
    pneumaticThermo.init();

    solenoidDriver.init();
    udp.init();
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if ((uint32_t)pFcCommand.packetNum > rxPacketNum) {
            pBrakeNodeTelemetry.state = pFcCommand.manualNodeState;
            rxPacketNum = pFcCommand.packetNum;
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
    pBrakeNodeTelemetry.coolantTankPressure = coolantTransducer.read();

    // perform state-specific operations
    switch (pBrakeNodeTelemetry.state) {
        case BrakeNodeStates_bnsBooting: {
            branch1Solenoid.open();
            branch2Solenoid.open();
            branch3Solenoid.open();
            ventSolenoid.close();
            break;
        }
        case BrakeNodeStates_bnsStandby: {
            branch1Solenoid.close();
            branch2Solenoid.close();
            branch3Solenoid.close();
            ventSolenoid.open();
            break;
        }
        case BrakeNodeStates_bnsFlight: {
            branch1Solenoid.close();
            branch2Solenoid.close();
            branch3Solenoid.close();
            ventSolenoid.close();
            break;
        }
        case BrakeNodeStates_bnsBraking: {
            branch1Solenoid.open();
            branch2Solenoid.open();
            branch3Solenoid.open();
            ventSolenoid.close();
            // branch cutoff logic here
            break;
        }
        case BrakeNodeStates_bnsSolenoidControl: {
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

    // update solenoid values
    pBrakeNodeTelemetry.solenoid1 = branch1Solenoid.bState;
    pBrakeNodeTelemetry.solenoid2 = branch2Solenoid.bState;
    pBrakeNodeTelemetry.solenoid3 = branch3Solenoid.bState;
    pBrakeNodeTelemetry.solenoid4 = ventSolenoid.bState;

    // send to FC is interval has expired
    txTimer.update();
}
