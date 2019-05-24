#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>

#include "drivers/adc.h"
#include "sensors/ir_temp.h"
#include "sensors/thermocouple.h"
#include "sensors/hp_transducer.h"
#include "sensors/lp_transducer.h"

#include "drivers/solenoid_driver.h"
#include "drivers/solenoid.h"

#include "Paradigm.pb.h"
#include "drivers/node_ethernet.h"

Timer txTimer;
const uint32_t TX_INTERVAL_MS = 20;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI);
typeKThermo pneumaticTempSensor (&adc, 1);
U5374 tankTransducer (&adc, 6);
OS101E irTempSensor (&adc, 11);
PX2300P lowPressureTransducer (&adc, 11);

// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver;
Solenoid ventSolenoid (&solenoidDriver, 3);

// instantiate a UDP class and protobuf message objects
UDPClass udp (PIN_SPI_SS, IPAddress(192, 168, 0, 50), 5000, 0);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_default;

// the dts brake node state
BrakeNodeStates dtsState;

void sendToFlightComputer(void*) {
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(IPAddress(192, 168, 0, 100), 5555, outStream.bytes_written);
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

    txTimer.every(TX_INTERVAL_MS, &sendToFlightComputer, (void*)0);
}

void loop() {
    txTimer.update();

    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToBrakeNode_fields, &pFcCommand);
        if (pFcCommand.has_manualNodeState) {
            dtsState = pFcCommand.manualNodeState;
            Serial.println(dtsState);
        }
    }

    // perform state-specific operations
    switch (dtsState) {
        case BrakeNodeStates_bnsFlight:
            if (!ventSolenoid.bState) {
                ventSolenoid.enable();
                Serial.println("sol flight");
            }
            break;
        case BrakeNodeStates_bnsVenting:
            if (ventSolenoid.bState) {
                ventSolenoid.disable();
                Serial.println("sol vent");
            }
            break;
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    pBrakeNodeTelemetry.brakeNodeState = dtsState;
    adc.readActiveChannels();
    pBrakeNodeTelemetry.rotorTemperature = irTempSensor.read();
    pBrakeNodeTelemetry.pneumaticTemperature = pneumaticTempSensor.read();
    pBrakeNodeTelemetry.ventSolenoidState = ventSolenoid.bState;
    pBrakeNodeTelemetry.brakePressure = lowPressureTransducer.read();
    pBrakeNodeTelemetry.tankPressure = tankTransducer.read();
}
