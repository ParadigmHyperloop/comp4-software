#include <Arduino.h>
#include <SPI.h>

SPIClass adcSPI (&PERIPH_SPI1, MISO1, MOSI1, SCK1, PAD_SPI1_TX, PAD_SPI1_RX);

const uint16_t CH7_READ = 4992;

void setup() {
  Serial.begin(9600);
  adcSPI.begin();
}

void loop() {
    digitalWrite(PIN_SPI1_SS, LOW);
    uint16_t received_data = adcSPI.transfer(CH7_READ);
    digitalWrite(PIN_SPI1_SS, LOW);
    Serial.println(received_data);
    delay(500);
}
