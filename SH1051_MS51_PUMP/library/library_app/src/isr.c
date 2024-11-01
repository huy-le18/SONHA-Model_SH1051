#include "ms51_16k.h"
#include "driver_app.h"
#include "library_app.h"
// for intterupt 
#include <intrins.h>

#define C_1MS_CNT_VALUE 5
#define ADDRESS_SLAVE		0xA0
#define WRITE						0
#define READ						1

#define FLAG_WRITE					0
#define FLAG_READ						1

#define MAX_TRIAC_PULSE		      20
#define MAX_ADDITION_INTERVAL   10

#define MAX_12MS_CNT 60


// For 1ms Timer0 Interrupt use. To count global time 50us, 200us, 1ms, 10ms, 100ms, 500ms, 2s
volatile unsigned char	u8_flag_200us = 0, u8_flag_1ms = 0, u8_flag_2ms = 0, u8_flag_10ms = 0, u8_flag_50ms = 0;
volatile unsigned char u8_flag_100ms = 0, u8_flag_200ms = 0, u8_flag_500ms = 0, u8_flag_1s = 0, u8_flag_2s = 0; 
volatile unsigned int	u16_timer3_int_counter, u16_1ms_timer_counter, u16_2ms_timer_counter, u16_10ms_time_counter, u16_50ms_time_counter;
volatile unsigned int	u16_100ms_time_counter, u16_200ms_time_counter,  u16_500ms_time_counter,u16_1s_time_counter, u16_2s_time_counter;

volatile unsigned int  u16_adc_value = 0;
volatile unsigned char u8_flag_adc = 0;

volatile bit b_i2c_read_write = FLAG_WRITE;

extern volatile uint8_t m_tx_data[3];
extern volatile uint8_t m_rx_data[3];

volatile uint8_t u8_cnt_data_rx = 0;
volatile uint8_t u8_cnt_data_tx = 0;

volatile uint8_t addSlaveAndRW = ADDRESS_SLAVE|WRITE;

volatile bit b_flag_i2c_receive = 0;
volatile bit b_flag_i2c_transmit = 0;

volatile uint8_t u8_i2c_time_out = 0;

volatile uint8_t u8_detect_zero = 0;

volatile uint8_t count12ms = 0;

volatile uint8_t triac_pulse_cnt;
volatile uint8_t num_active_pulse;           // power from 0 to 20
volatile uint8_t num_active_pulse_with_add;  // from 0 to 20
volatile uint8_t pulse_trigger_off_cnt;
volatile bit b_triggered;
volatile uint8_t num_addition;
volatile uint8_t additionInterval;
volatile uint16_t u16NumActive;  // raw number of active pulse * 10


extern volatile bit b_test_cg;   // test chong giat
extern volatile bit b_reset_cg;  // reset chong giat
extern volatile uint8_t test_cg_cnt;
extern volatile uint8_t reset_cg_cnt;

void Timer3_ISR(void) interrupt 16        // Vector @  0x83
{
  _push_(SFRS);
  clr_T3CON_TF3;
	u16_timer3_int_counter ++;	
	u8_flag_200us = 1;
	
	//---1ms-------------------------------------------
	if(u16_timer3_int_counter >= C_1MS_CNT_VALUE){						//1ms
		u16_timer3_int_counter = 0;	
		u16_1ms_timer_counter ++;
		u8_flag_1ms = 1;
	}
	//---2ms-------------------------------------------
		if(u16_1ms_timer_counter >= 2){													//2ms
		u16_1ms_timer_counter = 0;
		u16_2ms_timer_counter ++;	
		u8_flag_2ms = 1;			
	}
		
	
	//---10ms-------------------------------------------
	if(u16_2ms_timer_counter >= 5){													//10ms
		u16_2ms_timer_counter = 0;
		u16_10ms_time_counter ++;	
		u8_flag_10ms = 1;		

		if (b_test_cg == 1) {
        if (test_cg_cnt < 17) {
          lo_write_tonggle();
          test_cg_cnt++;
        } else {
          b_test_cg = 0;
          test_cg_cnt = 0;
          lo_write_low();
        }
      }
			
			// reset chong giat
      if (b_reset_cg == 1) {
        if (reset_cg_cnt < 30) {
          reset_cg_cnt++;
          reset_lo_write_high();
        } else {
          b_reset_cg = 0;
          reset_cg_cnt = 0;
          reset_lo_write_low();
        }
      }
	}
	
	
		//---100ms------------------------------------------
	if(u16_10ms_time_counter >= 5){													//100ms
		u16_10ms_time_counter = 0;
		u16_50ms_time_counter ++;	
		u8_flag_50ms = 1;
	}
	
	
	//---100ms------------------------------------------
	if(u16_50ms_time_counter >= 2){													//100ms
		u16_50ms_time_counter = 0;
		u16_100ms_time_counter ++;	
		u8_flag_100ms = 1;
	}
	
	
	//---200ms------------------------------------------
	if(u16_100ms_time_counter >= 2){													//200ms
		u16_100ms_time_counter = 0;
		u16_200ms_time_counter ++;	
		u8_flag_200ms = 1;
	}
	
//	//---500ms------------------------------------------
//	if(u16_100ms_time_counter >= 5){													//500ms
//		u16_100ms_time_counter = 0;
//		u16_500ms_time_counter ++;	
//		u8_flag_500ms = 1;
//	}
	
	//---1s--------------------------------------------
	if(u16_200ms_time_counter >= 5){													//1s
		u16_200ms_time_counter = 0;
		u16_1s_time_counter ++;	
		u8_flag_1s = 1;
	}				
		//---2s--------------------------------------------
	if(u16_1s_time_counter >= 2){															//2s
		u16_1s_time_counter = 0;
		u16_2s_time_counter ++;	
		u8_flag_2s = 1;
	}	
	
	count12ms++;
  if (count12ms >= MAX_12MS_CNT) {
    count12ms = 0;
  }
  if (b_triggered == 1) {
    if (count12ms == pulse_trigger_off_cnt) {
			ctr_triac_write_high();
      b_triggered = 0;
    }
  }
	
  _pop_(SFRS);
}


// interrupt for ADC

void ADC_ISR(void) interrupt 11  // Vector @  0x5B
{
  _push_(SFRS);
  clr_ADCCON0_ADCF;
	u16_adc_value = adc_read();
	u8_flag_adc = 1;
  _pop_(SFRS);
}

void Pin_INT_ISR(void) interrupt 7  // Vector @  0x3B
{
  _push_(SFRS);
	
	  // index pulse
  if (triac_pulse_cnt < MAX_TRIAC_PULSE - 1) {
    triac_pulse_cnt++;
  } else {
    triac_pulse_cnt = 0;

    if (additionInterval < MAX_ADDITION_INTERVAL - 1) {
      additionInterval++;
    } else {
      additionInterval = 0;
    }
  }
	
	  // update num_active_pulse_with_add
  if (additionInterval < num_addition) {
    num_active_pulse_with_add = num_active_pulse + 1;
    if (num_active_pulse_with_add > 20) {
      num_active_pulse_with_add = 20;
    }
  } else {
    num_active_pulse_with_add = num_active_pulse;
  }
	
	
	if (u16NumActive > 0) 
	{
     if (triac_pulse_cnt < num_active_pulse_with_add)
		 {
			 ctr_triac_write_low();
       b_triggered = 1;  // triac triggerd
       pulse_trigger_off_cnt = count12ms;
     }
	 }

	
//	u8_detect_zero++;
	
  PIF = 0;
  _pop_(SFRS);
}



void I2C_ISR(void) interrupt 6  // Vector @  0x3B
{
	_push_(SFRS);
  SFRS = 0;
	if(I2CON & (1 << 3))
	{
		switch (I2STAT)
		{
				 /* Bus error */
			case 0x00: 
				I2C_SET_STOP();
				break;
			
				/* I2C start */
			case 0x08: 
				I2DAT = addSlaveAndRW;
				I2C_CLEAR_START();  
				I2C_CLEAR_SI();		
				break;
			
			/* I2C repeat start */
			case 0x10:  
	//			I2C_SET_STOP();	
				break;
			
			/* Master Transmit Address ACK  */
			case 0x18:
				I2DAT = m_tx_data[0]; 		
				break;
			
			/* Master Transmit Address NACK  */
			case 0x20:
				I2C_SET_STOP();	
				break;
			
			/* Master Transmit Data ACK  */
			case 0x28:   
				if(u8_cnt_data_tx == 0)
				{
					I2DAT = m_tx_data[1];
					u8_cnt_data_tx = 1;
				}
				else if(u8_cnt_data_tx == 1)
				{
					I2DAT = m_tx_data[2];
					u8_cnt_data_tx = 2;
				}
				else if(u8_cnt_data_tx == 2)
				{
					I2C_SET_STOP();	
					u8_cnt_data_tx = 0;
					b_i2c_read_write = FLAG_READ;
				}
				break;
			
			/* Master Transmit Data NACK  */
			case 0x30:
	//			I2DAT = 0x55;
				I2C_SET_STOP();
				break;
			
			/* Master Arbitration Lost  */
			case 0x38:
				I2C_SET_STOP();
				break;
			
			
			
			/* Master Receive Address ACK  */
			case 0x40:
				I2C_SET_AA();
				break;
			
			/* Master Receive Address NACK  */
			case 0x48:
				I2C_SET_STOP();
				I2C_SET_AA();
				break;
			
			/* Master Receive Data ACK  */
			case 0x50:
				if(u8_cnt_data_rx == 0)
				{
					m_rx_data[0] = I2DAT;
					u8_cnt_data_rx = 1;
					I2C_SET_AA();
				}
				else if (u8_cnt_data_rx == 1)
				{
					m_rx_data[1] = I2DAT;
					u8_cnt_data_rx = 0;
					I2C_CLEAR_AA();
				}
				break;
			
			/* Master Receive Data NACK  */
			case 0x58:
				m_rx_data[2] = I2DAT;
				b_i2c_read_write = FLAG_WRITE;
				b_flag_i2c_receive = 1;
				I2C_SET_STOP();
				I2C_SET_AA();
				break;
		}
			// clear SI
		I2C_CLEAR_SI();
	}
	_pop_(SFRS);
}


