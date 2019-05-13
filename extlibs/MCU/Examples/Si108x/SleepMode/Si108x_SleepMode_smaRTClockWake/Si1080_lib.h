//-----------------------------------------------------------------------------
// C8051F930_lib.h
//-----------------------------------------------------------------------------
// Copyright (C) 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Driver for the power management function.
//
// Target:         Si108x
// Tool chain:     Generic
// Command Line:   None
//
//
// Release 1.0
//    - Initial Revision (FB)
//    - 29 JUL 2013
//
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <smartclock.h>                // RTC Functionality
#include <power.h>                     // Power Management Functionality


//-----------------------------------------------------------------------------
// Global Configuration Options
//-----------------------------------------------------------------------------

#define SYSCLK  20000000

#define SMARTCLOCK_ENABLED   1