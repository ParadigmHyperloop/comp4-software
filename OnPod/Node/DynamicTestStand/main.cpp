#include <Arduino.h>

#include "dts.h"
#include "solenoid.h"
#include "ethernet.h"
#include "adc.h"
#include "ir_temp.h"

#include "Paradigm.pb.h"

ADS7953 adc;
OS101E rotorTempSensor (&adc, 15);

DRV8806 solenoidDriver;
Solenoid brakeSolenoid (&solenoidDriver, 3);
Solenoid ventSolenoid (&solenoidDriver, 4);

UDPClass udp (IPAddress(192,168,2,78), 777, 0);
FcToBrakeNode pFcCommand = FcToBrakeNode_init_zero;
DtsNodeToFc pBrakeNodeTelemetry = DtsNodeToFc_init_zero;

DTS dts;

void setup() {
    while (!Serial) {}
    adc.init();
    solenoidDriver.init();
}

void loop() {
    // pBrakeNodeTelemetry.rotorTemperature = 13;
    // bool status = pb_encode(&udp.outStream, DtsNodeToFc_fields, &pBrakeNodeTelemetry);
    // Serial.println(status);
    udp.cSendBuffer[0] = 0x0F;
    Serial.println(udp.cSendBuffer[0], BIN);
    udp.sendPacket(IPAddress(192, 168, 2, 18), 55019);
    delay(500);
    Serial.println("abc");
}
