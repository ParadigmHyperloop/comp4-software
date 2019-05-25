#ifndef IR_TEMP_OS101E_H
#define IR_TEMP_OS101E_H

#include "drivers/adc_ADS7953.h"

class OS101E {
private:
    ADS7953 *adc;
    uint8_t uAdcChannel;
public:
    OS101E(ADS7953 *adc, uint8_t uAdcChannel);
    void init();
    float read();
};

#endif
