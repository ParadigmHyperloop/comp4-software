#include "ir_temp_OS101E.h"

OS101E::OS101E(ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void OS101E::init() {
    adc->enableChannel(uAdcChannel);
}

float OS101E::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return (float)uAdcConversion/(1<<ADC_BITS)*TEMP_RANGE + MIN_TEMP;
}
