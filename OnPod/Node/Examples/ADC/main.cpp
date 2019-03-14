#include <Arduino.h>
#include <SPI.h>
#include "adc.h"

<<<<<<< HEAD
// start ADC object ussing channels 5, 11, and 13
=======
>>>>>>> b3ba3ec0759ceec97209ce3f459d25058882140d
ExtADC adc(0b0010100000100000);

void setup() {
  Serial.begin(9600);
  adc.init();
}

void loop() {
    adc.readChannels();
    delay(500);
}
