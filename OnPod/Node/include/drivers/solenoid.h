#ifndef SOLENOID_H
#define SOLENOID_H

#include "drivers/solenoid_driver_DRV8806.h"

enum SolenoidType {
    NATURALLY_CLOSED = 0,
    NATURALLY_OPEN = 1,
};

class Solenoid {
private:
    DRV8806 *driver;
    uint8_t uSolenoidChannel;
    SolenoidType eSolenoidType;
public:
    Solenoid(DRV8806 *driver, uint8_t uSolenoidChannel, SolenoidType eSolenoidType);
    bool bState = 0;
    void open();
    void close();
};

#endif
