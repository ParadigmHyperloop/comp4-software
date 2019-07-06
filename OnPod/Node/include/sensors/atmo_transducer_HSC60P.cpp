#include <Arduino.h>
#include "atmo_transducer_HSC60P.h"

HSC60P::HSC60P(uint8_t uAddress) : uAddress(uAddress) {
    Wire.begin();
}

float HSC60P::read() {
    Wire.requestFrom(uAddress , 2);
    delayMicroseconds(200);
    uint16_t uReading = Wire.read() << 8;
    delayMicroseconds(200);
    uReading |= Wire.read();
    delayMicroseconds(200);
    return ((float)uReading-1638)*MAX_PRESSURE/13107;
}
