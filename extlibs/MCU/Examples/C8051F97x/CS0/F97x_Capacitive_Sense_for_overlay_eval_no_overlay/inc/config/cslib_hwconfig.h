#ifndef _HARDWARE_CONFIG_H
#define _HARDWARE_CONFIG_H

#define PORT(port, pin) (port<<3) | (pin)

// $[mux values]
#define MUX_VALUE_ARRAY PORT(2,5), PORT(2,7), PORT(2,6), PORT(2,4), PORT(4,1), PORT(4,6), PORT(3,1), PORT(3,6), PORT(2,2), PORT(4,3), PORT(4,4), PORT(3,3), PORT(3,4), PORT(2,0), PORT(4,5), PORT(4,2), PORT(3,5), PORT(3,2), PORT(2,1),PORT(4,7), PORT(4,0), PORT(3,7), PORT(3,0), PORT(2,3)
// [mux values]$

// $[gain values]
#define GAIN_VALUE_ARRAY 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
// [gain values]$

// $[accumulation values]
#define ACCUMULATION_VALUE_ARRAY 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5
// [accumulation values]$

// Pin mask used for port register config in sleep mode
// $[Active mode mask]
#define ACTIVE_MODE_MASK_P0              0x00
#define ACTIVE_MODE_MASK_P1              0x00
#define ACTIVE_MODE_MASK_P2              0xFF
#define ACTIVE_MODE_MASK_P3              0xFF
#define ACTIVE_MODE_MASK_P4              0xFF
#define ACTIVE_MODE_MASK_P5              0x00
// [Active mode mask]$

// $[Sleep mode mask]
#define SLEEP_MODE_MASK_P0              0x00
#define SLEEP_MODE_MASK_P1              0x00
#define SLEEP_MODE_MASK_P2              0xFF
#define SLEEP_MODE_MASK_P3              0xFF
#define SLEEP_MODE_MASK_P4              0xFF
#define SLEEP_MODE_MASK_P5              0x00
// [Sleep mode mask]$
#endif
