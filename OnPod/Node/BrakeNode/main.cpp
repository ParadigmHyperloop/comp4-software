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
ADS7953 adc(adcSPI, SS1, POWER_SEQ_ADC);


// instantiate solenoid driver and all solenoids
DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);


// instantiate a UDP class and protobuf message objects
NodeType nodeType = BRAKE;
//UDPClass udp (PIN_SPI_SS, BRAKE_NODE_IP, BRAKE_NODE_PORT, nodeType);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_default;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_default;
BrakeNodeStates dtsState;

void setup() {
    // initialize hardware
    adc.init();

    solenoidDriver.init();
    //udp.init();
}

void loop() {
    /*
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


    // perform state-specific operations
    switch (dtsState) {

    }

    // send the latest values to the flight computer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    pb_encode(&outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    udp.sendPacket(FC_IP, FC_PORT, outStream.bytes_written);
    */

    Serial.println(adc.readSingleChannel(0));
    delay(200);
}
