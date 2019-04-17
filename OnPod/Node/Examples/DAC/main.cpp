#include <Arduino.h>
#include <SPI.h>

SPIClass dacSPI (&PERIPH_SPI2, MISO2, SCK2, MOSI2, PAD_SPI2_TX, PAD_SPI2_RX);
SPISettings dacSPISettings (20000000, MSBFIRST, SPI_MODE0);

const uint8_t WRITE_DATA[3] {0x01, 0x00, 0xFF};
const uint8_t READ_DATA[3] {0x02, 0x00, 0x01};
const uint8_t NO_OP[3] {0x00, 0x00, 0x00};

void setup() {
    Serial.begin(9600);

    pinMode(SS2, OUTPUT);
    pinMode(POWER_SEQ_DAC, OUTPUT);
    pinMode(DAC_SEQ_START, OUTPUT);
    digitalWrite(SS2, HIGH);
    digitalWrite(POWER_SEQ_DAC, HIGH);
    digitalWrite(DAC_SEQ_START, HIGH);
    delay(1000);
}

void loop() {
    dacSPI.beginTransaction(dacSPISettings);
    digitalWrite(SS2, LOW);
    uint8_t r1 = dacSPI.transfer(WRITE_DATA[0]);
    uint8_t r2 = dacSPI.transfer(WRITE_DATA[1]);
    uint8_t r3 = dacSPI.transfer(WRITE_DATA[2]);
    uint8_t r4 = dacSPI.transfer(WRITE_DATA[0]);
    uint8_t r5 = dacSPI.transfer(WRITE_DATA[1]);
    uint8_t r6 = dacSPI.transfer(WRITE_DATA[2]);
    digitalWrite(SS2, HIGH);
    dacSPI.endTransaction();

    Serial.println("Write");
    Serial.println(r1, BIN);
    Serial.println(r2, BIN);
    Serial.println(r3, BIN);
    Serial.println(r4, BIN);
    Serial.println(r5, BIN);
    Serial.println(r6, BIN);
    delay(500);

    digitalWrite(SS2, LOW);
    r1 = dacSPI.transfer(READ_DATA[0]);
    r2 = dacSPI.transfer(READ_DATA[1]);
    r3 = dacSPI.transfer(READ_DATA[2]);
    r4 = dacSPI.transfer(READ_DATA[0]);
    r5 = dacSPI.transfer(READ_DATA[1]);
    r6 = dacSPI.transfer(READ_DATA[2]);
    digitalWrite(SS2, HIGH);

    Serial.println("Read");
    Serial.println(r1, BIN);
    Serial.println(r2, BIN);
    Serial.println(r3, BIN);
    Serial.println(r4, BIN);
    Serial.println(r5, BIN);
    Serial.println(r6, BIN);
    delay(500);

    digitalWrite(SS2, LOW);
    r1 = dacSPI.transfer(NO_OP[0]);
    r2 = dacSPI.transfer(NO_OP[1]);
    r3 = dacSPI.transfer(NO_OP[2]);
    r4 = dacSPI.transfer(NO_OP[0]);
    r5 = dacSPI.transfer(NO_OP[1]);
    r6 = dacSPI.transfer(NO_OP[2]);
    digitalWrite(SS2, HIGH);

    Serial.println("No Op");
    Serial.println(r1, BIN);
    Serial.println(r2, BIN);
    Serial.println(r3, BIN);
    Serial.println(r4, BIN);
    Serial.println(r5, BIN);
    Serial.println(r6, BIN);
}
