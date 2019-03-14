#include <Arduino.h>
#include <SPI.h>
#include "adc.h"

SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
SPISettings adcSPISettings (20000000, MSBFIRST, SPI_MODE0);
ADS7953 adc(adcSPI, adcSPISettings);

void setup() {
    Serial.begin(9600);
    adc.init();
    adc.enableChannel(5);
    adc.enableChannel(11);
    adc.enableChannel(13);
}

void loop() {
    adc.readActiveChannels();
    delay(500);
}
