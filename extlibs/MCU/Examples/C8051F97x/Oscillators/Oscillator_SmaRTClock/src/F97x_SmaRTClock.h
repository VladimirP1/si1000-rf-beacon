#ifndef  F97X_SMARTCLOCK_H
#define  F97X_SMARTCLOCK_H
//----------------------------------------------------------------------------
// F97x_SmaRTClock.h
//----------------------------------------------------------------------------
// Copyright 2014 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Header File Description:
//
// This file is SmaRTClock header file for the C8051F97x.
//
// Target:         C8051F97x
// Tool chain:     Simplicity Studio / Keil C51 9.51
// Command Line:   None
//
// Release 1.0
//    - Initial Revision (CM2/JL)
//    - 19 MAY 2014
//
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------
#define RTC_USE_CRYSTAL    1
#define RTC_USE_LFO        0
//-----------------------------------------------------------------------------
// Function Prototypes
//-----------------------------------------------------------------------------
void  RTC_Init (U8 useCrystal);
U8    RTC_Read (U8 reg);
void  RTC_Write (U8 reg, U8 value);
U8    RTC_SetTimer(U32 time);
U32   RTC_CaptureTimer (void);
void  RTC_WriteAlarm(U32 time);
U32   RTC_ReadAlarm ();
//-----------------------------------------------------------------------------
#endif  // F97X_SMARTCLOCK_H
