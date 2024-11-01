#include "driver_app.h"
#include "ms51_16k.h"

void timer1_counter_init(void)
{
	P0M1 &= ~(1 << 0);
  P0M2 &= ~(1 << 0);  

  /*external falling edge*/
  TMOD |= (1 << 6);

  /*GATE = 0*/
  TMOD &= ~(1 << 7);

  /*MODE 16bit, M1 = 0, M0 = 1*/
  TMOD |= (1 << 4);
  TMOD &= ~(1 << 5);

  /*TR0 = 1*/
  TCON |= (1 << 6);
}

unsigned int timer1_counter_read(void)
{
	unsigned int value_counter = 0;
	
	TCON &= ~(1 << 6);
  value_counter =  TL1 + (TH1 << 8);
  TCON |= (1 << 6);
	
	return value_counter;
}

void timer1_counter_reset(void)
{
	TCON &= ~(1 << 6);
	TL1 = 0;
  TH1 = 0;
	TCON |= (1 << 6);
}

void gpio_interrupt_init(void)
{
	// select port 0
  PICON &= ~(1 << 0);
  PICON &= ~(1 << 1);

  // Edge triggered
  PICON |= (1 << 3);

  // Low-level/falling edge detect Enabled
  PINEN |= (1 << 1);

  // Enable pin interrupt
  EIE |= (1 << 1);

  /*enable interrupt global*/
  IE |= (1 << 7);
}

void adc_init(void)
{
	/*enable ADC*/
	ADCCON1 |= (1 << 0); 
	
	/*Fadc = Fsys/1*/
	ADCCON1 &= ~(1 << 4);
	ADCCON1 &= ~(1 << 5);

	/*enable interrupt ADC*/
	IE |= (1 << 6);
	/*enable interrupt global*/
	IE |= (1 << 7);
}

void adc_start(void)
{
	ADCCON0 |= (1 << 6);
}

void adc_select_channel(ADC_CHANNEL_t adc_channel)
{
	// clear 4 bit
	ADCCON0 &= (0xF0);
	
	// chose channel
	ADCCON0 |= adc_channel & (0x0F);
}

ADC_CHANNEL_t adc_get_channel(void)
{
	ADC_CHANNEL_t adc_channel;
	
	adc_channel = (ADCCON0 & 0x0F);
	
	return adc_channel;
}

unsigned int adc_read(void)
{
	unsigned int value_adc = 0;
	
	value_adc = (unsigned int)(ADCRH << 4) | (ADCRL & 0x0f);
	
	return value_adc;
}

void timer3_init_for_task(unsigned int time_task_init)
{
  /*F = Fsys/16*/
	T3CON |= (1 << 2);

  RL3 = (65535 - time_task_init) & 0xff;
  RH3 = (65535 - time_task_init) >> 8;

  /*enable interupt tmr3*/
  EIE1 |= (1 << 1);

  /*enable interrupt global*/
  IE |= (1 << 7);

  /*START*/
  T3CON |= (1 << 3);
}

void i2c_init_interrupt(void)
{
	// config GPIO
	P1M1 |= (1 << 3);
	P1M2 |= (1 << 3); 
	
	P1M1 |= (1 << 4);
	P1M2 |= (1 << 4);  
	
//	P1M1 &= ~(1 << 3);
//	P1M2 &= ~(1 << 3);  
//	
//	P1M1 &= ~(1 << 4);
//	P1M2 &= ~(1 << 4);  
	
	P1S |= (1 << 3);
	P1S |= (1 << 4);
	
	// chose P1.3 1.4
	I2CON &= ~(1 << 0);
	
	// f = 100 kps
	I2CLK = 39; 
	
	// enable i2c bus
	I2CON |= (1 << 6);
	
	
//	// init time out 
//	I2TOC |= (1 << 2);
//	I2TOC |= (1 << 1);
	
	// enble interrup i2c
	EIE |= (1 << 0);
	IE |= (1 << 7);
}

