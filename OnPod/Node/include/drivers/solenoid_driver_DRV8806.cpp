#include <Arduino.h>
#include "solenoid_driver_DRV8806.h"

DRV8806::DRV8806(uint8_t PIN_LATCH, uint8_t PIN_DOUT, uint8_t PIN_DIN, uint8_t PIN_SCLK) :
PIN_LATCH(PIN_LATCH),
PIN_DOUT(PIN_DOUT),
PIN_DIN(PIN_DIN),
PIN_SCLK(PIN_SCLK)
{}


void DRV8806::init() {
    pinMode(PIN_SCLK, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(PIN_DOUT, INPUT); // DRV8806 => SAMD21
    pinMode(PIN_DIN, OUTPUT); // SAMD21 => DRV8806
    digitalWrite(PIN_SCLK, LOW);
    digitalWrite(PIN_LATCH, HIGH);
    digitalWrite(PIN_DOUT, LOW);
}

void DRV8806::updateSolenoids() {
    digitalWrite(PIN_LATCH, LOW);
    digitalWrite(PIN_SCLK, LOW);
    delayMicroseconds(10);
    // loop through iActiveSolenoids and write DIN to the value of each bit
    // then write SCLK high then low to shift the bit in
    for (uint8_t uSolenoid = 7; uSolenoid >= 0 && uSolenoid <= 7; uSolenoid--) {
        bool bState = (uActiveSolenoids >> uSolenoid) & 1;
        digitalWrite(PIN_DIN, bState);
        delayMicroseconds(1);
        digitalWrite(PIN_SCLK, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIN_SCLK, LOW);
        delayMicroseconds(1);
    }
    delayMicroseconds(10);
    digitalWrite(PIN_LATCH, HIGH);
}

void DRV8806::enableSolenoid(uint8_t uSolenoid) {
    // set the selected iActiveSolenoids bit to 1
    uActiveSolenoids |= (1 << uSolenoid);
}

void DRV8806::disableSolenoid(uint8_t uSolenoid) {
    // set the selected iActiveSolenoids bit to 0
    uActiveSolenoids &= ~(1 << uSolenoid);
}
