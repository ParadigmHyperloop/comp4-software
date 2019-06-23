#ifndef CURRENT_SENSOR_ACS711_H
#define CURRENT_SENSOR_ACS711_H

#include "drivers/adc_ADS7953.h"

class ACS711 {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    ACS711(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
