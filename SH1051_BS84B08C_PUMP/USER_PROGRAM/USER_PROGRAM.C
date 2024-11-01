#include "USER_PROGRAM.INC"
#include "driver_app.h"
#include "..\SCAN_BUTTON\scan_button.h"
#include "..\GLOBE\GLOBE_VARIES.INC"
#include "main.h"
#include "..\TM1628\TM1628.h"


#define CONNECT_UI_BOARD_ERR_BIT 0
#define OUT_TEMP_HIGHT_ERR_BIT 1
#define FLOW_LOW_ERR_BIT 2
#define TESTING_CB_NOTIFY_BIT 3
#define ADC_OUTPUT_ERR_BIT 4
#define ADC_INPUT_ERR_BIT 5
#define PUMP_STATUS_BIT 6

#define MAX_SWITCH_TEMP 6


#define COM_TIMEOUT		20

volatile unsigned char buttonIndexArr[MAX_BUTTON];
unsigned char btnPressed;

#define IN 1
#define OUT 0
//#define ADDRESS_SLAVE		0xA0;

volatile unsigned char s_rx_data[3];
volatile unsigned char s_tx_data[3];

volatile unsigned char data_handle[2];
volatile unsigned char machine_state;

volatile unsigned char comm_timeout_cnt;
volatile unsigned char old_rx_data;
 
extern volatile bit b_flag_i2c_receive;
extern volatile bit b_flag_i2c_transmit;

bit b_tonggle_E2_in_mode_test;
bit b_tonggle_E3_in_mode_reset;
bit b_tonggle_temp_in_mode_power_on;
bit b_flag_button;
volatile bit b_flag_pump;

volatile unsigned char pulse_cur;
volatile unsigned char pulse_pre;
volatile unsigned char temp_set;
volatile unsigned char buff_dem;
volatile unsigned char flag_button;
unsigned char u8_temp_fb;

volatile bit b_switch_temp;
volatile unsigned char switch_temp_cnt;
volatile bit bit_7seg_temp_tonggle;
volatile unsigned char leg7seg_error_check;

volatile unsigned char time_check_test;
volatile unsigned char time_check_reset;

volatile bit bit_7seg_E2_tonggle;
volatile bit b_pump_status;

DeviceMode_t deviceMode = DEVICE_MODE_START;
DeviceModePower_t deviceModePower = DEVICE_MODE_POWER_OFF;

volatile bit bit_mode_power_on_normal;

pressBt_t g_pressBt;
//==============================================
//**********************************************
//==============================================

unsigned char CRC8(volatile unsigned char *data, unsigned char length);

void USER_PROGRAM_INITIAL() {
  GCC_DELAY(10);  //wait system stable
  TIMER_Init();
  BUZZER_OUTPUT();
  
  g_pressBt = NO_PRESS_BT;
  
  machine_state = 0;
  old_rx_data = 50;
  comm_timeout_cnt = 0;
  s_tx_data[0] = 0;
  s_tx_data[1] = 0;
  s_tx_data[2] = CRC8(s_tx_data, 2);
  
  TM1628_init();
  I2C_init();
  IO_Init();
  
  LED_UI_ON();
  setLedELCB();
  setLedPump();
  setSeg12(8, 8);
  update();
  
  deviceMode = DEVICE_MODE_START;

  b_tonggle_E2_in_mode_test = 0;
  b_tonggle_E3_in_mode_reset = 0;
  b_flag_button = 0;
  b_flag_pump = 0;
  
  temp_set = 40; 
  set_buzzer_on(500);
  
 }

//==============================================
//**********************************************
//==============================================

void USER_PROGRAM() {
  GCC_CLRWDT();
//  set_buzzer_on_forever();
  if(b_flag_i2c_receive == 1)
  {
  	if(s_rx_data[2] == CRC8(s_rx_data, 2))
  	{
  		data_handle[0] = s_rx_data[0];
  		data_handle[1] = s_rx_data[1];
  		machine_state = data_handle[0];
  		u8_temp_fb = data_handle[1];
  	}
  	
  	if(machine_state & (1 << TESTING_CB_NOTIFY_BIT))
	{
		if(s_tx_data[0]&(1 << CMD_TEST_BIT))
		{
			s_tx_data[0] &= ~(1 << CMD_TEST_BIT);
		}
	}
	else
	{
		if(s_tx_data[0]&(1 << CMD_RESET_BIT))
		{
			s_tx_data[0] &= ~(1 << CMD_RESET_BIT);
		}
	}
	
	
	if (machine_state & (1 << PUMP_STATUS_BIT)) {
      b_pump_status = 1;
		
      if (s_tx_data[0]&(1 << PUMP_ON_CMD)) {
        s_tx_data[0] &= ~(1 << PUMP_ON_CMD);
      }
    } else {
      b_pump_status = 0;

      if (s_tx_data[0]&(1 << PUMP_OFF_CMD)) {
        s_tx_data[0] &= ~(1 << PUMP_OFF_CMD);
      }
    }
    
    
	time_check_test ++;
  	if(time_check_test > 3)
	{
		time_check_test = 5;
	}
	
	time_check_reset ++;
	if(time_check_reset > 3)
	{
		time_check_reset = 5;
	}	
	
  	
  	b_flag_i2c_receive = 0;
  	comm_timeout_cnt = 0;
  }  
  
  
    
  if(b_flag_2ms == 1)
  {
  	b_flag_2ms = 0;
  	
  				/*process main programer*/
  	if(comm_timeout_cnt < COM_TIMEOUT)   // 2s
  	{
		switch (deviceMode)
		{
		  case DEVICE_MODE_START:
			mode_start_program();
			break;
		  case DEVICE_MODE_TEST:
			mode_test_program();
			break;
		  case DEVICE_MODE_POWER:
			mode_power_program();
			break;
		}
  	}
  	else
  	{
  	}
  }
  
  if(b_flag_10ms == 1)
  {
  	b_flag_10ms = 0;

	
  	if(g_pressBt == NO_PRESS_BT)
  	{
		switch (button_detect_press())
		{
	      case NO_FLAG_BT:
	      	break;
		  case FLAG_BT_TEST:
		  	s_tx_data[0] |= (1 << CMD_TEST_BIT);
		  	s_tx_data[2] = CRC8(s_tx_data, 2);
		  	set_buzzer_on(100);
		  	deviceMode = DEVICE_MODE_TEST;
		  	b_tonggle_E2_in_mode_test = 0;
		  	
		  	
		  	time_check_test = 0;
		  	
		    break;
		  case FLAG_BT_RESET:
		  	
		  	s_tx_data[0] |= (1 << CMD_RESET_BIT);
		  	s_tx_data[2] = CRC8(s_tx_data, 2);
		  	set_buzzer_on(100);
		  	deviceMode = DEVICE_MODE_POWER;
		  	b_tonggle_E3_in_mode_reset = 0;
		  	
		  	
		  	time_check_reset = 0;

		    break;
		  case FLAG_BT_PUMP:
		  	
		  	if(deviceMode == DEVICE_MODE_POWER)
		  	{
		  		if(deviceModePower == DEVICE_MODE_POWER_ON)
		  		{
		  			if((machine_state == 0) || (machine_state == (1 << PUMP_STATUS_BIT)) )
		  			{
		  				
			          if (b_pump_status == 0) {
			              // send pump on
			            s_tx_data[0] |= (1 << PUMP_ON_CMD);  // command
			            s_tx_data[2] = CRC8(s_tx_data, 2);
			          } else {
			            // send pump off
			            s_tx_data[0] |= (1 << PUMP_OFF_CMD);  // command
			            s_tx_data[2] = CRC8(s_tx_data, 2);
			          }
						  	
		  			}
		  		}
		  	}
			set_buzzer_on(100);
		    break;
		}
  	}
  }
  
  if(b_flag_100ms == 1)
  {
  	b_flag_100ms = 0;
	
  	g_pressBt = button_detect_release();
  	
  	comm_timeout_cnt++;
  	if(comm_timeout_cnt < COM_TIMEOUT)   // 2s
  	{
  		
  	}
  	else
  	{
  		setSeg12(0x0E, 0);
		update(); 
  	}
  }
  
  if(b_flag_500ms == 1)
  {
  	b_flag_500ms = 0;
  	
  }
  
}

void USER_PROGRAM_HALT_PREPARE() {
}

void USER_PROGRAM_HALT_WAKEUP() {
}

void USER_PROGRAM_RETURN_MAIN() {
}

////==============================================
////**********************************************
////==============================================


// CRC8 caculator
unsigned char CRC8(volatile unsigned char *data, unsigned char length){
  unsigned char crc = 0x00;
  unsigned char extract;
  unsigned char sum;
  unsigned char i;
  unsigned char tempI;
  for (i = 0; i < length; i++) {
    extract = *data;
    for (tempI = 8; tempI; tempI--) {
      sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
        crc ^= 0x8C;
      extract >>= 1;
    }
    data++;
  }
  return crc;
}



void mode_start_program(void)
{
	if(b_flag2msInStart == 1)
	{
		b_flag2msInStart = 0;
		
	}
	
  	if(b_flag10msInStart == 1)
	{
		b_flag10msInStart = 0;
		
	}
	
	if(b_flag100msInStart == 1)
	{
		b_flag100msInStart = 0;
		
	}
	if(b_flag500msInStart == 1)
	{
		b_flag500msInStart = 0;
	
	}
	
	
	if(b_flag2sInStart == 1)
	{
		b_flag2sInStart = 0;
		
		LED_UI_OFF();
		clear();
        update();
		deviceMode = DEVICE_MODE_POWER;
	}
	
	
}
void mode_test_program(void)
{
  	if(b_flag2msInTest == 1)
	{
		b_flag2msInTest = 0;
		
	}
	
  	if(b_flag10msInTest == 1)
	{
		b_flag10msInTest = 0;
		
	}
	
	if(b_flag100msInTest == 1)
	{
		b_flag100msInTest = 0;
		
	}
	if(b_flag500msInTest == 1)
	{
		b_flag500msInTest = 0;
		
		if (b_pump_status == 1) {
          // send pump off
          s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
          s_tx_data[2] = CRC8(s_tx_data, 2);
        }
        else
        {
        	clearLedPump();
		  	update();
        }
        
        
		if(time_check_test > 2)
		{
			
			if(machine_state & (1 << TESTING_CB_NOTIFY_BIT))
			{
				setLedELCB();
				clearSeg12();
		        update();
		        set_buzzer_off_forever();
			}
			else 
			{
				if(b_tonggle_E2_in_mode_test)
				{
				  LED_UI_ON();
		          setSeg12(0x0E, 2);
		          update();
				}
				else
				{
				  LED_UI_OFF();
				  clearSeg12();
		          update();	
				}
				b_tonggle_E2_in_mode_test = !b_tonggle_E2_in_mode_test;
				
				set_buzzer_on_forever();
			}
		}
        
	}
  
}
void mode_power_program(void)
{
  	if(b_flag2msInPower== 1)
	{
		b_flag2msInPower = 0;
		switch (deviceModePower)
		{
		  case DEVICE_MODE_POWER_OFF:
			mode_power_off_program();
			break;
		  case DEVICE_MODE_POWER_ON:
			mode_power_on_program();
			break;
		}
		
	}
	
  	if(b_flag10msInPower == 1)
	{
	    b_flag10msInPower = 0;
		
	    if (_pa7 == 0) {
	      if(machine_state & (1 << TESTING_CB_NOTIFY_BIT))
	      {
	      }
	      else
	      {
	      	b_flag_button = 1;
	      }
        }
	}
	
	if(b_flag100msInPower == 1)
	{
		b_flag100msInPower = 0;
		
		if (b_flag_button == 1) {
	      if (_pa7 == 1) {
	      	set_buzzer_on(200);
	      	if(deviceModePower == DEVICE_MODE_POWER_OFF)
	      	{
	      		deviceModePower = DEVICE_MODE_POWER_ON;
	      		LED_UI_ON();
	      		temp_set = 40;
	      		s_tx_data[1] = temp_set;
	      		s_tx_data[2] = CRC8(s_tx_data, 2);
	      		b_switch_temp = 1;
		        switch_temp_cnt = MAX_SWITCH_TEMP;
		        bit_7seg_temp_tonggle = 0;
	      	}
	      	else if(deviceModePower == DEVICE_MODE_POWER_ON)
	      	{
	      		deviceModePower = DEVICE_MODE_POWER_OFF;
	      		LED_UI_OFF();
	      		temp_set = 0;
	      		s_tx_data[1] = temp_set;
	      		s_tx_data[2] = CRC8(s_tx_data, 2);
	            if (b_pump_status == 1) {
	              // send pump off
	              s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
	              s_tx_data[2] = CRC8(s_tx_data, 2);
	            }
				
	      	}
	        b_flag_button = 0;
	      }
	    }
		
	}
	if(b_flag500msInPower == 1)
	{
		b_flag500msInPower = 0;

	}
}



void mode_power_off_program(void)
{
  
  if(b_flag500msInPowerOff == 1)
  {
	b_flag500msInPowerOff = 0;
	
    if (b_pump_status == 1) {
      // send pump off
      s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
      s_tx_data[2] = CRC8(s_tx_data, 2);
    }
    else
    {
    	clearLedPump();
		update();
    }
	            
	           
	if(time_check_reset > 2)
	{
		if(machine_state & (1 << TESTING_CB_NOTIFY_BIT))
		{
//			set_buzzer_on(550);
			if(b_tonggle_E3_in_mode_reset)
			{
				LED_UI_ON();
				setLedELCB();
		        setSeg12(0x0E, 2);
		        update();
			}
			else
			{
				LED_UI_OFF();
				clearLedELCB();
				clearSeg12();
		        update();	
			}
			b_tonggle_E3_in_mode_reset = !b_tonggle_E3_in_mode_reset;
			
			set_buzzer_on_forever();
		}
		else
		{
			LED_UI_OFF();	
			clear();
			update();	
			set_buzzer_off_forever();
		}
	}
  }
}
void mode_power_on_program(void)
{
	if(b_flag2msInPowerOn == 1)
	{
		b_flag2msInPowerOn = 0;
		
		 if ((machine_state == 0) || (machine_state == (1 << PUMP_STATUS_BIT))) {
		    pulse_cur = _pd1;
		    if (pulse_cur != pulse_pre) {
		      if (_pd0 != pulse_cur) {
		        buff_dem++;
		        if (buff_dem == 2) {
		          temp_set++;
		          if (temp_set > 55) {
		            temp_set = 55;
		          }
		          set_buzzer_on(100);
		          setSeg12(temp_set / 10, temp_set % 10);
		          update();
		          buff_dem = 0;
		          b_switch_temp = 1;
		          switch_temp_cnt = MAX_SWITCH_TEMP;
		          bit_7seg_temp_tonggle = 1;
		        }
		      } else {
		        buff_dem++;
		        if (buff_dem == 2) {
		          temp_set--;
		          if (temp_set < 30) {
		            temp_set = 30;
		          }
		          set_buzzer_on(100);
		          setSeg12(temp_set / 10, temp_set % 10);
		          update();
		          buff_dem = 0;
		          b_switch_temp = 1;
		          switch_temp_cnt = MAX_SWITCH_TEMP;
		          bit_7seg_temp_tonggle = 1;
		        }
		      }
		    }
		    pulse_pre = pulse_cur;
		    
		    s_tx_data[1] = temp_set;
	      	s_tx_data[2] = CRC8(s_tx_data, 2);
		 }	  
	}
	
	
  	if(b_flag10msInPowerOn == 1)
	{
	    b_flag10msInPowerOn = 0;
		

	}
	
	if(b_flag100msInPowerOn == 1)
	{
		b_flag100msInPowerOn = 0;
		
	}
	
	if(b_flag500msInPowerOn == 1)
	{
		b_flag500msInPowerOn = 0;
		
		
		if((machine_state == 0) || (machine_state == (1 << PUMP_STATUS_BIT)))
		{
			
		  clearLedELCB();
	   	  update();
	   	  
		  LED_UI_ON();
		  if ((b_switch_temp == 1) && (switch_temp_cnt > 0)) {
            if (bit_7seg_temp_tonggle == 1) {
              setSeg12(temp_set / 10, temp_set % 10);
              update();
              switch_temp_cnt--;
            } else {
              clearSeg12();
              update();
            }
            bit_7seg_temp_tonggle = !bit_7seg_temp_tonggle;
          }
          if ((switch_temp_cnt == 0)) {
            setSeg12(u8_temp_fb/10, u8_temp_fb%10);
            update();
          }		  
          
          	if(b_pump_status == 1)
			{
				setLedPump();
			  	update();	
			}
			else
			{
				clearLedPump();
			  	update();
			}
		
		  set_buzzer_off_forever();
		  b_tonggle_E3_in_mode_reset = 0;	
		}
		else if((machine_state & (1 << TESTING_CB_NOTIFY_BIT)) && (time_check_reset == 5))
		{
			if(b_tonggle_E3_in_mode_reset)
			{
				LED_UI_ON();
				setLedELCB();
		        setSeg12(0x0E, 2);
		        update();
			}
			else
			{
				LED_UI_OFF();
				clearLedELCB();
				clearSeg12();
		        update();	
			}
			
			if (b_pump_status == 1) {
             // send pump off
	          s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
	          s_tx_data[2] = CRC8(s_tx_data, 2);
	        }
	        else
	        {
	        	clearLedPump();
			  	update();
	        }
	        
			b_tonggle_E3_in_mode_reset = !b_tonggle_E3_in_mode_reset;
			b_switch_temp = 1;
			switch_temp_cnt = MAX_SWITCH_TEMP;
			bit_7seg_temp_tonggle = 0;
			
			set_buzzer_on_forever();
			
		}
		else if(machine_state & (1 << ADC_OUTPUT_ERR_BIT))
		{
		
			b_tonggle_E3_in_mode_reset = !b_tonggle_E3_in_mode_reset;
			if(b_tonggle_E3_in_mode_reset)
			{
			  LED_UI_ON();
	          setSeg12(0x0E, 1);
	          update();
			}
			else
			{
			  LED_UI_OFF();
			  clearSeg12();
	          update();	
			}
			
			if (b_pump_status == 1) {
             // send pump off
	          s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
	          s_tx_data[2] = CRC8(s_tx_data, 2);
	        }
	        else
	        {
	        	clearLedPump();
			  	update();
	        }
	        
			clearLedELCB();
	   	  	update();
	   	  	
			b_switch_temp = 1;
		    switch_temp_cnt = MAX_SWITCH_TEMP;
		    bit_7seg_temp_tonggle = 0;
		    set_buzzer_on_forever();
			
		}
		else if(machine_state & (1 << FLOW_LOW_ERR_BIT))
		{
		
			b_tonggle_E3_in_mode_reset = !b_tonggle_E3_in_mode_reset;
			if(b_tonggle_E3_in_mode_reset)
			{
			  set_buzzer_on(200);
			  LED_UI_ON();
	          setSeg12(0x0E, 3);
	          update();
			}
			else
			{
			  LED_UI_OFF();	
			  clearSeg12();
	          update();	
			}
			
			if (b_pump_status == 1) {
             // send pump off
	          s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
	          s_tx_data[2] = CRC8(s_tx_data, 2);
	        }
	        else
	        {
	        	clearLedPump();
			  	update();
	        }
	        
	        
			clearLedELCB();
	   	  	update();
	   	  	set_buzzer_off_forever();
			b_switch_temp = 1;
		    switch_temp_cnt = MAX_SWITCH_TEMP;
		    bit_7seg_temp_tonggle = 0;
		 
		}
		else if(machine_state & (1 << OUT_TEMP_HIGHT_ERR_BIT))
		{
		 
			b_tonggle_E3_in_mode_reset = !b_tonggle_E3_in_mode_reset;
			if(b_tonggle_E3_in_mode_reset)
			{
			  set_buzzer_on(200);
			  LED_UI_ON();
	          setSeg12(0x0E, 4);
	          update();
			}
			else
			{
			  LED_UI_OFF();	
			  clearSeg12();
	          update();	
			}
			
			if (b_pump_status == 1) {
             // send pump off
	          s_tx_data[0] |= (1 << PUMP_OFF_CMD); 
	          s_tx_data[2] = CRC8(s_tx_data, 2);
	        }
	        else
	        {
	        	clearLedPump();
			  	update();
	        }
			clearLedELCB();
	   	  	update();
	   	  	set_buzzer_off_forever();
			b_switch_temp = 1;
		    switch_temp_cnt = MAX_SWITCH_TEMP;
		    bit_7seg_temp_tonggle = 0;
		}
		
	}
}
