#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include "drivers/adc_ADS7953.h"

class VoltageDivider {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
    const uint8_t ADC_BITS = 12;
    const uint8_t ADC_RANGE= 5;
    uint16_t R1;
    uint16_t R2;
public:
    VoltageDivider(ADS7953 *adc, uint8_t uAdcChannel, uint16_t R1, uint16_t R2);
    void init();
    float read();
};

#endif
