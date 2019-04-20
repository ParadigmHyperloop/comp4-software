#include <Arduino.h>

#include "node/drivers/adc.h"
#include "node/sensors/ir_temp.h"
#include "node/sensors/thermocouple.h"
#include "node/sensors/hp_transducer.h"
#include "node/sensors/lp_transducer.h"

#include "node/drivers/solenoid_driver.h"
#include "node/drivers/solenoid.h"

#include "Paradigm.pb.h"
#include "ethernet.h"

// instantiate adc and all sensors with which it interfaces
ADS7953 adc;
OS101E rotorTempSensor (&adc, 15);
THERMO_NAME pneumaticTempSensor (&adc, 0);
HP_NAME tankTransducer (&adc, 8);
LP_NAME brakeTransducer (&adc, 10);

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

    // initialize drivers / classes
    adc.init();
    solenoidDriver.init();
    udp.init();
}

void loop() {
    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    pBrakeNodeTelemetry.rotorTemperature = rotorTempSensor.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pBrakeNodeTelemetry.brakeSolenoidState = brakeSolenoid.bState;
    pBrakeNodeTelemetry.ventSolenoidState = ventSolenoid.bState;
    pBrakeNodeTelemetry.brakePressure = brakeTransducer.read();
    pBrakeNodeTelemetry.tankPressure = tankTransducer.read();

    // perform state-specific operations
    switch (dtsState) {
        case BrakeNodeStates_bnsFlight:
            brakeSolenoid.disable();
            ventSolenoid.disable();
            break;
        case BrakeNodeStates_bnsBraking:
            brakeSolenoid.enable();
            ventSolenoid.disable();
<<<<<<< HEAD
            if (pBrakeNodeTelemetry.rotorTemperature > 500) {
=======
            if (pBrakeNodeTelemtry > 500) {
>>>>>>> d0c1338d218722e8f5e55dd0b1d90dddb3781c3e
                dtsState = BrakeNodeStates_bnsFlight;
            }
            break;
        case BrakeNodeStates_bnsVenting:
            brakeSolenoid.disable();
            ventSolenoid.enable();
            break;
        case BrakeNodeStates_bnsError:
            // uhhhh
            break;
    }

    // check for incoming telemetry and set the state accordingly
    if (udp.readPacket()) {
        pb_decode(&udp.inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            dtsState = pFcCommand.manualNodeState;
        }
    }

    // send the latest values to the flight computer
    pb_encode(&udp.outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(IPAddress(192, 168, 0, 2), 55019);
}
