#include "hp_transducer.h"

MLH03KPSL01G::MLH03KPSL01G (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel) {
        adc->enableChannel(uAdcChannel);
    }

float MLH03KPSL01G::read() {
    uint16_t uAdcConversion = adc->uAdcData[uAdcChannel];
    return (uAdcConversion/4096*5-1)*750;
}
