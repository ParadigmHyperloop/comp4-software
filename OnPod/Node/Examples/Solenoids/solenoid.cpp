#include <Arduino.h>
#include "solenoid.h"

DRV8806::DRV8806(uint8_t PIN_LATCH, uint8_t PIN_DOUT, uint8_t PIN_DIN, uint8_t PIN_SCLK) :
    PIN_LATCH(PIN_LATCH), PIN_DOUT(PIN_DOUT),
    PIN_DIN(PIN_DIN), PIN_SCLK(PIN_SCLK)
    {}

void DRV8806::init() {
    pinMode(PIN_SCLK, OUTPUT);
    pinMode(PIN_LATCH, OUTPUT);
    pinMode(PIN_DOUT, OUTPUT);
    pinMode(PIN_DIN, INPUT);
    digitalWrite(PIN_SCLK, LOW);
    digitalWrite(PIN_LATCH, HIGH);
    digitalWrite(PIN_DOUT, LOW);
}

void DRV8806::updateSolenoids() {
    digitalWrite(PIN_LATCH, LOW);
    digitalWrite(PIN_SCLK, LOW);
    delayMicroseconds(10);

    for (uint8_t i = 0; i < 8; i++) {
        uint8_t iSolenoid = 7 - i;
        bool bState = (iActiveSolenoids >> iSolenoid) & 1;
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

void DRV8806::enableSolenoid(uint8_t iSolenoid) {
    iActiveSolenoids |= (1 << iSolenoid);
}

void DRV8806::disableSolenoid(uint8_t iSolenoid) {
    iActiveSolenoids &= ~(1 << iSolenoid);
}
