#include "sdcard.h"
#include "loader.h"
volatile uint8_t read_status=0;
volatile uint8_t read_data=0;
__bit spi9_slow=1;
const __code uint8_t hextbl[]={'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'};
volatile uint8_t s[512];
int state = 0;
uint16_t ptr = 0;
volatile uint16_t ptr2 = 0;
uint32_t block = 0;

const __code uint16_t speech_table[] = {
  201, 215,  // 0
  32, 47,    // 1
  49, 63,    // 2
  71, 85,    // 3
  86, 103,   // 4
  108, 123,  // 5
  125, 139,  // 6
  145, 160,  // 7
  164, 181,  // 8
  184, 196,  // 9
  0, 5   // Music
};

void putc_uart0(uint8_t i){
  TI0=0;
  SBUF0=i;
  while(TI0==0);
  TI0=0;
}
void hexdump(uint8_t i){
  putc_uart0('0');
  putc_uart0('x');
  putc_uart0(hextbl[i>>4]);
  putc_uart0(hextbl[i&0x0f]);
  putc_uart0('\n');
  putc_uart0('\r');
  i=0;
}
void spi9_init(){
  XBR2|=0x80;
  P0MDIN  |= 0x04;
  P0MDOUT &=~0x04;
  P1MDIN  |= 0x60;
  P1MDOUT |= 0x60;
  SPI9_SCK=0;
  SPI9_MISO=1;
  SPI9_MOSI=0;
}
uint8_t spi9_read_fast(){
__asm
  mov a,0             //set a to 0
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  nop                 //delay
  setb  _SPI9_SCK     //SCK=1
  mov c,_SPI9_MISO    //read miso
  rlc a               //put it into bit0 of a
  clr _SPI9_SCK       //SCK=0
  mov dpl,a           //return value of a
  ret
__endasm;
return 0;
}
void spi9_write(uint8_t in){
  uint8_t out=0;
  uint8_t mask=0x80;
  while(mask){
    SPI9_SCK=0;
    SPI9_MOSI=in&mask?1:0;
    SPI9_SCK=1;
    mask=mask>>1;
    if(spi9_slow)
      delay_us(400);        
  }
  SPI9_SCK=0;
}
uint8_t spi9_read(){
  uint8_t out=0;
  uint8_t mask=0x80;
  if(!spi9_slow)
    return spi9_read_fast();
  while(mask){
    SPI9_SCK=0;
    SPI9_MOSI=1;
    SPI9_SCK=1;
    if(SPI9_MISO)
      out|=mask;
    mask=mask>>1;
    delay_us(400);        
  }
  SPI9_SCK=0;
  return out;
}
void sd_init_card() {
  SD_CS_OFF
  delay(70);
  for (uint8_t i = 20; i--;){
    spi9_write(0xff);
  }
  delay(70);
  //CMD0
  SD_CS_ON
  delay(70);
  cmd0();
  r1();
  //CMD8
  SD_CS_ON
  delay(5);
  cmd_crc(8, 0x000001AA, 0x87);
  r1();
  SD_CS_OFF
  for (int i = 0; i < 10; i++) {
    SD_CS_ON
    delay(1);
    cmd(55, 0);
    r1();
    cmd(41, 0x40000000UL);
    delay(300);
    if (r1() == 0) {
      spi9_slow=0;
      return;
    }
  }
  spi9_slow=0;
  return;
}
uint8_t r1() {
  uint8_t c = 0;
  uint16_t err = 0;
  while ((c = spi9_read()) == 0xff && err < 300)
    err++;
  return c;
}
uint8_t rb() {
  return spi9_read_fast();
}
void cmd0() {
  cmd_crc(0, 0, 0b10010101);
}
void cmd_crc(uint8_t n, uint32_t arg, uint8_t crc) {
  while (spi9_read() != 0xff);
  spi9_write(64 | n);
  spi9_write(0xff & (arg >> 24));
  spi9_write(0xff & (arg >> 16));
  spi9_write(0xff & (arg >> 8));
  spi9_write(0xff & arg);
  spi9_write(0x01 | crc);

}
inline void cmd(uint8_t n, uint32_t arg) {
  cmd_crc(n, arg, 0);
}
uint8_t rd(uint32_t block, uint8_t* data) {
  //SD_CS_ON
  rb();
  cmd(17, block);
  delay(100);
  if (r1() != 0) {
    //SD_CS_OFF
    return 1;
  }
  while (r1() != 0xfe); //Wait for SD card to read the block
  for (int i = 0; i < 512; i++) {
    uint8_t q=rb();
    data[i]=q;
  }
  rb();
  rb();
  //SD_CS_OFF
  return 0;
}
uint8_t wr(uint32_t block, uint8_t* data) {
  int cc = 0;
  uint8_t resp = 0xff;
  cmd(24, block);
  if (r1() != 0) {
    return 1;
  }
  spi9_write(0xfe);
  for (int i = 0; i < 514; i++) {
    spi9_write(data[i]);
  }
  if ((rb() & 0x1f ) != 0x5) {
    return 2;
  }
  while ((resp = rb()) != 0xff && cc < 1000) {
    cc++;
    delay(1);
  }
  return resp == 0xff ? 0 : 3;
}
void sd_update(){
    uint16_t dist = 0;
    /*if(block>30) {
      state=0;
      block=0;
      ptr=0;
    }*/
    if (ptr2 < ptr) {
      dist = ptr - ptr2;
    } else {
      dist = 512 - ptr2 + ptr;
    }
    if (dist < 200) {
      static uint16_t err = 0;
      switch (state) {
      case 0:
        rb();
        cmd(17, block);
        if (r1() == 0) {
          state = 1;
          err = 0;
        }
        ptr = 0;
        break;
      case 1:
        if (rb() == 0xfe && err < 300) { //Wait for SD card to read the block
          state = 2;
        } else if (err >= 300) {
          state = 0;
          err = 0;
        } else {
          err++;
        }
        break;
      case 2:
        s[ptr] = rb();
        ptr++;
        s[ptr] = rb();
        ptr++;
        s[ptr] = rb();
        ptr++;
        s[ptr] = rb();
        ptr++;
        if (ptr >= 512) {
          state = 3;
          err = 0;
        }
        break;
      case 3:
        if(loader_blocked()==0)
          if (rb() != 0xff && err < 300) {
            state = 0;
            block++;
          } else if (err >= 300) {
            state = 0;
            block++;
          } else {
            err++;
          }
      }
    }
}
uint32_t get_sound_start(uint16_t id) {
  return speech_table[id << 1];
}
uint32_t get_sound_end(uint16_t id) {
  return speech_table[(id << 1) + 1];
}
void say(uint16_t n) {
  uint16_t end = get_sound_end(n);;
  block = get_sound_start(n);
  state = 0;
  ptr = 0;
  ptr2 = 100;
  while (block <= end )
    sd_update();
}
void fs_beep(uint8_t t) {
  for (int i = 0; i < 512; i++) {
    s[i] = (i % 16 > 8) ? 0xff : 0x00;
  }
  delay(t);
}
INTERRUPT(TIMER0_ISR, INTERRUPT_TIMER0)
{
  rf_write(0x73, s[ptr2]);
  ptr2++;
  if (ptr2 >= 512) {
    ptr2 = 0;
  }
}
