#include <Arduino.h>
#include "atmo_temperature_TC74.h"

TC74::TC74(uint8_t uAddress) : uAddress(uAddress) {
    Wire.begin();
}

float TC74::read() {
    Wire.beginTransmission(uAddress);
    Wire.write(0);
    Wire.requestFrom(uAddress, 1);
    return Wire.read();
}
