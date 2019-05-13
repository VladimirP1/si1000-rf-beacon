#include <compiler_defs.h>             // compiler declarations
#include <stdint.h>
#include <Si1000_defs.h>               // SFR declarations
#ifndef RADIO_H
#define RADIO_H

#define SYSCLK       (24500000L)

SBIT(NSS1, SFR_P1, 4);
SBIT(IRQ, SFR_P0,1);
SBIT(SDN, SFR_P2,6);

void    spi1_init();
void delay (U16 ticks);
void delay_us (U16 ticks);
void    rf_init();
void    rf_write(uint8_t adr, uint8_t data);
uint8_t rf_read(uint8_t adr);
void rf_setfreq(U32 frequency);
void serial_init();
void rf_set_power(uint8_t pow);

#endif
