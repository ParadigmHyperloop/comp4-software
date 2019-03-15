#ifndef ADC_H
#define ADC_H

class ADS7953 {
private:
    SPIClass spi;
    SPISettings spiSettings;
    // 16-bit data words for SPI
    const uint16_t SET_CHANNEL_REG = 0x8000;
    const uint16_t CONFIG_PROGRAM_REG = 2840;
    const uint16_t MANUAL_READ = 0x1000;
    const uint16_t AUTO_READ_RESET = 0x2C00;
    const uint16_t AUTO_READ_NEXT = 0x2000;
    uint16_t iActiveChannels; // each bit represents a channel; 1=used, 0=unused
    uint16_t iNumChannels = 0; // number of channels being used;

    uint16_t transfer(uint16_t iData);
public:
    uint16_t iADCData[16] {}; //array of the latest data; 0 for channels not being used

    ADS7953(SPIClass spi, SPISettings spiSettings);
    void init();
    uint16_t readSingleChannel(uint8_t);
    void readActiveChannels();
    void enableChannel(uint8_t);
    void disableChannel(uint8_t);
};

#endif
