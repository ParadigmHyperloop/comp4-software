#include <Arduino.h>
#include "atmo_transducer_HSC30P.h"

HSC30P::HSC30P(uint8_t uAddress) : uAddress(uAddress) {
    Wire.begin();
}

float HSC30P::read() {
    Wire.requestFrom(uAddress , 2);
    delayMicroseconds(200);
    uint16_t uReading = Wire.read() << 8;
    delayMicroseconds(200);
    uReading |= Wire.read();
    delayMicroseconds(200);
    return ((float)uReading/(1<<RESOLUTION_BITS))*MAX_PRESSURE;
}
