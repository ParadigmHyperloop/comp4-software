#ifndef HP_TRANSDUCER_U5374_H
#define HP_TRANSDUCER_U5374_H

#include "drivers/adc_ADS7953.h"

class U5374 {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    U5374(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
