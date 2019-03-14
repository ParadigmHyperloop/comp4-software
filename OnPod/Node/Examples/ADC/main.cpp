#include <Arduino.h>
#include <SPI.h>
#include "adc.h"

// start ADC object ussing channels 5, 11, and 13
ADS7953 adc(0b0010100000100000);

void setup() {
  Serial.begin(9600);
  adc.init();
}

void loop() {
    adc.readChannels();
    delay(500);
}
