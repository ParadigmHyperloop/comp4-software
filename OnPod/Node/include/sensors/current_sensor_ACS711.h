#ifndef CURRENT_SENSOR_ACS711_H
#define CURRENT_SENSOR_ACS711_H

#include "drivers/adc_ADS7953.h"

class ACS711 {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
    const uint16_t MV_PER_V = 1000;
    const uint8_t MV_PER_AMP = 45;
    const uint16_t ADC_BITS = 12;
    const uint8_t ADC_MAX_VOLTAGE = 5;

public:
    ACS711(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
