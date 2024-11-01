#include "driver_app.h"
#include "USER_PROGRAM.INC"

#define ADDRESS_SLAVE		0xA0

void TIMER_Init() {
  _tmr = 255 - 125;  // 1us * 125 = 125us
  _ts = 0;           // fsys
  _tpsc2 = 1;
  _tpsc1 = 0;
  _tpsc0 = 0;  // ftp/16 = 1Mhz = 1us

  _ton = 1;  // timer/counter ENABLE
  _te = 1;   // enable timer interrupt
}

void I2C_init(void)
{
  _pac0 = 1;
  _pac2 = 1;
  _papu0 = 1;
  _papu2 = 1;
  _sim2 = 1;
  _sim1 = 1;
  _sim0 = 0;
  
  _simen = 1;
  
  _sima = ADDRESS_SLAVE  ;
  
  
  _i2ctos0 = 1;
  _i2ctos1 = 1;
  _i2ctos2 = 1;
  _i2ctos3 = 1;
  _i2ctos4 = 1;
  _i2ctos5 = 1;
  
  _i2ctoen = 1;
  
  _sime = 1;
  _emi = 1;
}

void IO_Init() {
  _acerl = 0;
  _papu = 0;
  _pbpu = 0;
  _pdpu = 0;

  _pac3 = 0;
  _pa3 = 0;

  _pac1 = 0;
  _pa1 = 0;

  _pdc0 = 0;
  _pd0 = 1;

  _pac4 = 0;
  _pa4 = 0;

  _pac7 = 1;
  _papu7 = 1;

  _pdc0 = 1;
  _pdpu0 = 1;
  //  _pd0 = 1;

  _pdc1 = 1;
  _pdpu1 = 1;
  //  _pd0 = 1;
}

void set_buzzer_on(unsigned int timer_for_buzzer)
{
	u16_timer_for_buzzer = timer_for_buzzer * 8;
}


void set_buzzer_on_forever()
{
	b_forever_buzzer = 1;
}

void set_buzzer_off_forever()
{
	b_forever_buzzer = 0;
}