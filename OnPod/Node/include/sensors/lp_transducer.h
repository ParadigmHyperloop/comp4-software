#ifndef LP_TRANSDUCER_H
#define LP_TRANSDUCER_H

#include "drivers/adc.h"

class U5244 {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    U5244(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
