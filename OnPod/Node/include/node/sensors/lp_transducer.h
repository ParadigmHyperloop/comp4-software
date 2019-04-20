#ifndef LP_TRANSDUCER_H
#define LP_TRANSDUCER_H

#include "node/drivers/adc.h"

class LP_NAME {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    LP_NAME(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
