#include "lp_transducer_PX250P.h"

PX250P::PX250P (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void PX250P::init() {
    adc->enableChannel(uAdcChannel);
}

float PX250P::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    float fVoltage = ((float)uAdcConversion/(1<<ADC_BITS)) * ADC_RANGE;
    return ((fVoltage-MIN_VOLTAGE) / (MAX_VOLTAGE-MIN_VOLTAGE)) * MAX_PRESSURE;
}
