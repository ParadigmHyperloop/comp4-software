#include "lp_transducer_PX2300P.h"

PX2300P::PX2300P (ADS7953 *adc, uint8_t uAdcChannel) :
    adc(adc), uAdcChannel(uAdcChannel)
    {}

void PX2300P::init() {
    adc->enableChannel(uAdcChannel);
}

float PX2300P::read() {
    uint16_t uAdcConversion = adc->getuAdcData()[uAdcChannel];
    return (((float)uAdcConversion/4096*5-0.5)/4)*300;
}
