#ifndef OLED_SDHAL_HPP
#define OLED_SDHAL_HPP

#include <SPI.h>
#include <SD.h>

#define SD_CS         10
#define SPI_MOSI      11
#define SPI_MISO      13
#define SPI_SCK       12

const char * sdcard_error_message(int code);

int sdcard_init();

#endif // #ifndef OLED_SDHAL_HPP