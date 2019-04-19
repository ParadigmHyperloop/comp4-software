#ifndef IRTEMP_H
#define IRTEMP_H

#include "node/drivers/adc.h"

class OS101E {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    OS101E(ADS7953 *adc, uint8_t uAdcChannel);
    float readTemperature();
};

#endif
