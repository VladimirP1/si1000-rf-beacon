#include "loader.h"
uint8_t loader_state=0;
uint16_t loader_ptr=0;
uint32_t loader_block=0;
uint8_t loader_buf[512];
uint8_t loader_blocked(){
  return (loader_state>1);
}
void loader_update(uint8_t c){
  switch(loader_state){
    case 0:
      if(c==0x41)
        loader_state=1;
    break;
    case 1:
      if(c==0x42)
        loader_state=2;
    break;
    case 2:
      if(c==0x43){
        rf_write(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,0); //Power off RF
        loader_state=3;
        loader_ptr=0;
        loader_block=0;
      }
    break;
    case 3:
      TR0=0;
      loader_buf[loader_ptr]=c;
      loader_ptr++;
      if(loader_ptr>=512){
        __critical{
          wr(loader_block,&loader_buf[0]);
          loader_ptr=0;
          loader_block++;
          putc_uart0(loader_block%26+65);
        }
      }
    break;
  }
}
