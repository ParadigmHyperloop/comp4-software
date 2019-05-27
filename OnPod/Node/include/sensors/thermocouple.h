#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include "drivers/adc_ADS7953.h"

class TypeKThermo {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
    const uint8_t ADC_BITS = 12;
    const uint8_t ADC_RANGE= 5;
    const float AD8495_VREF = 0.5;
    const float V_PER_DEGREE = 0.005;
public:
    TypeKThermo(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
