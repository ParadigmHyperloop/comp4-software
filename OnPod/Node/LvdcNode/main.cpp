#include <Arduino.h>
#include <pb_decode.h>
#include <pb_encode.h>
#include <Timer.h>

#include "drivers/adc_ADS7953.h"
#include "sensors/voltage_divider.h"
#include "drivers/node_ethernet.h"

#include "Paradigm.pb.h"
#include "../../pod_internal_network.h"

const NodeType NODE_TYPE = LVDC;
Timer txTimer;

// instantiate adc and all sensors with which it interfaces
SPIClass adcSPI(&PERIPH_SPI1, ADC_MISO, ADC_SCK, ADC_MOSI, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI, ADC_SS);
VoltageDivider HPBAT_VSense(&adc, 9, 1, 1);
VoltageDivider LPBAT_VSense(&adc, 8, 1, 1);
VoltageDivider LP24V_VSense(&adc, 12, 1, 1);
VoltageDivider HP12V_VSense(&adc, 13, 1, 1);
VoltageDivider LP12V_VSense(&adc, 11, 1, 1);
VoltageDivider LP5V_VSense(&adc, 10, 1, 1);

// instantiate a UDP class
UDPClass udp (W5500_SS, LVDC_NODE_IP, BRAKE_NODE_PORT, NODE_TYPE);
uint32_t txPacketNum = 0;
uint32_t rxPacketNum = 0;

// instantiate protobuf objects
FcToLvdcNode pFcCommand = FcToLvdcNode_init_default;
LvdcNodeToFc pLvdcNodeTelemetry = LvdcNodeToFc_init_default;
LvdcNodeStates lvdcNodeState = LvdcNodeStates_lvdcBooting;

void sendToFlightComputer(void*) {
    pLvdcNodeTelemetry.packetNum = txPacketNum;
    // create an output stream that writes to the UDP buffer
    pb_ostream_t outStream = pb_ostream_from_buffer(udp.uSendBuffer, sizeof(udp.uSendBuffer));
    // encode the message object and store it in the UDP buffer
    pb_encode(&outStream, LvdcNodeToFc_fields, &pLvdcNodeTelemetry);
    if (udp.sendPacket(FC_IP, FC_LVDC_NODE_PORT, outStream.bytes_written)) {
        txPacketNum++;
    }
}

void setup() {
    // initialize hardware
    adc.init();
    udp.init();
}

void loop() {
    // check for incoming telemetry and set the state accordingly (unless error)
    if (udp.readPacket()) {
        pb_istream_t inStream = pb_istream_from_buffer(udp.uRecvBuffer, sizeof(udp.uRecvBuffer));
        pb_decode(&inStream, FcToLvdcNode_fields, &pFcCommand);
        if ((uint32_t)pFcCommand.packetNum > rxPacketNum) {
            pLvdcNodeTelemetry.state = pFcCommand.manualNodeState;
            rxPacketNum = pFcCommand.packetNum;
        }
    }

    // update sensor values and hold them in the pBrakeNodeTelemetry message object
    adc.readActiveChannels();


    // perform state-specific operations
    switch (pLvdcNodeTelemetry.state) {

    }

    // send to FC is interval has expired
    txTimer.update();
}
