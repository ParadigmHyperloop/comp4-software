#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>

#include "drivers/adc.h"
#include "sensors/ir_temp.h"
#include "sensors/thermocouple.h"
#include "sensors/hp_transducer.h"
#include "sensors/lp_transducer.h"

#include "drivers/solenoid_driver.h"
#include "drivers/solenoid.h"

#include "Paradigm.pb.h"
#include "drivers/ethernet.h"

// instantiate adc and all sensors with which it interfaces
ADS7953 adc;
OS101E rotorTempSensor (&adc, 15);
typeKThermo pneumaticTempSensor (&adc, 1);
U5374 tankTransducer (&adc, 7);
U5244 brakeTransducer (&adc, 10);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver;
Solenoid brakeSolenoid (&solenoidDriver, 3);
Solenoid ventSolenoid (&solenoidDriver, 4);

// instantiate a UDP class and protobuf message objects
UDPClass udp (IPAddress(192, 168, 2, 50), 5555, 0);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_default;

// the dts brake node state
BrakeNodeStates dtsState;

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
        pb_istream_t inStream = pb_istream_from_buffer((uint8_t*)udp.cRecvBuffer, sizeof(udp.cRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            dtsState = pFcCommand.manualNodeState;
        }
    }

    // perform state-specific operations
    switch (dtsState) {
        case BrakeNodeStates_bnsFlight:
            brakeSolenoid.enable();
            ventSolenoid.disable();
            break;
        case BrakeNodeStates_bnsBraking:
            ventSolenoid.disable();
            brakeSolenoid.disable();
            if (pBrakeNodeTelemetry.rotorTemperature > 500) {
                dtsState = BrakeNodeStates_bnsError;
            }
            break;
        case BrakeNodeStates_bnsVenting:
            brakeSolenoid.enable();
            ventSolenoid.enable();
            break;
        case BrakeNodeStates_bnsError:
            brakeSolenoid.enable();
            ventSolenoid.enable();
            break;
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    pBrakeNodeTelemetry.brakeNodeState = dtsState;
    adc.readActiveChannels();
    pBrakeNodeTelemetry.rotorTemperature = rotorTempSensor.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pBrakeNodeTelemetry.brakeSolenoidState = brakeSolenoid.bState;
    pBrakeNodeTelemetry.ventSolenoidState = ventSolenoid.bState;
    pBrakeNodeTelemetry.brakePressure = brakeTransducer.read();
    pBrakeNodeTelemetry.tankPressure = tankTransducer.read();

    // send the latest values to the flight computer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.cSendBuffer, sizeof(udp.cSendBuffer));
    pb_encode(&outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(IPAddress(192, 168, 2, 27), 5555, outStream.bytes_written);

    //delay(500);
}
