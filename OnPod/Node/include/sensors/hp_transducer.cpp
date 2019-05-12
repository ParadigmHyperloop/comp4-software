#include "hp_transducer.h"

U5374::U5374 (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void U5374::init() {
    adc->enableChannel(uAdcChannel);
}

float U5374::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return (uAdcConversion);
}
