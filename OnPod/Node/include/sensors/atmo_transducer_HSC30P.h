#ifndef ATMO_TRANSDUCER_HSC30P_H
#define ATMO_TRANSDUCER_HSC30P_H

#include <Wire.h>

class HSC30P {
private:
    uint8_t uAddress;
    const uint8_t RESOLUTION_BITS = 14;
    const uint8_t MAX_PRESSURE = 30;
public:
    HSC30P(uint8_t uAddress);
    float read();
};

#endif
