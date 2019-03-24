#include <Arduino.h>
#include <SPI.h>

SPIClass solSPI (&PERIPH_SPI2, MISO2, SCK2, MOSI2, PAD_SPI2_TX, PAD_SPI2_RX);
SPISettings solSPISettings (20000000, MSBFIRST, SPI_MODE0);

void setup() {
    Serial.begin(9600);
}

void loop() {

}
