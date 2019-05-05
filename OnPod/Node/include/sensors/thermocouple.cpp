#include "thermocouple.h"

typeKThermo::typeKThermo (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void typeKThermo::init() {
    adc->enableChannel(uAdcChannel);
}

float typeKThermo::read() {
    uint16_t uAdcConversion = adc->uAdcData[uAdcChannel];
    return uAdcConversion/4.096 - 105;
}
