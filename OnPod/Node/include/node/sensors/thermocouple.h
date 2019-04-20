#ifndef THERMOCOUPLE_H
#define THERMOCOUPLE_H

#include "node/drivers/adc.h"

class THERMO_NAME {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    THERMO_NAME(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
