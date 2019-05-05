#ifndef HP_TRANSDUCER_H
#define HP_TRANSDUCER_H

#include "drivers/adc.h"

class MLH03K {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    MLH03K(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
