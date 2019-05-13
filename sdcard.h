#include <compiler_defs.h>             // compiler declarations
#include <stdint.h>
#include <Si1000_defs.h>               // SFR declarations
#include "radio.h"

#ifndef SDCARD_H
#define SDCARD_H
SBIT(SPI9_SCK,    SFR_P1, 6);  // SCK  - Red led
SBIT(SPI9_MOSI,   SFR_P1, 5);  // MOSI - Green led
SBIT(SPI9_MISO,   SFR_P0, 2);  // MISO - Config

#define SD_CS_ON   rf_write(EZRADIOPRO_IO_PORT_CONFIGURATION,0);
#define SD_CS_OFF   rf_write(EZRADIOPRO_IO_PORT_CONFIGURATION,4);


void spi9_init();

void sd_init_card();
uint8_t r1();
uint8_t rb();
void cmd0();
void cmd_crc(uint8_t n, uint32_t arg, uint8_t crc);
inline void cmd(uint8_t n, uint32_t arg);
uint8_t rd(uint32_t block, uint8_t* data);
uint8_t wr(uint32_t block, uint8_t* data);
void hexdump(uint8_t i);
void putc_uart0(uint8_t i);
void sd_update();
uint32_t get_sound_start(uint16_t id);
uint32_t get_sound_end(uint16_t id);
void say(uint16_t n);
void fs_beep(uint8_t t);

#endif
