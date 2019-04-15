#include <Arduino.h>
#include "irtemp.h"

OS101E::OS101E (ADS7953 adc, uint8_t uAdcChannel) :
    adc(adc),
    uAdcChannel(uAdcChannel)
    {}

float OS101E::readTemperature () {
    uint16_t uAdcConversion = adc.readSingleChannel(uAdcChannel);
    return (uAdcConversion/4096)*556 - 18;
}
