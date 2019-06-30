#ifndef LP_TRANSDUCER_PX250P_ARDUINO_H
#define LP_TRANSDUCER_PX250P_ARDUINO_H

#include "drivers/adc_ADS7953.h"

class ArduinoPX250P {
private:
    uint8_t uAnalogPin;
    const uint8_t ADC_BITS = 10;
    const uint8_t ADC_RANGE= 5;
    const float MIN_VOLTAGE = 0.5;
    const float MAX_VOLTAGE = 4.5;
    const uint16_t MAX_PRESSURE = 50;
public:
    ArduinoPX250P(uint8_t uAnalogPin);
    float read();
};

#endif
