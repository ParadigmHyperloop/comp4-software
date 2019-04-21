#ifndef HP_TRANSDUCER_H
#define HP_TRANSDUCER_H

#include "node/drivers/adc.h"

class MLH03KPSL01G {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    MLH03KPSL01G(ADS7953 *adc, uint8_t uAdcChannel);
    float read();
};

#endif
