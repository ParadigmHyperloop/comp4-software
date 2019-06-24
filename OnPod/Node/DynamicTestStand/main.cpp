/** Dynamic Test Stand Node main program and state machine.
 * Program that controls the sensors and brake solenoids.
 * 
 * Specific for use with DTS, not on full pod!
 **/

#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>

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
OS101E rotorTempSensor (&adc, 15);
TypeKThermo pneumaticTempSensor (&adc, 1);
U5374 tankTransducer (&adc, 7);
PX2300P brakeTransducer (&adc, 10);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);
Solenoid brakeSolenoid (&solenoidDriver, 3, NATURALLY_OPEN);
Solenoid ventSolenoid (&solenoidDriver, 4, NATURALLY_CLOSED);

// instantiate a UDP class and protobuf message objects
UDPClass udp (PIN_SPI_SS, BRAKE_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);
uint32_t txPacketNum = 0;
uint32_t rxPacketNum = 0;

// instantiate protobuf objects
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
DtsNodeToFc pDtsNodeTelemetry = DtsNodeToFc_init_default;
BrakeNodeStates dtsState;

void sendToFlightComputer(void*) {
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, DtsNodeToFc_fields, &pDtsNodeTelemetry);
    udp.sendPacket(FC_IP, FC_BRAKE_NODE_PORT, outStream.bytes_written);
}

void setup() {
    // initialize hardware
    adc.init();
    rotorTempSensor.init();
    pneumaticTempSensor.init();
    tankTransducer.init();
    brakeTransducer.init();

    solenoidDriver.init();
    udp.init();
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket() && (dtsState != BrakeNodeStates_bnsError)) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            dtsState = pFcCommand.manualNodeState;
        }
    }

    // update sensor values and hold them in the pDtsNodeTelemetry message object
    pDtsNodeTelemetry.brakeNodeState = dtsState;
    adc.readActiveChannels();
    pDtsNodeTelemetry.rotorTemperature = rotorTempSensor.read();
    pDtsNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pDtsNodeTelemetry.brakePressure = brakeTransducer.read();
    pDtsNodeTelemetry.tankPressure = tankTransducer.read();

    // perform state-specific operations
    switch (dtsState) {
        case BrakeNodeStates_bnsFlight:
            brakeSolenoid.close();
            ventSolenoid.close();
            break;
        case BrakeNodeStates_bnsBraking:
            brakeSolenoid.open();
            ventSolenoid.close();
            if (pDtsNodeTelemetry.rotorTemperature > 500) {
                dtsState = BrakeNodeStates_bnsError;
            }
            break;
        case BrakeNodeStates_bnsVenting:
            brakeSolenoid.close();
            ventSolenoid.open();
            break;
        case BrakeNodeStates_bnsError:
            brakeSolenoid.close();
            ventSolenoid.open();
            break;
    }

    // update solenoid values
    pDtsNodeTelemetry.brakeSolenoidState = brakeSolenoid.bState;
    pDtsNodeTelemetry.ventSolenoidState = ventSolenoid.bState;

    // send to FC is interval has expired
    txTimer.update();
}
