#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include "drivers/adc.h"

class typeKThermo {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    typeKThermo(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
