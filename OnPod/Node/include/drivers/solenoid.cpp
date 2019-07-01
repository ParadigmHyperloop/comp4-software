#include "solenoid.h"

Solenoid::Solenoid(DRV8806 *driver, uint8_t uSolenoidChannel, SolenoidType eSolenoidType) :
    driver(driver),
    uSolenoidChannel(uSolenoidChannel),
    eSolenoidType(eSolenoidType)
    {}

void Solenoid::open() {
    if (eSolenoidType == NATURALLY_OPEN) {
        driver->disableSolenoid(uSolenoidChannel);
    }
    else if (eSolenoidType == NATURALLY_CLOSED) {
        driver->enableSolenoid(uSolenoidChannel);
    }
    driver->updateSolenoids();
    bState = true;
}

void Solenoid::close() {
    if (eSolenoidType == NATURALLY_OPEN) {
        driver->enableSolenoid(uSolenoidChannel);
    }
    else if (eSolenoidType == NATURALLY_CLOSED) {
        driver->disableSolenoid(uSolenoidChannel);
    }
    driver->updateSolenoids();
    bState = false;
}
