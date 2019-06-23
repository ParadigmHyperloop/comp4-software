#include "lp_transducer_PX2300P.h"

PX2300P::PX2300P (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void PX2300P::init() {
    adc->enableChannel(uAdcChannel);
}

float PX2300P::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    float fVoltage = ((float)uAdcConversion/(1<<ADC_BITS)) * ADC_RANGE;
    return ((fVoltage-MIN_VOLTAGE) / (MAX_VOLTAGE-MIN_VOLTAGE)) * MAX_PRESSURE;
}
