//-----------------------------------------------------------------------------
// F00x_SMBus_Master_Multibyte.c
//-----------------------------------------------------------------------------
// Copyright 2006 Silicon Laboratories, Inc.
// http://www.silabs.com
//
// Program Description:
//
// Example software to demonstrate the C8051F0xx ('F005 family) SMBus interface
// in Master mode.
// - Interrupt-driven SMBus implementation
// - Only master states defined (no slave or arbitration)
// - multiple-byte SMBus data holders used for each transmit and receive
// - Timer3 used by SMBus for SCL low timeout detection
// - SCL frequency defined by <SMB_FREQUENCY> constant
// - ARBLOST support included
// - supports multiple-byte writes and multiple-byte reads
// - Pinout:
//    P0.0 -> SDA (SMBus)
//    P0.1 -> SCL (SMBus)
//
//    P1.6 -> LED
//
//    all other port pins unused
//
// How To Test:
//
// 1) Download code to a 'F0xx device that is connected to a SMBus slave.
// 2) Run the code:
//         a) The test will indicate proper communication with the slave by
//            toggling the LED on and off each time a value is sent and
//            received.
//         b) The best method to view the proper functionality is to run to
//            the indicated line of code in the TEST CODE section of main and
//            view the SMB_DATA_IN and SMB_DATA_OUT variable arrays in the
//            Watch Window.
//
//
// FID:            00X000007
// Target:         C8051F0xx ('F005 TB)
// Tool chain:     Keil C51 7.50 / Keil EVAL C51
// Command Line:   None
//
// Release 1.0
//    -Initial Revision (TP)
//    -20 APR 2006
//

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------

#include <C8051F000.h>                 // SFR declarations

//-----------------------------------------------------------------------------
// Global CONSTANTS
//-----------------------------------------------------------------------------

#define  SYSCLK         4000000L       // System clock frequency in Hz

#define  SMB_FREQUENCY  10000L         // Target SCL clock rate
                                       // This example supports between 10kHz
                                       // and 100kHz

#define  WRITE          0x00           // WRITE direction bit
#define  READ           0x01           // READ direction bit

// Device addresses (7 bits, lsb is a don't care)
#define  SLAVE_ADDR     0xF0           // Device address for slave target

#define  MY_ADDR        0x02           // Address of this SMBus device
                                       // (dummy value since this device does
                                       // not have any defined slave states)

#define  SMB_BUS_ERROR  0x00           // (all modes) BUS ERROR
#define  SMB_START      0x08           // (MT & MR) START transmitted
#define  SMB_RP_START   0x10           // (MT & MR) repeated START
#define  SMB_MTADDACK   0x18           // (MT) Slave address + W transmitted;
                                       //    ACK received
#define  SMB_MTADDNACK  0x20           // (MT) Slave address + W transmitted;
                                       //    NACK received
#define  SMB_MTDBACK    0x28           // (MT) data byte transmitted;
                                       //    ACK rec'vd
#define  SMB_MTDBNACK   0x30           // (MT) data byte transmitted;
                                       //    NACK rec'vd
#define  SMB_MTARBLOST  0x38           // (MT) arbitration lost
#define  SMB_MRADDACK   0x40           // (MR) Slave address + R transmitted;
                                       //    ACK received
#define  SMB_MRADDNACK  0x48           // (MR) Slave address + R transmitted;
                                       //    NACK received
#define  SMB_MRDBACK    0x50           // (MR) data byte rec'vd;
                                       //    ACK transmitted
#define  SMB_MRDBNACK   0x58           // (MR) data byte rec'vd;
                                       //    NACK transmitted


#define  NUM_BYTES_WR   3              // Number of bytes to write
                                       // Master -> Slave
#define  NUM_BYTES_RD   3              // Number of bytes to read
                                       // Master <- Slave

//-----------------------------------------------------------------------------
// Global VARIABLES
//-----------------------------------------------------------------------------

// Global holder for SMBus data
// All receive data is written here
unsigned char SMB_DATA_IN[NUM_BYTES_RD];

// Global holder for SMBus data.
// All transmit data is read from here
unsigned char SMB_DATA_OUT[NUM_BYTES_WR];


unsigned char TARGET;                  // Target SMBus slave address

bit SMB_BUSY;                          // Software flag to indicate when the
                                       // SMB_Read() or SMB_Write() functions
                                       // have claimed the SMBus

bit SMB_RW;                            // Software flag to indicate the
                                       // direction of the current transfer

unsigned long NUM_ERRORS;              // Counter for the number of errors.

// 16-bit SFR declarations
sfr16    TMR3RL   = 0x92;              // Timer3 reload registers
sfr16    TMR3     = 0x94;              // Timer3 counter registers

sbit LED = P1^6;                       // LED on P1.6

sbit SDA = P0^0;                       // SMBus on P0.0
sbit SCL = P0^1;                       // and P0.1

//-----------------------------------------------------------------------------
// Function PROTOTYPES
//-----------------------------------------------------------------------------

void Port_Init(void);
void SMBus_Init(void);
void Timer3_Init(void);

void SMBus_ISR(void);
void Timer3_ISR(void);

void SMB_Write (void);
void SMB_Read (void);
void T0_Wait_ms (unsigned char ms);

//-----------------------------------------------------------------------------
// MAIN Routine
//-----------------------------------------------------------------------------

void MAIN (void)
{
   volatile unsigned char dat;         // Test counter
   volatile unsigned char data_count;  // SMB_DATA_IN and SMB_DATA_OUT counter
   unsigned char i;                    // Dummy variable counters

   WDTCN = 0xde;                       // Disable watchdog timer
   WDTCN = 0xad;

   OSCICN = 0x05;                      // Set internal oscillator to run
                                       // at 4 MHz

   // If slave is holding SDA low because of an improper SMBus reset or error
   while(!SDA)
   {
      // Provide clock pulses to allow the slave to advance out
      // of its current state. This will allow it to release SDA.
      XBR1 = 0x40;                     // Enable Crossbar
      SCL = 0;                         // Drive the clock low
      for(i = 0; i < 255; i++);        // Hold the clock low
      SCL = 1;                         // Release the clock
      while(!SCL);                     // Wait for open-drain
                                       // clock output to rise
      for(i = 0; i < 10; i++);         // Hold the clock high
      XBR1 = 0x00;                     // Disable Crossbar
   }

   Port_Init ();                       // Initialize Crossbar and GPIO

   // Turn off the LED before the test starts
   LED = 0;

   SMBus_Init ();                      // Configure and enable SMBus

   Timer3_Init ();                     // Configure and enable Timer3

   EIE1 |= 0x02;                       // Enable the SMBus interrupt

   EA = 1;                             // Global interrupt enable

   SI = 0;

// TEST CODE-------------------------------------------------------------------

   dat = 0;                            // Output data counter
   NUM_ERRORS = 0;                     // Error counter
   while (1)
   {
      // SMBus Write Sequence
      for (data_count = 0; data_count < NUM_BYTES_WR; data_count++)
      {
         SMB_DATA_OUT[data_count] = dat; // Define next outgoing byte
         dat++;
      }
      TARGET = SLAVE_ADDR;             // Target the Slave for next SMBus
                                       // transfer
      SMB_Write();                     // Initiate SMBus write

      // SMBus Read Sequence
      TARGET = SLAVE_ADDR;             // Target the Slave for next SMBus
                                       // transfer
      SMB_Read();

      // Check transfer data
      for (data_count = 0; data_count < NUM_BYTES_RD; data_count++)
      {
         // Received data match transmit data?
         if(SMB_DATA_IN[data_count] != SMB_DATA_OUT[data_count])
         {
            NUM_ERRORS++;              // Increment error counter if no
                                       // match
         }
      }

      // Indicate that an error has occurred (LED no longer lit)
      if (NUM_ERRORS > 0) {
         LED = 0;
      }
      else {
         LED = ~LED;
      }

      // Run to here to view the SMB_DATA_IN and SMB_DATA_OUT variable arrays

      T0_Wait_ms (1);
   }

// END TEST CODE---------------------------------------------------------------

}

//-----------------------------------------------------------------------------
// Initialization Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// PORT_Init
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
// P1.6   digital   push-pull     LED
//
// all other port pins unused
//
// Note: If the SMBus is moved, the SCL and SDA sbit declarations must also
// be adjusted.
//
void PORT_Init (void)
{
   PRT0CF = 0x00;                      // All P0 pins open-drain output

   PRT1CF |= 0x40;                     // Make the LED (P1.6) a push-pull
                                       // output

   XBR0 = 0x01;                        // Enable SMBus on the crossbar
   XBR2 = 0x40;                        // Enable crossbar and weak pull-ups

   P0 = 0xFF;
}

//-----------------------------------------------------------------------------
// SMBus_Init
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// The SMBus peripheral is configured as follows:
// - SMBus enabled
// - Assert Acknowledge low (AA bit = 1b)
// - Free and SCL low timeout detection enabled
//
void SMBus_Init (void)
{
   SMB0CN = 0x07;                      // Assert Acknowledge low (AA bit = 1b);
                                       // Enable SMBus Free timeout detect;
                                       // Enable SCL low timeout detect

   // SMBus clock rate (derived approximation from the Tlow and Thigh equations
   // in the SMB0CR register description)
   SMB0CR = 257 - (SYSCLK / (2 * SMB_FREQUENCY));

   SMB0ADR = MY_ADDR;                  // Set own slave address.

   SMB0CN |= 0x40;                     // Enable SMBus;
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
// - SYSCLK/12 as Timer3 clock source
// - Timer3 reload registers loaded for a 25ms overflow period
// - Timer3 pre-loaded to overflow after 25ms
// - Timer3 enabled
//
void Timer3_Init (void)
{
   TMR3CN = 0x00;                      // Timer3 uses SYSCLK/12

   TMR3RL = -(SYSCLK/12/40);           // Timer3 configured to overflow after
   TMR3 = TMR3RL;                      // ~25ms (for SMBus low timeout detect)

   EIE2 |= 0x01;                       // Timer3 interrupt enable
   TMR3CN |= 0x04;                     // Start Timer3
}

//-----------------------------------------------------------------------------
// Interrupt Service Routines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// SMBus Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// SMBus ISR state machine
// - Master only implementation - no slave or arbitration states defined
// - All incoming data is written to global array <SMB_DATA_IN>
// - All outgoing data is read from global array <SMB_DATA_OUT>
//
void SMBUS_ISR (void) interrupt 7
{
   bit FAIL = 0;                       // Used by the ISR to flag failed
                                       // transfers

   static unsigned char sent_byte_counter;
   static unsigned char rec_byte_counter;

   // Status code for the SMBus (SMB0STA register)
   switch (SMB0STA)
   {
      // Master Transmitter/Receiver: START condition transmitted.
      // Load SMB0DAT with slave device address.
      case SMB_START:

      // Master Transmitter/Receiver: repeated START condition transmitted.
      // Load SMB0DAT with slave device address
      case SMB_RP_START:
         SMB0DAT = TARGET;             // Load address of the slave.
         SMB0DAT &= 0xFE;              // Clear the LSB of the address for the
                                       // R/W bit
         SMB0DAT |= SMB_RW;            // Load R/W bit
         STA = 0;                      // Manually clear STA bit

         rec_byte_counter = 1;         // Reset the counter
         sent_byte_counter = 1;        // Reset the counter

         break;

      // Master Transmitter: Slave address + WRITE transmitted.  ACK received.
      // For a READ: N/A
      //
      // For a WRITE: Send the first data byte to the slave.
      case SMB_MTADDACK:

         SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
         sent_byte_counter++;

         break;

      // Master Transmitter: Slave address + WRITE transmitted.  NACK received.
      // Restart the transfer.
      case SMB_MTADDNACK:
         STA = 1;                      // Restart transfer
         break;

      // Master Transmitter: Data byte transmitted.  ACK received.
      // For a READ: N/A
      //
      // For a WRITE: Send all data.  After the last data byte, send the stop
      //  bit.
      case SMB_MTDBACK:

         if (sent_byte_counter <= NUM_BYTES_WR)
         {
            // send data byte
            SMB0DAT = SMB_DATA_OUT[sent_byte_counter-1];
            sent_byte_counter++;
         }
         else
         {
            STO = 1;                   // Set STO to terminate transfer
            SMB_BUSY = 0;              // And free SMBus interface
         }

         break;

      // Master Transmitter: Data byte transmitted.  NACK received.
      // Restart the transfer.
      case SMB_MTDBNACK:
         STA = 1;                      // Restart transfer

         break;

      // Master Receiver: Slave address + READ transmitted.  ACK received.
      // For a READ: check if this is a one-byte transfer. if so, set the
      //  NACK after the data byte is received to end the transfer. if not,
      //  set the ACK and receive the other data bytes.
      //
      // For a WRITE: N/A
      case SMB_MRADDACK:

         if (rec_byte_counter == NUM_BYTES_RD)
         {
            AA = 0;                    // Only one byte in this transfer,
                                       // send NACK after byte is received
         }
         else
         {
            AA = 1;                    // More than one byte in this transfer,
                                       // send ACK after byte is received
         }

         break;

      // Master Receiver: Slave address + READ transmitted.  NACK received.
      // Restart the transfer.
      case SMB_MRADDNACK:
         STA = 1;                      // Restart transfer

         break;

      // Master Receiver: Data byte received.  ACK transmitted.
      // For a READ: receive each byte from the slave.  if this is the last
      //  byte, send a NACK and set the STOP bit.
      //
      // For a WRITE: N/A
      case SMB_MRDBACK:

         if (rec_byte_counter < NUM_BYTES_RD)
         {
            SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT; // Store received byte
            AA = 1;                    // Send ACK to indicate byte received
            rec_byte_counter++;        // Increment the byte counter
         }
         else
         {
            AA = 0;                    // Send NACK to indicate last byte
                                       // of this transfer
         }

         break;

      // Master Receiver: Data byte received.  NACK transmitted.
      // For a READ: Read operation has completed.  Read data register and
      //  send STOP.
      //
      // For a WRITE: N/A
      case SMB_MRDBNACK:

         SMB_DATA_IN[rec_byte_counter-1] = SMB0DAT; // Store received byte
         STO = 1;
         SMB_BUSY = 0;
         AA = 1;                       // Set AA for next transfer

         break;

      // Master Transmitter: Arbitration lost.
      case SMB_MTARBLOST:

         FAIL = 1;                     // Indicate failed transfer
                                       // and handle at end of ISR

         break;

      // All other status codes invalid.  Reset communication.
      default:
         FAIL = 1;

         break;
   }

   if (FAIL)                           // If the transfer failed,
   {
      SMB0CN &= ~0x40;                 // Reset communication
      SMB0CN |= 0x40;
      STA = 0;
      STO = 0;
      AA = 0;

      SMB_BUSY = 0;                    // Free SMBus

      FAIL = 0;
   }

   SI = 0;                             // Clear interrupt flag
}

//-----------------------------------------------------------------------------
// Timer3 Interrupt Service Routine (ISR)
//-----------------------------------------------------------------------------
//
// A Timer3 interrupt indicates an SMBus SCL low timeout.
// The SMBus is disabled and re-enabled if a timeout occurs.
//
void Timer3_ISR (void) interrupt 14
{
   SMB0CN &= ~0x40;                    // Disable SMBus
   SMB0CN |= 0x40;                     // Re-enable SMBus
   TMR3CN &= ~0x80;                    // Clear Timer3 interrupt-pending flag
   SMB_BUSY = 0;                       // Free bus
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
// 2) Write outgoing data to the <SMB_DATA_OUT> array
// 3) Call SMB_Write()
//
void SMB_Write (void)
{
   while (SMB_BUSY);                   // Wait for SMBus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 0;                         // Mark this transfer as a WRITE
   STA = 1;                            // Start transfer
}

//-----------------------------------------------------------------------------
// SMB_Read
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   : None
//
// Reads a single byte from the slave with address specified by the <TARGET>
// variable.
// Calling sequence:
// 1) Write target slave address to the <TARGET> variable
// 2) Call SMB_Write()
// 3) Read input data from <SMB_DATA_IN> array
//
void SMB_Read (void)
{
   while (SMB_BUSY);                   // Wait for bus to be free.
   SMB_BUSY = 1;                       // Claim SMBus (set to busy)
   SMB_RW = 1;                         // Mark this transfer as a READ

   STA = 1;                            // Start transfer

   while (SMB_BUSY);                   // Wait for transfer to complete
}

//-----------------------------------------------------------------------------
// T0_Wait_ms
//-----------------------------------------------------------------------------
//
// Return Value : None
// Parameters   :
//   1) unsigned char ms - number of milliseconds to wait
//                        range is full range of character: 0 to 255
//
// Configure Timer0 to wait for <ms> milliseconds using SYSCLK as its time
// base.
//
void T0_Wait_ms (unsigned char ms)
{
   TCON &= ~0x30;                      // Stop Timer0; Clear TF0
   TMOD &= ~0x0f;                      // 16-bit free run mode
   TMOD |=  0x01;

   CKCON |= 0x08;                      // Timer0 counts SYSCLKs

   while (ms) {
      TR0 = 0;                         // Stop Timer0
      TH0 = -(SYSCLK/1000 >> 8);       // Overflow in 1ms
      TL0 = -(SYSCLK/1000);
      TF0 = 0;                         // Clear overflow indicator
      TR0 = 1;                         // Start Timer0
      while (!TF0);                    // Wait for overflow
      ms--;                            // Update ms counter
   }

   TR0 = 0;                            // Stop Timer0
}

//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
