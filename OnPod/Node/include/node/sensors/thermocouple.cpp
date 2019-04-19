#include "thermocouple.h"

THERMO_NAME::THERMO_NAME (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

float THERMO_NAME::readTemperature () {
    uint16_t uAdcConversion = adc->readSingleChannel(uAdcChannel);
    return (uAdcConversion/4096)*556 - 18; // TODO: fix conversion for type k
}
