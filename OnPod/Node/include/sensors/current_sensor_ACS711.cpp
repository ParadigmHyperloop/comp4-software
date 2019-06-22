#include "current_sensor_ACS711.h"

ACS711::ACS711(ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void ACS711::init() {
    adc->enableChannel(uAdcChannel);
}

float ACS711::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];

}
