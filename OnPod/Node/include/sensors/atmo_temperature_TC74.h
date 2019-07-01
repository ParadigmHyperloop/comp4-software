#ifndef ATMO_TEMPERATURE_TC74_H
#define ATMO_TEMPERATURE_TC74_H

#include <Wire.h>

class TC74 {
private:
    uint8_t uAddress;
public:
    TC74(uint8_t uAddress);
    float read();
};

#endif
