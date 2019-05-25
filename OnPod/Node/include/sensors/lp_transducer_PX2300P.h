#ifndef LP_TRANSDUCER_PX2300P_H
#define LP_TRANSDUCER_PX2300P_H

#include "drivers/adc_ADS7953.h"

class PX2300P {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    PX2300P(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
