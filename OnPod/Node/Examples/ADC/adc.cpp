#include <Arduino.h>
#include <SPI.h>
#include "adc.h"

ExtADC::ExtADC(uint16_t iActiveChannels) : iActiveChannels(iActiveChannels) {
    // find the number of ones in iActiveChannels; number of channels used
    uint16_t i = iActiveChannels;
    while(i != 0) {
        i &= (i - 1);
        iNumChannels++;
    }
}

void ExtADC::init() {
    SPI.begin();
    pinMode(SS1, OUTPUT);
    pinMode(POWER_SEQ_ADC, OUTPUT);
    digitalWrite(SS1, HIGH); // start with SS high
    digitalWrite(POWER_SEQ_ADC, HIGH); // enable on power sequencer
    delay(10);
    transfer(SET_CHANNEL_REG); // enables programming of active channels
    transfer(iActiveChannels); // tells the ADC what channels to read
    transfer(CONFIG_PROGRAM_REG); // configures the ADC to use 0-5V
}

uint16_t ExtADC::transfer(uint16_t iData) {
    SPI.beginTransaction(Settings);
    digitalWrite(SS1, LOW);
    uint16_t iRecievedData = SPI.transfer16(iData);
    digitalWrite(SS1, HIGH);
    SPI.endTransaction();
    return iRecievedData;
}

void ExtADC::readChannels() {
    // data results are from two read comands ago;
    // send two data requests before we start to read
    transfer(READ_RESET);
    transfer(READ_NEXT);
    for(uint8_t i = 0; i < iNumChannels; i++) {
        uint16_t iConversionData = transfer(READ_NEXT);
<<<<<<< HEAD
        // ADC channel is the 4 leftmost bits
=======
        // adc channel is the 4 leftmost bits
>>>>>>> b3ba3ec0759ceec97209ce3f459d25058882140d
        uint8_t iChannelNumber = (iConversionData & 0xF000) >> 12;
        // conversion is the 12 rightmost bits
        iADCData[iChannelNumber] = iConversionData & 0x0FFF;
    }
}
