#include <Arduino.h>
#include <SPI.h>
#include "adc.h"

ADS7953::ADS7953(SPIClass spi, SPISettings spiSettings) :
    spi(spi), spiSettings(spiSettings)
    {}

void ADS7953::init() {
    spi.begin();
    pinMode(SS1, OUTPUT);
    pinMode(POWER_SEQ_ADC, OUTPUT);
    digitalWrite(SS1, HIGH); // start with SS high
    digitalWrite(POWER_SEQ_ADC, HIGH); // enable ADC power sequencer
    delay(100);
    transfer(CONFIG_PROGRAM_REG); // configures the ADC to use 0-5V
}

uint16_t ADS7953::transfer(uint16_t iData) {
    spi.beginTransaction(spiSettings);
    digitalWrite(SS1, LOW);
    uint16_t iRecievedData = spi.transfer16(iData);
    digitalWrite(SS1, HIGH);
    spi.endTransaction();
    return iRecievedData;
}

void ADS7953::readActiveChannels() {
    // data results are from two read commands ago;
    // send two data requests before we start to read
    transfer(READ_RESET);
    transfer(READ_NEXT);
    // find the number of ones in iActiveChannels; number of channels used
    uint16_t iNumChannels = 0; // reset channel count
    uint16_t i = iActiveChannels; // copy iActiveChannels to i
    while(i != 0) {
        i &= (i - 1);
        iNumChannels++; // increment while there are still 1s
    }
    for(uint8_t i = 0; i < iNumChannels; i++) {
        uint16_t iConversionData = transfer(READ_NEXT);
        // ADC channel is the 4 leftmost bits
        uint8_t iChannelNumber = (iConversionData & 0xF000) >> 12;
        // conversion is the 12 rightmost bits
        iADCData[iChannelNumber] = iConversionData & 0x0FFF;
    }
}

void ADS7953::enableChannel(uint8_t iChannel) {
    iActiveChannels |= (1 << iChannel); // the the iChannel'th bit to 1
    transfer(SET_CHANNEL_REG); // enables programming of active channels
    transfer(iActiveChannels); // programs the active channels
}

void ADS7953::disableChannel(uint8_t iChannel) {
    iActiveChannels &= ~(1 << iChannel); // the the iChannel'th bit to 0
    transfer(SET_CHANNEL_REG); // enables programming of active channels
    transfer(iActiveChannels); // programs the active channels
}
