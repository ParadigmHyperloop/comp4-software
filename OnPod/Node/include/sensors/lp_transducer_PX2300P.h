#ifndef LP_TRANSDUCER_PX2300P_H
#define LP_TRANSDUCER_PX2300P_H

#include "drivers/adc_ADS7953.h"

class PX2300P {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
    const uint8_t ADC_BITS = 12;
    const uint8_t ADC_RANGE= 5;
    const float MIN_VOLTAGE = 0.5;
    const float MAX_VOLTAGE = 4.5;
    const uint16_t MAX_PRESSURE = 300;
public:
    PX2300P(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
