#include <Arduino.h>
#include "solenoid.h"

DRV8806 solenoidDriver(SS2, MOSI2, MISO2, SCK2);

void setup() {
    Serial.begin(9600);
    solenoidDriver.init();
}

void loop() {
    solenoidDriver.enableSolenoid(5);
    solenoidDriver.enableSolenoid(7);
    solenoidDriver.updateSolenoids();
    delay(1000);

    solenoidDriver.disableSolenoid(5);
    solenoidDriver.disableSolenoid(7);
    solenoidDriver.updateSolenoids();
    delay(1000);
}
