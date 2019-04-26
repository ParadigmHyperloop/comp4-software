#include <Arduino.h>

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
typeKThermo pneumaticTempSensor (&adc, 0);
MLH03KPSL01G tankTransducer (&adc, 8);
U5244 brakeTransducer (&adc, 10);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver;
Solenoid brakeSolenoid (&solenoidDriver, 3);
Solenoid ventSolenoid (&solenoidDriver, 4);

// instantiate a UDP class and protobuf message objects
UDPClass udp (IPAddress(192,168,0,3), 777, 0);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_zero;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_zero;

// the dts brake node state
BrakeNodeStates dtsState = BrakeNodeStates_bnsFlight;

void setup() {
    while (!Serial) {}  // wait for a serial connection to begin

    // initialize drivers
    adc.init();
    solenoidDriver.init();
    udp.init();
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket() && (dtsState != BrakeNodeStates_bnsError)) {
        pb_decode(&udp.inStream, FcToBrakeNode_fields, &pFcCommand);
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
            ventSolenoid.disable();
            break;
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    adc.readActiveChannels();
    pBrakeNodeTelemetry.brakeNodeState = dtsState;
    pBrakeNodeTelemetry.rotorTemperature = rotorTempSensor.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pBrakeNodeTelemetry.brakeSolenoidState = brakeSolenoid.bState;
    pBrakeNodeTelemetry.ventSolenoidState = ventSolenoid.bState;
    pBrakeNodeTelemetry.brakePressure = brakeTransducer.read();
    pBrakeNodeTelemetry.tankPressure = tankTransducer.read();

    // send the latest values to the flight computer
    pb_encode(&udp.outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(IPAddress(192, 168, 0, 2), 55019);
}
