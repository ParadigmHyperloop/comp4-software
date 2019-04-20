#include "hp_transducer.h"

HP_NAME::HP_NAME (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

float HP_NAME::read() {
    uint16_t uAdcConversion = adc->readSingleChannel(uAdcChannel);
    return (uAdcConversion/4096)*556 - 18; // TODO: fix conversion for transducer
}
