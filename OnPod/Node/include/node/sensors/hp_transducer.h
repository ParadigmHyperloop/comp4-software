#ifndef HP_TRANSDUCER_H
#define HP_TRANSDUCER_H

#include "node/drivers/adc.h"

class HP_NAME {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    HP_NAME(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
