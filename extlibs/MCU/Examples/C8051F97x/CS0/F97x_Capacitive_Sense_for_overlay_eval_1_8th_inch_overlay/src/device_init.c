/**************************************************************************//**
 * Copyright (c) 2015 by Silicon Laboratories Inc. All rights reserved.
 *
 * http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
 *****************************************************************************/

#include <si_toolchain.h>             // compiler declarations
#include "SI_C8051F970_Defs.h"
#include "cslib_config.h"
#include "device_init.h"
#include "comm_routines.h"

#include "cslib.h"

//-----------------------------------------------------------------------------
// OSCILLATOR_init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function initializes the system clock to use the internal precision
// oscillator
//
//-----------------------------------------------------------------------------
void OSCILLATOR_init (void)
{
   RSTSRC = 0x06;                      // Enable missing clock detector and
                                       // leave VDD Monitor enabled.

   OSCICN |= 0x80;                     

   while(OSCICN & 0x40);

   CLKSEL &= 0x03;                     // Select high precision oscillator

   FLSCL |= 0x40;                      // Set the BYPASS bit for operation > 10MHz

}

//-----------------------------------------------------------------------------
// PORT_init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// This function configures the crossbar and ports pins.
//
// P1.2 CS0 Switch 4
// P1.4 UART TX
// P1.5 UART RX

//-----------------------------------------------------------------------------
void PORT_init (void)
{

   SI_SEGMENT_VARIABLE(SFRPAGEsave, uint8_t, SI_SEG_IDATA) = SFRPAGE;

   SFRPAGE = CONFIG_PAGE;

   P1MDOUT |= 0x03;
   P3MDOUT |= 0x3F;

   P5MDOUT |= 0x01;


   XBR1 |= 0x40;               // Enable crossbar and weak pull-ups


   SFRPAGE = SFRPAGEsave;    // Restore the SFRPAGE

}


void systemInit(void)
{
   OSCILLATOR_init();
   PORT_init();

}
