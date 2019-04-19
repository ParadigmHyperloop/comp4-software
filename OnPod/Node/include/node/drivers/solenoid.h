#ifndef SOLENOID_H
#define SOLENOID_H

#include "solenoid_driver.h"

class Solenoid {
private:
    DRV8806 *driver;
    uint8_t uSolenoidChannel;
public:
    Solenoid(DRV8806 *driver, uint8_t uSolenoidChannel);
    void enable();
    void disable();
};

#endif
