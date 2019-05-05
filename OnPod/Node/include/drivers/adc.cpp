#include "adc.h"

void ADS7953::init() {
    spi.begin();
    pinMode(SS1, OUTPUT);
    pinMode(POWER_SEQ_ADC, OUTPUT);
    digitalWrite(SS1, HIGH); // start with SS high
    digitalWrite(POWER_SEQ_ADC, HIGH); // enable ADC power sequencer
    delay(100);
    transfer(CONFIG_PROGRAM_REG); // configures the ADC to use 0-5V
}

uint16_t ADS7953::transfer(uint16_t uData) {
    spi.beginTransaction(spiSettings);
    digitalWrite(SS1, LOW);
    uint16_t uRecievedData = spi.transfer16(uData);
    digitalWrite(SS1, HIGH);
    spi.endTransaction();
    return uRecievedData;
}

uint16_t ADS7953::readSingleChannel(uint8_t uChannel) {
    uint16_t uReadCommand = MANUAL_READ;
    // insert the channel address into bits 10-7 of the command
    uReadCommand |= (uChannel << 7);
    // data results are from two data frames ago;
    // send two data requests before we start to read
    transfer(uReadCommand);
    transfer(uReadCommand);
    return transfer(uReadCommand) & 0x0FFF;
}

void ADS7953::readActiveChannels() {
    // data results are from two data frames ago;
    // send two data requests before we start to read
    transfer(AUTO_READ_RESET);
    transfer(AUTO_READ_NEXT);
    // find the number of ones in uActiveChannels; number of channels used
    uint16_t uNumChannels = 0; // reset channel count
    uint16_t i = uActiveChannels; // copy uActiveChannels to i
    while(i != 0) {
        i &= (i - 1);
        uNumChannels++; // increment while there are still 1s
    }
    for(uint8_t i = 0; i < uNumChannels; i++) {
        uint16_t uConversionData = transfer(AUTO_READ_NEXT);
        // ADC channel is the 4 leftmost bits
        uint8_t uChannelNumber = (uConversionData & 0xF000) >> 12;
        // conversion is the 12 rightmost bits
        uAdcData[uChannelNumber] = uConversionData & 0x0FFF;
    }
}

void ADS7953::enableChannel(uint8_t uChannel) {
    uActiveChannels |= (1 << uChannel); // set the uChannel'th bit to 1
    transfer(SET_CHANNEL_REG); // enables programming of active channels
    transfer(uActiveChannels); // programs the active channels
}

void ADS7953::disableChannel(uint8_t uChannel) {
    uActiveChannels &= ~(1 << uChannel); // aet the uChannel'th bit to 0
    transfer(SET_CHANNEL_REG); // enables programming of active channels
    transfer(uActiveChannels); // programs the active channels
}
