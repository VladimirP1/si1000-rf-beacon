#include <compiler_defs.h>             // compiler declarations
#include <stdint.h>
#include <Si1000_defs.h>               // SFR declarations
#include "sdcard.h"

#ifndef LOADER_H
#define LOADER_H
void loader_update(uint8_t c);
uint8_t loader_blocked();
#endif
