#include "hp_transducer.h"

MLH03KPSL01G::MLH03KPSL01G (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

float MLH03KPSL01G::read() {
    uint16_t uAdcConversion = adc->readSingleChannel(uAdcChannel);
    return (uAdcConversion/4096*5-1)*750;
}
