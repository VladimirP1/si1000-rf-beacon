//-----------------------------------------------------------------------------
// F85x_SMBus_Multimaster.c
//-----------------------------------------------------------------------------
// Copyright 2013 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Example software to demonstrate the SMBus interface in a Multi-Master
// environment.
// - Interrupt-driven SMBus implementation
// - Master and slave states defined
// - 1-byte SMBus data holders used for each transmit and receive
// - Timer1 used as SMBus clock source
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - ARBLOST support included
// - When another master attempts to transmit at the same time:
//      - the 'F36x SMBus master should detect any conflict using ARBLOST
//      - the 'F36x SMBus master should give up the bus, if necessary, and
//        switch to slave receiver mode
//      - the 'F36x SMBus master should transmit after the other master is done
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.0 -> LED1
//
//    P1.7 -> S1
//
//    all other port pins unused
//
// How To Test:
//
// 1) Ensure shorting blocks are place on the following:
//    - J27: DS5 P1.0 - P1.0
//    - JP2
// 2) Ensure that J32 is not populated.
// 3) Connect the device to another C8051Fxxx SMBus multimaster.
// 4) Place the SW1 switch in the +3.3V_VREG position and power the board
//    using the J6 power connector.
// 5) Verify that MY_ADDR and MULTI_ADDR are defined to the proper values.
// 6) Compile and download code to the 'F85x UDP MCU board.
// 7) Run the code. The test will indicate proper communication with the
//    other C8051Fxxx device by blinking the LED on the other device on and
//    off each time SW is pressed, even if the buttons on both boards are
//    pressed at the same time.
//
//      'F85x Multimaster                     Other C8051Fxxx Multimaster
//         LED1 (blinks) <------------------------------ Switch
//              SW --------------------------------> LED (blinks)
//
// Target:         C8051F85x/85x
// Tool chain:     Generic
// Command Line:   None
//
// Release 1.1 (TP)
//    - Updated descriptions and comments
//    - 24 JUL 2013
//
// Release 1.0
//    -Initial Revision (TP)
//    -27 FEB 2013
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <compiler_defs.h>
#include <C8051F850_defs.h>            // SFR declarations

//-----------------------------------------------------------------------------
// Global Constants
//-----------------------------------------------------------------------------

#define  SYSCLK               24500000 // System clock frequency in Hz

#define  SMB_FREQUENCY           10000 // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE                    0x00 // SMBus WRITE command
#define  READ                     0x01 // SMBus READ command

// Device addresses (7 bits, lsb is a don't care)
#define  MY_ADDR                  0x0F // Device address for this device target
#define  MULTI_ADDR               0x50 // Device address for other multimaster
                                       // target

// Status vector - top 4 bits only
#define  SMB_MTSTA                0xE0 // (MT) start transmitted
#define  SMB_MTDB                 0xC0 // (MT) data byte transmitted
#define  SMB_MRDB                 0x80 // (MR) data byte received

#define  SMB_SRADD                0x20 // (SR) slave address received
                                       //    (also could be a lost
                                       //    arbitration)
#define  SMB_SRSTO                0x10 // (SR) STOP detected while SR or ST,
                                       //    or lost arbitration
#define  SMB_SRDB                 0x00 // (SR) data byte received, or
                                       //    lost arbitration
#define  SMB_STDB                 0x40 // (ST) data byte transmitted
#define  SMB_STSTO                0x50 // (ST) STOP detected during a
                                       //    transaction; bus error
// End status vector definition

// Data to indicate the switch was pressed or released
#define  SW_PRESSED               0x0A
#define  SW_RELEASED              0x50

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

// Global holder for SMBus master data. All receive data is written here
// while in master mode
// Slave->Master
U8 SMB_DATA_IN_MASTER = 0x00;

// Global holder for SMBus master data. All transmit data is read from here
// while in master mode
// Master->Slave
U8 SMB_DATA_OUT_MASTER = 0x00;

// Global holder for SMBus slave data. All receive data is written here
// while slave mode
// Master->Slave
U8 SMB_DATA_IN_SLAVE = 0x00;

// Global holder for SMBus slave data. All transmit data is read from here
// while in slave mode
// Slave->Master
U8 SMB_DATA_OUT_SLAVE = 0x00;

volatile bit DATA_READY = 0;           // Set to '1' by the SMBus ISR
                                       // when a new data byte has been
                                       // received in slave mode.

U8 TARGET;                             // Target SMBus slave address

volatile bit SMB_BUSY;                 // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

volatile bit SMB_RW;                   // Software flag to indicate the
                                       // direction of the current transfer

U8 NUM_ERRORS;                         // Counter for the number of errors.

//-----------------------------------------------------------------------------
// Pin Definitions
//-----------------------------------------------------------------------------

SBIT(LED1, SFR_P1, 0);                 // DS5 P1.0 LED

#define LED_ON                       0
#define LED_OFF                      1

SBIT(S1, SFR_P1, 7);

SBIT(SDA, SFR_P0, 0);                  // SMBus on P0.0
SBIT(SCL, SFR_P0, 1);                  // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void SMBus_Init(void);
void Timer0_Init(void);
void Timer1_Init(void);
void Timer3_Init(void);
void Port_Init(void);

INTERRUPT_PROTO(TIMER0_ISR, INTERRUPT_TIMER0);
INTERRUPT_PROTO(TIMER3_ISR, INTERRUPT_TIMER3);
INTERRUPT_PROTO(SMBUS0_ISR, INTERRUPT_SMBUS0);

void SMB_Write(void);
void Blink_LED(void);
void Stop_LED(void);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------
//
// Main routine performs all configuration tasks, then loops forever checking
// for and sending the LED commands.
//
//-----------------------------------------------------------------------------
void main (void)
{
   bit switch_pressed_flag = 0;        // Used to detect when the switch is
                                       // released
   U8 i;                               // Dummy variable counters

   // Disable the watchdog timer
   WDTCN = 0xDE;
   WDTCN = 0xAD;

   CLKSEL = 0x00;                      // Set SYSCLK to 24.5 MHz divide by 1

   // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR2 = 0x40;                     // Enable Crossbar
      SCL = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL = 1;                         // Release the clock
      while(!SCL);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR2 = 0x00;                     // Disable Crossbar
   }

   Port_Init();                        // Initialize Crossbar and GPIO
   Timer0_Init();                      // Configure Timer0 to blink the LED
   Timer1_Init();                      // Configure Timer1 for use as SMBus
                                       // clock source
   Timer3_Init();                      // Configure Timer3 for use with SMBus
                                       // low timeout detect
   SMBus_Init();                       // Configure and enable SMBus


   EIE1 |= 0x01;                       // Enable the SMBus interrupt
   EIP1 |= 0x01;                       // make the SMBus interrupt high
                                       // priority

   ET0 = 1;                            // Enable the Timer0 interrupt (LED)

   LED1 = LED_OFF;

   EA = 1;                             // Global interrupt enable

   while(1)
   {
      // Check if the switch was just pressed
      if((switch_pressed_flag == 0) && (S1 == 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was pressed
          SMB_DATA_OUT_MASTER = SW_PRESSED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 1;
      }

      // Determine if the switch was just released
      if((switch_pressed_flag == 1) && (S1 != 0))
      {
          // SMBus Write Sequence

          // Tell the other device the switch was released
          SMB_DATA_OUT_MASTER = SW_RELEASED;

          TARGET = MULTI_ADDR;         // Target the other multimaster device
          SMB_Write();                 // Initiate SMBus write

          switch_pressed_flag = 0;
      }

      // Check if data was sent to this device from the other multimaster
      // device
      if(DATA_READY == 1)
      {
         if(SMB_DATA_IN_SLAVE == SW_PRESSED)
         {
            Blink_LED();
         }
         else
         {
            if(SMB_DATA_IN_SLAVE == SW_RELEASED)
            {
               Stop_LED();
            }
         }

         DATA_READY = 0;
      }
   }  // Loop forever
}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// SMBus configured as follows:
// - SMBus enabled
// - Timer1 used as clock source. The maximum SCL frequency will be
//   approximately 1/3 the Timer1 overflow rate
// - Setup and hold time extensions enabled
// - Bus Free and SCL Low timeout detection enabled
//
//-----------------------------------------------------------------------------
void SMBus_Init(void)
{
   SMB0CF = 0x1D;                      // Use Timer1 overflows as SMBus clock
                                       // source;
                                       // Enable setup & hold time
                                       // extensions;
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect;

   SMB0CF |= 0x80;                     // Enable SMBus;
}

//-----------------------------------------------------------------------------
// Timer0_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer0 is configured to blink LED1.  Timer0 uses the same CKCON register
// settings at Timer1, since the LED1 blink frequency is not dependent on
// anything.
//
//-----------------------------------------------------------------------------
void Timer0_Init(void)
{
   TMOD = 0x02;                        // Timer0 in 8-bit auto-reload mode

   TH0 = 0x00;                         // Make Timer0 wait the longest time
                                       // between LED blinks.
   TL0 = TH0;                          // Init Timer0

   // Keeper Timer0 off until the switch on the other multimaster device is
   // pushed
}

//-----------------------------------------------------------------------------
// Timer1_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer1 configured as the SMBus clock source as follows:
// - Timer1 in 8-bit auto-reload mode
// - SYSCLK or SYSCLK / 4 as Timer1 clock source
// - Timer1 overflow rate => 3 * SMB_FREQUENCY
// - The resulting SCL clock rate will be ~1/3 the Timer1 overflow rate
// - Timer1 enabled
//
//-----------------------------------------------------------------------------
void Timer1_Init(void)
{
// Make sure the Timer can produce the appropriate frequency in 8-bit mode
// Supported SMBus Frequencies range from 10kHz to 100kHz.  The CKCON register
// settings may need to change for frequencies outside this range.
#if ((SYSCLK/SMB_FREQUENCY/3) < 255)
   #define SCALE 1
      CKCON |= 0x08;                   // Timer1 clock source = SYSCLK
#elif ((SYSCLK/SMB_FREQUENCY/4/3) < 255)
   #define SCALE 4
      CKCON |= 0x01;
      CKCON &= ~0x0A;                  // Timer1 clock source = SYSCLK / 4
#elif ((SYSCLK/SMB_FREQUENCY/12/3) < 255)
   #define SCALE 8
      CKCON &= ~0x0B;                  // Timer1 clock source = SYSCLK / 12
#elif ((SYSCLK/SMB_FREQUENCY/48/3) < 255)
   #define SCALE 4
      CKCON |= 0x02;
      CKCON &= ~0x09;                  // Timer1 clock source = SYSCLK / 48
#else
   #error "Error: Timer1 cannot produce the desired SMBus frequency"
#endif

   TMOD = 0x20;                        // Timer1 in 8-bit auto-reload mode

   // Timer1 configured to overflow at 1/3 the rate defined by SMB_FREQUENCY
   TH1 = -(SYSCLK/SMB_FREQUENCY/SCALE/3);

   TL1 = TH1;                          // Init Timer1

   TR1 = 1;                            // Timer1 enabled
}

//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Timer3 configured for use by the SMBus low timeout detect feature as
// follows:
// - Timer3 in 16-bit auto-reload mode
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
//-----------------------------------------------------------------------------
void Timer3_Init(void)
{
   TMR3CN = 0x00;                      // Timer3 configured for 16-bit auto-
                                       // reload, low-byte interrupt disabled

   CKCON &= ~0x40;                     // Timer3 uses SYSCLK/12

   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect):
                                       // 1/.025 = 40

   EIE1 |= 0x80;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3
}

//-----------------------------------------------------------------------------
// Port_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Configure the Crossbar and GPIO ports.
//
// P0.0   digital   open-drain    SMBus SDA
// P0.1   digital   open-drain    SMBus SCL
//
// P1.0   digital   push-pull     LED1
//
// P1.7   digital   open-drain    S1 (Switch 1)
//
// all other port pins unused
//
// Note: If the SMBus is moved, the SCL and SDA sbit declarations must also
// be adjusted.
//
//-----------------------------------------------------------------------------
void Port_Init(void)
{
   P0MDOUT = 0x00;                     // All P0 pins open-drain output

   P1MDOUT |= 0x01;                    // Make LED1 a push-pull output

   XBR0 = 0x04;                        // Enable SMBus pins
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0 = 0xFF;
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Timer0 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// Timer0 blinks the LED when instructed by the other multimaster.
//
//-----------------------------------------------------------------------------
INTERRUPT(TIMER0_ISR, INTERRUPT_TIMER0)
{
   static U8 count = 0;                // Count up to 256 Timer 0 interrupts
                                       // before toggling LED1.
                                       // Otherwise, it won't be visible to
                                       // the eye.
   if(count == 255)
   {
      LED1 = !LED1;                    // Turn the LED on and off

      count = 0;
   }
   else
   {
      count++;
   }
}

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Master only implementation - no slave or arbitration states defined
// - Incoming master data is written to global variable <SMB_DATA_IN_MASTER>
// - Incoming slave data is written to global variable <SMB_DATA_IN_SLAVE>
// - Outgoing master data is read from global variable <SMB_DATA_OUT_MASTER>
// - Outgoing slave data is read from global variable <SMB_DATA_OUT_SLAVE>
//
//-----------------------------------------------------------------------------
INTERRUPT(SMBUS0_ISR, INTERRUPT_SMBUS0)
{
   bit M_FAIL = 0;                     // Used by the master states to flag a
                                       // bus error condition
                                       // transfers

   static bit ADDR_SEND = 0;           // Used by the ISR to flag byte
                                       // transmissions as slave addresses

   static bit arbitration_lost = 0;    // Used by the ISR to flag whether
                                       // arbitration was lost and the transfer
                                       // should be rescheduled

   // Normal operation
   switch (SMB0CN & 0xF0)              // Status vector
   {
      // MASTER ---------------------------------------------------------------

      // Master Transmitter/Receiver: START condition transmitted.
      case SMB_MTSTA:
         SMB0DAT = TARGET;             // Load address of the target slave
         SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
                                       // R/W bit
         SMB0DAT |= SMB_RW;            // Load R/W bit
         STA = 0;                      // Manually clear START bit
         ADDR_SEND = 1;

         break;

      // Master Transmitter: Data byte transmitted
      case SMB_MTDB:

         if(ARBLOST == 0)              // Check for a bus error
         {
            if(ACK)                    // Slave ACK?
            {
               if(ADDR_SEND)           // If the previous byte was a slave
               {                       // address,
                  ADDR_SEND = 0;       // Next byte is not a slave address
                  if (SMB_RW == WRITE) // If this transfer is a WRITE,
                  {
                     // Send data byte
                     SMB0DAT = SMB_DATA_OUT_MASTER;
                  }
                  else {}              // If this transfer is a READ,
                                       // proceed with transfer without
                                       // writing to SMB0DAT (switch
                                       // to receive mode)
               }
               else                    // If previous byte was not a slave
               {                       // address,
                  STO = 1;             // Set STO to terminate transfer
                  SMB_BUSY = 0;        // And free SMBus interface
               }
            }
            else                       // If slave NACK,
            {
               STO = 1;                // Send STOP condition, followed
               STA = 1;                // By a START
               NUM_ERRORS++;           // Indicate error
            }
         }
         else
         {
            M_FAIL = 1;                // If a bus error occurs, reset
         }

         break;

      // Master Receiver: byte received
      case SMB_MRDB:

         if(ARBLOST == 0)              // Check for a bus error
         {
            // Store received byte
            SMB_DATA_IN_MASTER = SMB0DAT;
            SMB_BUSY = 0;              // Free SMBus interface
            ACK = 0;                   // Send NACK to indicate last byte
                                       // of this transfer

            STO = 1;                   // Send STOP to terminate transfer
         }
         else
         {
            M_FAIL = 1;                // If a bus error occurs, reset
         }

         break;

      // ----------------------------------------------------------------------

      // SLAVE ----------------------------------------------------------------

      // Slave Receiver: Start+Address received
      case  SMB_SRADD:

         if (ARBLOST == 1)
         {
            arbitration_lost = 1;      // Indicate lost arbitration
         }

         STA = 0;                      // Clear STA bit
         if((SMB0DAT&0xFE) == (MY_ADDR&0xFE)) // Decode address
         {                             // If the received address matches,
            ACK = 1;                   // ACK the received slave address
            if((SMB0DAT&0x01) == READ) // If the transfer is a master READ
            {
               // Prepare outgoing byte
               SMB0DAT = SMB_DATA_OUT_SLAVE;
            }
         }
         else                          // If received slave address does not
         {                             // match,
            ACK = 0;                   // NACK received address
         }

         break;

      // Slave Receiver: Data received
      case  SMB_SRDB:

         if(ARBLOST == 0)              // No bus error
         {

            // Store incoming data
            SMB_DATA_IN_SLAVE = SMB0DAT;

            DATA_READY = 1;            // Indicate new data received
            ACK = 1;                   // ACK received data
         }
         else                          // Bus error detected
         {
            STA = 0;
            STO = 0;
            ACK = 0;
         }

         break;

      // Slave Receiver: Stop received while either a Slave Receiver or Slave
      // Transmitter
      case  SMB_SRSTO:

         if(arbitration_lost == 1)
         {
            STA = 1;                   // The ARBLOST bit indicated the master
                                       // lost arbitration
                                       // reschedule the transfer

            arbitration_lost = 0;
         }

         STO = 0;                      // STO must be cleared by software when
                                       // a STOP is detected as a slave
         break;

      // Slave Transmitter: Data byte transmitted
      case  SMB_STDB:
                                       // No action required;
                                       // one-byte transfers
                                       // only for this example
         break;

      // Slave Transmitter: Arbitration lost, Stop detected
      //
      // This state will only be entered on a bus error condition.
      // In normal operation, the slave is no longer sending data or has
      // data pending when a STOP is received from the master, so the TXMODE
      // bit is cleared and the slave goes to the SRSTO state.
      case  SMB_STSTO:

         STO = 0;                      // STO must be cleared by software when
                                       // a STOP is detected as a slave
         break;

      // Default: all other cases undefined

      // ----------------------------------------------------------------------

      default:
         M_FAIL = 1;                   // Indicate failed transfer
                                       // and handle at end of ISR
         break;

   } // end switch

   if(M_FAIL)                          // If the transfer failed,
   {
      SMB0CF &= ~0x80;                 // Reset communication
      SMB0CF |= 0x80;
      STA = 0;
      STO = 0;
      ACK = 0;

      SMB_BUSY = 0;                    // Free SMBus

      M_FAIL = 0;
      LED1 = LED_OFF;

      NUM_ERRORS++;                    // Indicate an error occurred
   }

   SI = 0;                             // Clear interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled here
//
//-----------------------------------------------------------------------------
INTERRUPT(TIMER3_ISR, INTERRUPT_TIMER3)
{
   SMB0CF &= ~0x80;                    // Disable SMBus
   SMB0CF |= 0x80;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
   STA = 0;
   SMB_BUSY = 0;                       // Free SMBus
}

//-----------------------------------------------------------------------------
// Support Functions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMB_Write
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Writes a single byte to the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Write outgoing data to the <SMB_data_out> variable
// 3) Call SMB_Write()
//
//-----------------------------------------------------------------------------
void SMB_Write(void)
{
   while(SMB_BUSY);                    // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   STA = 1;                            // Start transfer
}

//-----------------------------------------------------------------------------
// Blink_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Starts the LED blinking.
//
//-----------------------------------------------------------------------------
void Blink_LED(void)
{
   TR0 = 1;
}

//-----------------------------------------------------------------------------
// Stop_LED
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Stops the LED from blinking.
//
//-----------------------------------------------------------------------------
void Stop_LED(void)
{
   TR0 = 0;

   LED1 = LED_OFF;                     // Turn the LED off when it's not
                                       // blinking
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
