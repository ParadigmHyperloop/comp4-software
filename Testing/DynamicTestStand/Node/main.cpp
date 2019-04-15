#include <Arduino.h>
#include "dts.h"
#include "solenoid.h"
#include "ethernet.h"
#include "adc.h"
#include "irtemp.h"

ADS7953 adc;
OS101E rotorTempSensor (adc, 15);

DRV8806 solenoidDriver;
Solenoid brakeSolenoid (solenoidDriver, 3);
Solenoid ventSolenoid (solenoidDriver, 4);

DTS dts;

void setup() {

}

void loop() {
    switch (dts.state) {
        case STATE_STANDBY:
            rotorTempSensor.readTemperature();
            break;
        case STATE_ACCELERATING:
            break;
        case STATE_BRAKING:
            break;
        case STATE_VENTING:
            break;
    }
}
