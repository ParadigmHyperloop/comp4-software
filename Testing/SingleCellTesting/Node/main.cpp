#include <Arduino.h>
#include <SPI.h>

#include "drivers/adc.h"

SPIClass adcSPI (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
ADS7953 adc(adcSPI);

void setup() {
    adc.init();
    Serial.begin(9600);
    randomSeed(analogRead(0));
}

void loop() {
    // TODO: replace the random numbers with adc reads and conversions
    float cell_voltage = random(1, 1000);
    float cell_current = random(1, 1000);
    float temp_1 = random(1, 1000);
    float temp_2 = random(1, 1000);
    float temp_3 = random(1, 1000);
    Serial.print(millis(), 1);
    Serial.print(",");
    Serial.print(cell_voltage, 2);
    Serial.print(",");
    Serial.print(cell_current, 2);
    Serial.print(",");
    Serial.print(temp_1, 2);
    Serial.print(",");
    Serial.print(temp_2, 2);
    Serial.print(",");
    Serial.print(temp_3);
    Serial.println(",");
    delay(15);
}
