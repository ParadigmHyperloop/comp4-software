#include "thermocouple.h"

typeKThermo::typeKThermo (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel) {
        adc->enableChannel(uAdcChannel);
    }

float typeKThermo::read() {
    uint16_t uAdcConversion = adc->uAdcData[uAdcChannel];
    return (uAdcConversion/4096)*556 - 18; // TODO: fix conversion for type k
}
