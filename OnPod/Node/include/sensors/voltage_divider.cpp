#include "voltage_divider.h"

VoltageDivider::VoltageDivider(ADS7953 *adc, uint8_t uAdcChannel, uint16_t R1, uint16_t R2) :
    adc(adc), uAdcChannel(uAdcChannel), R1(R1), R2(R2)
    {}

void VoltageDivider::init() {
    adc->enableChannel(uAdcChannel);
}

float VoltageDivider::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return ((float)uAdcConversion/(1<<ADC_BITS)*ADC_RANGE) / (R2/(R1+R2));
}
