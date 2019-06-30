#include <Arduino.h>
#include "lp_transducer_PX250P_arduino.h"

ArduinoPX250P::ArduinoPX250P (uint8_t uAnalogPin) : uAnalogPin(uAnalogPin) {}

float ArduinoPX250P::read() {
    uint16_t uAdcConversion = analogRead(uAnalogPin);
    float fVoltage = ((float)uAdcConversion/(1<<ADC_BITS)) * ADC_RANGE;
    return ((fVoltage-MIN_VOLTAGE) / (MAX_VOLTAGE-MIN_VOLTAGE)) * MAX_PRESSURE;
}
