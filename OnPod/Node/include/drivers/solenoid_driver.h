#ifndef SOLENOID_DRIVER_H
#define SOLENOID_DRIVER_H

#include <Arduino.h>

class DRV8806 {
private:
    const uint8_t PIN_LATCH = SOL_CS;
    const uint8_t PIN_DOUT = SOL_DOUT;
    const uint8_t PIN_DIN = SOL_DIN;
    const uint8_t PIN_SCLK = SOL_CLK;
    uint8_t uActiveSolenoids = 0;
public:
    void init();
    void updateSolenoids();
    void enableSolenoid(uint8_t uSolenoids);
    void disableSolenoid(uint8_t uSolenoids);
};

#endif
