#include "atmo_transducer_HSC30P.h"

HSC30P::HSC30P(uint8_t uAddress) : uAddress(uAddress) {
    Wire.begin();
}

float HSC30P::read() {
    Wire.requestFrom(0x28 , 2);
    uint16_t uReading = Wire.read() << 8;
    uReading |= Wire.read();
    return ((float)uReading/(1<<RESOLUTION_BITS))*MAX_PRESSURE;
}
