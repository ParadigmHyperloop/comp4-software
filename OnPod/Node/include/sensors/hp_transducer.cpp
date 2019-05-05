#include "hp_transducer.h"

MLH03K::MLH03K (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void MLH03K::init() {
    adc->enableChannel(uAdcChannel);
}

float MLH03K::read() {
    uint16_t uAdcConversion = adc->uAdcData[uAdcChannel];
    return (uAdcConversion/4096*5-1)*750;
}
