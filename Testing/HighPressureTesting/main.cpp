#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>

#include "Paradigm.pb.h"
#include "../../pod_internal_network.h"

#include "drivers/node_ethernet.h"
#include "drivers/solenoid_driver_DRV8806.h"
#include "drivers/solenoid.h"
#include "drivers/adc_ADS7953.h"
#include "sensors/ir_temp_OS101E.h"
#include "sensors/thermocouple.h"
#include "sensors/hp_transducer_U5374.h"
#include "sensors/lp_transducer_PX2300P.h"

const NodeType NODE_TYPE = BRAKE;
Timer txTimer;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI(&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI, SS1, POWER_SEQ_ADC);
TypeKThermo pneumaticTempSensor (&adc, 1);
U5374 tankTransducer (&adc, 6);
OS101E irTempSensor (&adc, 11);
PX2300P lowPressureTransducer (&adc, 11);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);
Solenoid ventSolenoid (&solenoidDriver, 3, NATURALLY_OPEN);

// instantiate a UDP class and protobuf message objects
UDPClass udp (PIN_SPI_SS, BRAKE_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_default;

// the dts brake node state
BrakeNodeStates testState;

void sendToFlightComputer(void*) {
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(FC_IP, FC_BRAKE_NODE_PORT, outStream.bytes_written);
}

void setup() {
    // initialize hardware
    adc.init();
    pneumaticTempSensor.init();
    tankTransducer.init();
    irTempSensor.init();
    lowPressureTransducer.init();

    solenoidDriver.init();
    udp.init();

    txTimer.every(BRAKE_NODE_TO_FC_INTERVAL, &sendToFlightComputer, (void*)0);
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            testState = pFcCommand.manualNodeState;
        }
    }

    // perform state-specific operations
    switch (testState) {
        case BrakeNodeStates_bnsFlight:
            if (ventSolenoid.bState) {
                ventSolenoid.close();
            }
            break;
        case BrakeNodeStates_bnsBraking:
            if (!ventSolenoid.bState) {
                ventSolenoid.open();
            }
            break;
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    pBrakeNodeTelemetry.brakeNodeState = testState;
    adc.readActiveChannels();
    pBrakeNodeTelemetry.rotorTemperature = irTempSensor.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pBrakeNodeTelemetry.ventSolenoidState = ventSolenoid.bState;
    pBrakeNodeTelemetry.brakePressure = lowPressureTransducer.read();
    pBrakeNodeTelemetry.tankPressure = tankTransducer.read();

    txTimer.update();
}
