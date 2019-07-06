#ifndef ATMO_TRANSDUCER_HSC60P_H
#define ATMO_TRANSDUCER_HSC60P_H

#include <Wire.h>

class HSC60P {
private:
    uint8_t uAddress;
    const uint8_t RESOLUTION_BITS = 14;
    const uint8_t MAX_PRESSURE = 60;
public:
    HSC60P(uint8_t uAddress);
    float read();
};

#endif
