#include "thermocouple.h"

TypeKThermo::TypeKThermo (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void TypeKThermo::init() {
    adc->enableChannel(uAdcChannel);
}

float TypeKThermo::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return uAdcConversion/4.096 - 105;
}
