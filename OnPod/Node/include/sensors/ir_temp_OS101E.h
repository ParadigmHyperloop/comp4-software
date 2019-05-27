#ifndef IR_TEMP_OS101E_H
#define IR_TEMP_OS101E_H

#include "drivers/adc_ADS7953.h"

class OS101E {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
    const int8_t MIN_TEMP = -18;
    const uint16_t MAX_TEMP = 538;
    const uint16_t TEMP_RANGE = MAX_TEMP - MIN_TEMP;
    const uint8_t ADC_BITS = 12;
public:
    OS101E(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
