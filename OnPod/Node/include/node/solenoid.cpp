#include "solenoid.h"

Solenoid::Solenoid(DRV8806 *driver, uint8_t uSolenoidChannel) :
    driver(driver),
    uSolenoidChannel(uSolenoidChannel)
    {}

void Solenoid::enable() {
    driver->enableSolenoid(uSolenoidChannel);
    driver->updateSolenoids();
    bState = 1;
}

void Solenoid::disable() {
    driver->disableSolenoid(uSolenoidChannel);
    driver->updateSolenoids();
    bState = 0;
}
