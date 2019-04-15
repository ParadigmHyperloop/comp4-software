#include <Arduino.h>
#include "solenoid.h"

DRV8806 solenoidDriver(SOL_CS, SOL_DOUT, SOL_DIN, SOL_CLK);

void setup() {
    solenoidDriver.init();
}

void loop() {
    solenoidDriver.enableSolenoid(5);
    solenoidDriver.updateSolenoids();
    delay(1000);

    solenoidDriver.disableSolenoid(5);
    solenoidDriver.updateSolenoids();
    delay(1000);
}
