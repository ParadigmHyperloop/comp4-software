#ifndef SOLENOID_H
#define SOLENOID_H

class DRV8806 {
private:
    const uint8_t PIN_LATCH;
    const uint8_t PIN_DOUT;
    const uint8_t PIN_DIN;
    const uint8_t PIN_SCLK;
    uint8_t iActiveSolenoids = 0;
public:
    DRV8806(uint8_t PIN_LATCH, uint8_t PIN_DOUT, uint8_t PIN_DIN, uint8_t PIN_SCLK);
    void init();
    void updateSolenoids();
    void enableSolenoid(uint8_t iSolenoids);
    void disableSolenoid(uint8_t iSolenoids);
};

#endif
