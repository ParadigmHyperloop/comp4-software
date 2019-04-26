#ifndef ADC_H
#define ADC_H

#include <SPI.h>


class ADS7953 {
private:
    SPIClass spi = SPIClass (&PERIPH_SPI1, MISO1, SCK1, MOSI1, PAD_SPI1_TX, PAD_SPI1_RX);
    SPISettings spiSettings = SPISettings (20000000, MSBFIRST, SPI_MODE0);

    // 16-bit data words for SPI
    const uint16_t SET_CHANNEL_REG = 0x8000;
    const uint16_t CONFIG_PROGRAM_REG = 0x2840;
    const uint16_t MANUAL_READ = 0x1000;
    const uint16_t AUTO_READ_RESET = 0x2C00;
    const uint16_t AUTO_READ_NEXT = 0x2000;
    uint16_t uActiveChannels = 0; // each bit represents a channel; 1=used, 0=unused
    uint16_t uNumChannels = 0; // number of channels being used;

    uint16_t transfer(uint16_t uData);
public:
    uint16_t uAdcData[16] {}; //array of the latest data; 0 for channels not being used
    void init();
    uint16_t readSingleChannel(uint8_t);
    void readActiveChannels();
    void enableChannel(uint8_t);
    void disableChannel(uint8_t);
};

#endif
