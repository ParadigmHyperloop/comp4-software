#include "ir_temp.h"

OS101E::OS101E(ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void OS101E::init() {
    adc->enableChannel(uAdcChannel);
}

float OS101E::read() {
    uint16_t uAdcConversion = adc->uAdcData[uAdcChannel];
    return (uAdcConversion/4096)*556 - 18;
}
