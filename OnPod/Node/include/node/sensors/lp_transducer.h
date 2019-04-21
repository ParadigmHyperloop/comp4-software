#ifndef LP_TRANSDUCER_H
#define LP_TRANSDUCER_H

#include "node/drivers/adc.h"

class U5200 {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    U5200(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
