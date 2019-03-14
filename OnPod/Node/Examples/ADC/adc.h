#ifndef ADC_H
#define ADC_H

class ExtADC {
private:
    SPIClass SPI = SPIClass(&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
    SPISettings Settings = SPISettings(20000000, MSBFIRST, SPI_MODE0);
    // 16-bit data words for SPI
    const uint16_t SET_CHANNEL_REG = 0x8000;
    const uint16_t CONFIG_PROGRAM_REG = 2840;
    const uint16_t READ_RESET = 0x2C00;
    const uint16_t READ_NEXT = 0x2000;

    uint16_t iActiveChannels; // each bit represents a channel; 1=used, 0=unused
    uint16_t iNumChannels = 0; // number of channels being used;
    uint16_t iADCData[16] {}; //array of the latest data; 0 for channels not being used

    uint16_t transfer(uint16_t iData);
public:
    ExtADC(uint16_t iActiveChannels);
    void init();
    void readChannels();
};

#endif
