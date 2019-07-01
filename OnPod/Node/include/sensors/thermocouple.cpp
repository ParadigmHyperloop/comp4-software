#include "thermocouple.h"

TypeKThermo::TypeKThermo (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void TypeKThermo::init() {
    adc->enableChannel(uAdcChannel);
}

float TypeKThermo::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    float fVoltage = (float)uAdcConversion/(1<<ADC_BITS) * ADC_RANGE;
    return (fVoltage - AD8495_VREF) / V_PER_DEGREE;
}
