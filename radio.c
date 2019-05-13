#include "radio.h"
void spi1_init(){
  XBR1    |= 0x40;                    // Enable SPI1 (3 wire mode)
  P1MDOUT |= 0x15;                    // MOSI, SCK, NSS, push-pull
  SFRPAGE  = CONFIG_PAGE;
  P1DRV   |= 0x15;                    // MOSI, SCK, NSS, high current mode
  SFRPAGE  = LEGACY_PAGE;
  P2MDOUT |= 0x40;                    // SDN P2.6 push-pull
  SFRPAGE  = CONFIG_PAGE;
  P2DRV   |= 0x40;                    // SDN  P2.6 high-current mode
  SFRPAGE  = LEGACY_PAGE;
  SPI1CFG   = 0x40;   // master mode
  SPI1CN    = 0x00;   // 3 wire master mode
  SPI1CKR   = 0x00;   // Use SYSCLK/2
  SPI1CN   |= 0x01;   // enable SPI
  NSS1 = 1;           // set NSS high
}

void serial_init(){
  //0x92 - 115200; 0x2b - 57600
  TH1 = 0x96;        // Init Timer1 High register
  TL1 = TH1;         // Set the intial Timer1 value
  TMOD |= 0x20;       // Timer1 in 8-bit reload mode
  CKCON |= 0x08;      // Timer1 uses system clock
  ET1=0;             // Timer1 interrupt enabled
  TCON |= 0x40;       // Timer1 ON
  EA=1;              // Enable interrupts
  SCON0=0x10;
  ES0=1;
}
void delay (U16 time)
{
  U16 ticks;
  UU16 reload;
  while(time>0){
    if(time>10){
      ticks=10;
      time-=10;
    }else{
      ticks=time;
      time=0;
    }
    reload.U16 = -(ticks<<11);
    TR2 = 0;
    TF2H = 0;
    CKCON&=~0x10;
    TMR2RLL = reload.U8[LSB];
    TMR2RLH = reload.U8[MSB];
    TR2 = 1;
    while (!TF2H);
    TR2 = 0;
    TF2H = 0;
  }
}
void delay_us (U16 ticks)
{
  UU16 reload;
  reload.U16 = -(ticks<<1);
  TR2 = 0;
  TF2H = 0;
  CKCON&=~0x10;
  TMR2RLL = reload.U8[LSB];
  TMR2RLH = reload.U8[MSB];
  TR2 = 1;
  while (!TF2H);
  TR2 = 0;
  TF2H = 0;
}
void rf_init(){
  SDN = 0;
  for(uint8_t i=0;i<40;i++)
    delay(2);
  rf_setfreq(360000000L); 
  rf_write(EZRADIOPRO_TX_POWER, 0x18|3); // set bit 3 and 4
  rf_write(EZRADIOPRO_MODULATION_MODE_CONTROL_2, 0x00);
  rf_write(EZRADIOPRO_GPIO0_CONFIGURATION, 0x12);
  rf_write(EZRADIOPRO_GPIO1_CONFIGURATION, 0x15);
  rf_write(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_TXON|EZRADIOPRO_XTON));
  rf_write(EZRADIOPRO_GPIO2_CONFIGURATION,0b00001010);

  PT0=1;                  // Timer 0 interrupt pritority is high
  TMOD=(TMOD&0xf0)|0x02;  // Timer 0 is a 8-bit timer with auto-reload
  TH0=127;
  TL0=TH0;
  TR0=1;       
  ET0=1;                  // Enale Timer 0 interrupt*/

}
void rf_write(uint8_t reg, uint8_t value){
  // Send SPI data using double buffered write
  __critical{ 
  NSS1 = 0;                           // drive NSS low
  SPIF1 = 0;                          // clear SPIF
  SPI1DAT = (reg | 0x80);             // write reg address
  while(!TXBMT1);                     // wait on TXBMT
  SPI1DAT = value;                    // write value
  while(!TXBMT1);                     // wait on TXBMT
  while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
  SPIF1 = 0;                          // leave SPIF cleared
  NSS1 = 1;                           // drive NSS high
  }
}
uint8_t rf_read(uint8_t reg){
  U8 value;
  __critical{ 
  // Send SPI data using double buffered write
  NSS1 = 0;                           // drive NSS low
  SPIF1 = 0;                          // cleat SPIF
  SPI1DAT = ( reg );                  // write reg address
  while(!TXBMT1);                     // wait on TXBMT
  SPI1DAT = 0x00;                     // write anything
  while(!TXBMT1);                     // wait on TXBMT
  while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
  value = SPI1DAT;                    // read value
  SPIF1 = 0;                          // leave SPIF cleared
  NSS1 = 1;                           // drive NSS low
  }
  return value;
}
U32 util_divide(U32 value, U32 divisor)
{
   value += (divisor >> 1);
   value /= divisor;
   return value;
}
void rf_setfreq(U32 frequency)
{
   U8 frequencyBandSelect;
   UU16 nominalCarrierFrequency;

   if (frequency > 480000000L )
   {
      frequency -= 480000000L;
      frequencyBandSelect  = frequency / 20000000L;
      frequency -= (U32)frequencyBandSelect * 20000000L;
      frequency  = util_divide(frequency, 625);
      frequency <<= 1;
      frequencyBandSelect |= 0x20;
   }
   else
   {
      frequency -= 240000000L;
      frequencyBandSelect  = frequency / 10000000L;
      frequency -= (U32)frequencyBandSelect * 10000000L;
      frequency  = util_divide(frequency, 625);
      frequency <<= 2;
   }

   frequencyBandSelect |= 0x40;        // set sbsel

   nominalCarrierFrequency.U16 = (U16)frequency;

   rf_write(EZRADIOPRO_FREQUENCY_BAND_SELECT, frequencyBandSelect);
   rf_write(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1, nominalCarrierFrequency.U8[MSB]);
   rf_write(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0, nominalCarrierFrequency.U8[LSB]);

}
void rf_set_power(uint8_t pow){
  rf_write(EZRADIOPRO_TX_POWER, 0x18|pow); // set bit 3 and 4
}
