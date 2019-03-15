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
}

void loop() {
    // read a single channel
    uint16_t ch5_single = adc.readSingleChannel(5);

    // read all active channels then access this data through adc.iADCData
    adc.readActiveChannels();
    uint16_t ch5_auto = adc.iADCData[5];

    delay(500);
}
