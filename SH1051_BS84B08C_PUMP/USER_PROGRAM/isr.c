#include "USER_PROGRAM.INC"
#include "driver_app.h"


#define VALUE_FOR_1MS		8

volatile bit	b_flag_200us = 0, b_flag_1ms = 0, b_flag_2ms = 0, b_flag_10ms = 0;
volatile bit    b_flag_100ms = 0, b_flag_500ms = 0, b_flag_1s = 0, b_flag_2s = 0;
volatile bit	b_flag2msInStart = 0, b_flag10msInStart = 0, b_flag100msInStart = 0, b_flag500msInStart = 0, b_flag1sInStart = 0, b_flag2sInStart = 0;
volatile bit	b_flag2msInTest = 0, b_flag10msInTest = 0, b_flag100msInTest = 0, b_flag500msInTest = 0;
volatile bit 	b_flag2msInPower = 0, b_flag10msInPower = 0, b_flag100msInPower = 0, b_flag500msInPower = 0;
volatile bit 	b_flag2msInPowerOn = 0, b_flag10msInPowerOn = 0, b_flag100msInPowerOn = 0, b_flag500msInPowerOn = 0;
volatile bit 	b_flag500msInPowerOff = 0;

volatile unsigned char	u8_timer_int_counter, u8_1ms_timer_counter, u8_2ms_timer_counter, u8_10ms_timer_counter;
volatile unsigned char	u8_100ms_timer_counter, u8_500ms_timer_counter,u8_1s_timer_counter, u8_2s_timer_counter;
volatile unsigned char	u8_data_i2c;
volatile unsigned char	u8_data1_i2c;

volatile unsigned int u16_timer_for_buzzer = 0;
volatile bit 	b_forever_buzzer = 0;

extern volatile unsigned char s_tx_data[3];
extern volatile unsigned char s_rx_data[3];

volatile unsigned char dat1 = 0;
volatile unsigned char dat2 = 0;

volatile unsigned char u8_cnt_data_rx = 0;
volatile unsigned char u8_cnt_data_tx = 0;

volatile bit b_flag_i2c_receive = 0;
volatile bit b_flag_i2c_transmit = 0;


void __attribute((interrupt(0x0C))) timer_ISR(void)  // 0.125ms (125us) TIMER interrupt service routine
{
  u8_timer_int_counter ++;
  
  if(u16_timer_for_buzzer > 0)
  {
  	u16_timer_for_buzzer --;
  	BUZZER_TONGGLE();
  }
  
  if(b_forever_buzzer == 1)
  {
  	BUZZER_TONGGLE();
  }
  
//  BUZZER_TONGGLE();
  
  if(u8_timer_int_counter >= VALUE_FOR_1MS)
  {
  	u8_timer_int_counter = 0;
  	u8_1ms_timer_counter++;
  	b_flag_1ms = 1;
  }
  
  if(u8_1ms_timer_counter >= 2)
  {
  	u8_1ms_timer_counter = 0;
  	u8_2ms_timer_counter++;
  	b_flag_2ms = 1;
  	b_flag2msInStart = 1;
    b_flag2msInTest = 1;
	b_flag2msInPower = 1;
	b_flag2msInPowerOn = 1;
  }
  
  if(u8_2ms_timer_counter >= 5)
  {
  	u8_2ms_timer_counter = 0;
  	u8_10ms_timer_counter++;
  	b_flag_10ms = 1;
  	b_flag10msInStart = 1;
    b_flag10msInTest = 1;
	b_flag10msInPower = 1;
	b_flag10msInPowerOn = 1;
  	
  }
  
  if(u8_10ms_timer_counter >= 10)
  {
  	u8_10ms_timer_counter = 0;
  	u8_100ms_timer_counter++;
  	b_flag_100ms = 1;
  	b_flag100msInStart = 1;
    b_flag100msInTest = 1;
	b_flag100msInPower = 1;
	b_flag100msInPowerOn = 1;
  	
  }
  
  if(u8_100ms_timer_counter >= 5)
  {
  	u8_100ms_timer_counter = 0;
  	u8_500ms_timer_counter++;
  	b_flag_500ms = 1;
  	b_flag500msInStart = 1;
    b_flag500msInTest = 1;
	b_flag500msInPower = 1;
	b_flag500msInPowerOn = 1;
	b_flag500msInPowerOff = 1;
  }
  
  if(u8_500ms_timer_counter >= 2)
  {
  	u8_500ms_timer_counter = 0;
  	u8_1s_timer_counter++;
  	b_flag_1s = 1;
  	b_flag1sInStart = 1;
  }
  
  if(u8_1s_timer_counter >= 2)
  {
  	u8_1s_timer_counter = 0;
  	u8_2s_timer_counter++;
  	b_flag_2s = 1;
  	b_flag2sInStart = 1;
  }
  
  _tf = 0;
}


void __attribute((interrupt(0x10))) i2c_ISR(void)
{
//	LED_TONGGLE();
	if(_i2ctof == 1)
	{
		_i2ctoen = 1;
		_i2ctof = 0;
	}
	else
	{
		if(_haas == 1)
		{
			if(_srw == 1)
			{
				
				_htx = 1;
				_simd = s_tx_data[0];	
			}
			else
			{
				_htx = 0;
				_txak = 0;
				dat1 = _simd;
			}
		}
		else
		{
			if(_htx == 1)   // 
	    	{
	    		if(_rxak == 1)
	    		{
	    			_htx = 0;
					_txak = 0;
					dat2 = _simd;
					
	    		}
	    		else
	    		{
					if(u8_cnt_data_tx == 0)
					{
						_simd = s_tx_data[1];
						u8_cnt_data_tx = 1;
					}
					else if (u8_cnt_data_tx == 1)
					{
						_simd = s_tx_data[2];
						u8_cnt_data_tx = 0;
						b_flag_i2c_transmit = 1;
					}
	    		}
	    	}
	    	else
	    	{
	    		if(u8_cnt_data_rx == 0)
	    		{
	    			s_rx_data[0] = _simd;
	    			u8_cnt_data_rx = 1;	
	    		}
	    		else if(u8_cnt_data_rx == 1)
	    		{
	    			s_rx_data[1] = _simd;
	    			u8_cnt_data_rx = 2;	
	    		}
	    		else if(u8_cnt_data_rx == 2)
	    		{
	    			s_rx_data[2] = _simd;
	    			u8_cnt_data_rx = 0;	
	    			b_flag_i2c_receive = 1;
	    		}
	    		
	    	}
		}
	}
	_simf = 0;
}