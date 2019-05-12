#include "lp_transducer.h"

U5244::U5244 (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void U5244::init() {
    adc->enableChannel(uAdcChannel);
}

float U5244::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return (uAdcConversion/4096*5-1)*50;
}
