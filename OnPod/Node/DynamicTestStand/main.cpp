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

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI);
OS101E rotorTempSensor (&adc, 15);
typeKThermo pneumaticTempSensor (&adc, 1);
U5374 tankTransducer (&adc, 7);
PX2300P brakeTransducer (&adc, 10);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver;
Solenoid brakeSolenoid (&solenoidDriver, 3);
Solenoid ventSolenoid (&solenoidDriver, 4);

// instantiate a UDP class and protobuf message objects
UDPClass udp (PIN_SPI_SS, IPAddress(192, 168, 2, 50), 5555, 0);
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
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            dtsState = pFcCommand.manualNodeState;
        }
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

    // send the latest values to the flight computer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    pb_encode(&outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(IPAddress(192, 168, 2, 27), 5555, outStream.bytes_written);

    Serial.println(pBrakeNodeTelemetry.pneumaticTemperature);

    delay(500);

}
