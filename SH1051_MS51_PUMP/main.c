/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* SPDX-License-Identifier: Apache-2.0                                                                     */
/* Copyright(c) 2020 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/
#include "main.h"

#define CYCLE_FOR_ELCB								15
#define CYCLE_FOR_RESET_ELCB					10
#define FLAG_WRITE					0
#define FLAG_READ						1

// define error send from master
#define CONNECT_UI_BOARD_ERR_BIT 0
#define OUT_TEMP_HIGHT_ERR_BIT 1
#define FLOW_LOW_ERR_BIT 2
#define TESTING_CB_NOTIFY_BIT 3
#define ADC_OUTPUT_ERR_BIT 4
#define ADC_INPUT_ERR_BIT 5
#define PUMP_STATUS_BIT 6

#define MAX_FLOW_SAMPLE		5
#define MAX_TIN_SAMPLE		14

unsigned int FULSE_Tset_to_55[MAX_TIN_SAMPLE][5] = 
{
//   23	  28   33   38   43
	{205, 270 ,290, 330, 350},   // 16
	{200, 246 ,275, 325, 360},   // 18
	{195, 236 ,263, 315, 350},   // 20
	{175, 230 ,260, 290, 320},   // 22
	{160, 210 ,240, 270, 310},   // 24
	{150, 190 ,230, 270, 310},   // 26
//	{140, 160 ,200, 220, 260},   // 28
	{140, 175 ,210, 240, 280},   // 28
	{130, 160 ,190, 210, 250},   // 30
	{115, 145 ,180, 205, 240},   // 32
	{110, 135 ,170, 190, 220},   // 34
	{100, 120 ,150, 170, 190},   // 36
	{85 , 110 ,130, 150, 170},   // 38
	{65 , 95  ,115, 125, 140},   // 40
	{50 , 75  ,95 , 115, 135}    // 42
};

unsigned char Flow_sample_arr[5] = {23, 28, 33, 38, 43};
unsigned char Tin_sample_arr[MAX_TIN_SAMPLE] = {16,18,20, 22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42};

unsigned char Flow_sample_index_l;  // luu chi so thap
unsigned char Flow_sample_index_h;  // luu chi so cao

unsigned char Tin = 23;
unsigned char Flow_in = 26;
unsigned char Tset = 40;

float F1, F2, F3, F4, F;
unsigned int fulse;
unsigned char delta_t_h;
unsigned char delta_t_l;
unsigned char i = 0;

volatile bit b_boost;

unsigned char u8_flag2msInTest = 0, u8_flag10msInTest = 0, u8_flag100msInTest = 0, u8_flag500msInTest = 0;
unsigned char u8_flag2msInPower = 0, u8_flag10msInPower = 0, u8_flag100msInPower = 0, u8_flag500msInPower = 0;
unsigned char u8_flag2msInPowerOnErr = 0, u8_flag10msInPowerOnErr = 0, u8_flag100msInPowerOnErr = 0, u8_flag500msInPowerOnErr = 0;
unsigned int g_u16CounterFlow = 0;
unsigned int g_adcNtcIn = 0;
unsigned int g_adcNtcOut = 0;
unsigned char u8_elcbCounter = 0;
unsigned char u8_restElcbCounter = 0;
extern volatile unsigned char u8_value;
extern volatile bit b_i2c_read_write;
extern volatile bit b_flag_i2c_receive;
extern volatile bit b_flag_i2c_transmit;
extern volatile uint8_t u8_i2c_time_out;

extern volatile bit b_pump_on;


volatile uint8_t machine_state = 0;
//powerMode_t g_powerMode = POWER_MODE_OFF;

DeviceMode_t deviceMode = DEVICE_MODE_POWER;
DeviceModePower_t deviceModePower = DEVICE_MODE_POWER_OFF;
DeviceModePowerOn_t deviceModePowerOn = DEVICE_MODE_POWER_ON_NORMAL; 

bit b_pump_on = 0;

volatile uint8_t cnt_clk = 0;
volatile uint8_t data_rx;
volatile uint8_t m_tx_data[3];
volatile uint8_t m_rx_data[3];
volatile uint8_t temp_out;
volatile uint8_t temp_in;
volatile uint8_t temp_set;
volatile unsigned char data_handle[2];

//volatile bit b_last_state = 1;
//volatile bit b_state = 0;
//volatile bit b_data = 0;
//volatile bit b_test = 0;
//volatile bit b_reset = 0;
//volatile bit b_cg_in_mode_test = 0;
//volatile bit b_cg_in_mode_power = 0;

volatile bit b_test_cg;   // test chong giat
volatile bit b_reset_cg;  // reset chong giat
volatile uint8_t test_cg_cnt;
volatile uint8_t reset_cg_cnt;


uint8_t datas;

void main(void) {
	uart_sw_init();
	task_init();
	flow_counter_init();
	ntc_in_adc_init();
	ntc_out_adc_init();
	ntc_in_select_channel();
	
	lo_output_init();
	reset_lo_output_init();
	int_os_input_init();
	ctr_outac_output_init();
	ctr_triac_output_init();
	ctr_pump_output_init();
	
	lo_write_low();
	reset_lo_write_low();
	ctr_outac_write_low();
//	ctr_triac_write_low();
	ctr_triac_write_high();
	ctr_pump_write_low();
	
	i2c_init_interrupt();
	gpio_interrupt_init();
	
	uart_sw_send_string("le danh huy", 13);
	
	
  while (1) 
	{
		
		if(u8_flag_200us == 1)
		{
			u8_flag_200us = 0;
		}
		if(u8_flag_1ms == 1)
		{
			u8_flag_1ms = 0;
		}
		if(u8_flag_2ms == 1)
		{
			u8_flag_2ms = 0;
			
			
			/*check flag adc*/
			if(u8_flag_adc == 1)
			{
				u8_flag_adc = 0;
				
				switch (adc_get_ntc_in_or_ntc_out())
				{
					case ADC_CHANNEL_NTC_IN:
						g_adcNtcIn = convert12BitsTo10Bit(u16_adc_value);
						ntc_out_select_channel();
						break;
					case ADC_CHANNEL_NTC_OUT:
						g_adcNtcOut = convert12BitsTo10Bit(u16_adc_value);
						ntc_in_select_channel();
						break;
					default:
						break;
				}
				
				
			}
			
			if ((g_adcNtcIn > ADC_HIGH_LIMIT) || (g_adcNtcIn < ADC_LOW_LIMIT)) {
				machine_state |= (1 << ADC_INPUT_ERR_BIT);

			} else {
				machine_state &= ~(1 << ADC_INPUT_ERR_BIT);
			}
			
			
			if ((g_adcNtcOut > ADC_HIGH_LIMIT) || (g_adcNtcOut < ADC_LOW_LIMIT)) {
				machine_state |= (1 << ADC_OUTPUT_ERR_BIT);

			} else {
				machine_state &= ~(1 << ADC_OUTPUT_ERR_BIT);
			}
			
			
			if(g_adcNtcIn < 384) 
			{
				g_adcNtcIn = 384;
			}
			
			if(g_adcNtcOut < 384) 
			{
				g_adcNtcOut = 384;
			}
			/*handle temperature*/
			getTempFromTable((uint16_t)g_adcNtcIn, &temp_in);
			getTempFromTable((uint16_t)g_adcNtcOut, &temp_out);
			
			
			if ((temp_out > TEMP_MIN_TO_PROTECT)) {
				machine_state |= (1 << OUT_TEMP_HIGHT_ERR_BIT);

			} else {
				machine_state &= ~(1 << OUT_TEMP_HIGHT_ERR_BIT);
			}
			
			
			/*process main programer*/
			switch (deviceMode)
			{
				case DEVICE_MODE_START:

					break;
				case DEVICE_MODE_TEST:
					if(int_os_read_state() == 0)
					{
						ctr_outac_write_low();
//						ctr_pump_write_low();
//						machine_state &= ~(1 << PUMP_STATUS_BIT);
//						b_pump_on = 0;
						
						
						machine_state |= (1 << TESTING_CB_NOTIFY_BIT);
//						b_cg_in_mode_test = 1;
					}
					else
					{
						machine_state &= ~(1 << TESTING_CB_NOTIFY_BIT);
//						b_cg_in_mode_test = 0;
					}
					break;
				case DEVICE_MODE_POWER:

					if(int_os_read_state() == 0)
					{
						ctr_outac_write_low();
						fulse = 0;
						u16NumActive = 0;
						num_active_pulse = 0;
						num_addition = 0;
						
//						ctr_pump_write_low();
//						machine_state &= ~(1 << PUMP_STATUS_BIT);
//						b_pump_on = 0;
						
						machine_state |= (1 << TESTING_CB_NOTIFY_BIT);
//						b_cg_in_mode_power = 1;
						
						
					}
					else
					{
						machine_state &= ~(1 << TESTING_CB_NOTIFY_BIT);
//						b_cg_in_mode_power = 0;
					}
					break;
			}
		}
		
		if(u8_flag_10ms == 1)
		{
			u8_flag_10ms = 0;
			
			if(b_flag_i2c_receive == 1)
			{
				if(m_rx_data[2] == CRC8(m_rx_data, 2))
				{
					data_handle[0] = m_rx_data[0];
					data_handle[1] = m_rx_data[1];
					 
					temp_set = data_handle[1];
					if(data_handle[0] & (1 << CMD_TEST_BIT))
					{
				
						deviceMode = DEVICE_MODE_TEST;
						b_test_cg = 1;
						
						ctr_outac_write_low();
						
					}
					
					if(data_handle[0] & (1 << CMD_RESET_BIT))
					{
						deviceMode = DEVICE_MODE_POWER;
						b_reset_cg = 1;

					}
					
					if(data_handle[0] & (1 << PUMP_ON_CMD))
					{
						ctr_pump_write_high();
						machine_state |= (1 << PUMP_STATUS_BIT);
					}
					
					if(data_handle[0] & (1 << PUMP_OFF_CMD))
					{
						ctr_pump_write_low();
						machine_state &= ~(1 << PUMP_STATUS_BIT);
					}
					
					
					if(temp_set == 0)
					{
						deviceModePower = DEVICE_MODE_POWER_OFF;
					}
					else if(temp_set > 0)
					{
						deviceModePower = DEVICE_MODE_POWER_ON;
					}

					b_flag_i2c_receive = 0;
				}
			
			}
			
			
			switch (deviceMode)
			{
				case DEVICE_MODE_START:

					break;
				case DEVICE_MODE_TEST:
					
					fulse = 0;
					u16NumActive = 0;
					num_active_pulse = 0;
					num_addition = 0;
				
					ctr_outac_write_low();
					
					
					break;
				case DEVICE_MODE_POWER:
					
					break;
			}
			
			/*check button press*/
		}
		
		if(u8_flag_50ms == 1)
		{
			u8_flag_50ms = 0;
			
			if(b_i2c_read_write == FLAG_WRITE)
			{
				m_tx_data[0] = machine_state;
				m_tx_data[1] = temp_out;
				m_tx_data[2] = CRC8(m_tx_data, 2);	
				i2c_master_write();
			}
			else if (b_i2c_read_write == FLAG_READ)
			{
				i2c_master_read();
			}
			I2C_SET_START();
			I2C_CLEAR_SI();
		}
		
//		if(u8_flag_100ms == 1)
//		{
//			u8_flag_100ms = 0;
//		}
		
		if(u8_flag_200ms == 1)
		{
			u8_flag_200ms = 0;
	
//			uart_sw_send_number(data_handle[0]);
//			uart_sw_send_byte('\n');
			
			switch (deviceMode)
			{
				case DEVICE_MODE_START:

					break;
				case DEVICE_MODE_TEST:
					
					break;
				case DEVICE_MODE_POWER:
					
					switch (deviceModePower)
					{
						case DEVICE_MODE_POWER_OFF:
							
						
							fulse = 0;
							u16NumActive = 0;
							num_active_pulse = 0;
							num_addition = 0;
							ctr_outac_write_low();
						
						
							break;
						case DEVICE_MODE_POWER_ON:
			
							if(machine_state & (1 << OUT_TEMP_HIGHT_ERR_BIT)
								|| machine_state & (1 << FLOW_LOW_ERR_BIT)
								|| machine_state & (1 << TESTING_CB_NOTIFY_BIT)
								|| machine_state & (1 << ADC_OUTPUT_ERR_BIT)
								|| machine_state & (1 << ADC_INPUT_ERR_BIT))
							{
								deviceModePowerOn = DEVICE_MODE_POWER_ON_ERR;
							}
							else
							{
								deviceModePowerOn = DEVICE_MODE_POWER_ON_NORMAL;
							}
						
							switch (deviceModePowerOn)
							{
								case DEVICE_MODE_POWER_ON_NORMAL:
									if(int_os_read_state() == 0)
									{
										ctr_outac_write_low();
										fulse = 0;
										u16NumActive = 0;
										num_active_pulse = 0;
										num_addition = 0;
									
										machine_state |= (1 << TESTING_CB_NOTIFY_BIT);
//										b_cg_in_mode_power = 1;
										
									}
									else
									{
										ctr_outac_write_high();
										machine_state &= ~(1 << TESTING_CB_NOTIFY_BIT);
//										b_cg_in_mode_power = 0;
										mode_power_on_normal_program();
									}
									
									break;
								case DEVICE_MODE_POWER_ON_ERR:
									fulse = 0;
									u16NumActive = 0;
									num_active_pulse = 0;
									num_addition = 0;
							  	ctr_outac_write_low();
									
									break;
							}
							
							break;
					}
			
					break;
			}
			
		}
//		if(u8_flag_500ms == 1)
//		{
//			u8_flag_500ms = 0;
//			
//			uart_sw_send_byte('\n');
//			uart_sw_send_number(u16NumActive);
//			uart_sw_send_byte('\n');
////			u8_detect_zero = 0;
//			uart_sw_send_number(num_active_pulse);
//			uart_sw_send_byte('\n');
//			
//			uart_sw_send_number(num_addition);
//			uart_sw_send_byte('\n');
//		}
		if(u8_flag_1s == 1)
		{
			u8_flag_1s = 0;
			
			g_u16CounterFlow = flow_counter_read();
//  			g_u16CounterFlow = 30;
			flow_counter_reset();
			
			if(g_u16CounterFlow < FLOW_MIN_TO_PROTECT)
			{
				machine_state |= (1 << FLOW_LOW_ERR_BIT);
			}
			else
			{
				machine_state &= ~(1 << FLOW_LOW_ERR_BIT);
			}
			
//			if(int_os_read_state() == 0)
//			{
//				uart_sw_send_string("co", 2);
//				uart_sw_send_byte('\n');
//			}
//			else
//			{
//				uart_sw_send_string("khong", 5);
//				uart_sw_send_byte('\n');
//			}
			
//			uart_sw_send_byte('\n');
//			machine_state
//			uart_sw_send_number(machine_state);
//			uart_sw_send_byte('\n');
//			
			uart_sw_send_number(data_handle[0]);
			uart_sw_send_byte('\n');
			uart_sw_send_number(data_handle[1]);
			uart_sw_send_byte('\n');
			uart_sw_send_number(m_rx_data[2]);
			uart_sw_send_byte('\n');
			uart_sw_send_byte('\n');

//			uart_sw_send_string("NtcI:",5);
//			uart_sw_send_number(g_adcNtcIn);
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("NtcO:",5);
//			uart_sw_send_number(g_adcNtcOut);
//			uart_sw_send_byte('\n');
//			uart_sw_send_number(g_u16CounterFlow);
//			uart_sw_send_byte('\n');
//			
//			uart_sw_send_string("tempi:",6);
//			uart_sw_send_number(temp_in);
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("tempo:",6);
//			uart_sw_send_number(temp_out);
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("temps:",6);
//			uart_sw_send_number(temp_set);
//			uart_sw_send_byte('\n');
//			uart_sw_send_number(u16NumActive);
//			uart_sw_send_byte('\n');
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("ntc:",4);
//			uart_sw_send_number(g_adcNtcValue);
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("knob:",5);
//			uart_sw_send_number(g_adcKnobValue);
//			uart_sw_send_byte('\n');
//			uart_sw_send_string("mode:",5);
//			uart_sw_send_number(g_powerMode);
//			uart_sw_send_byte('\n');
		}
		if(u8_flag_2s == 1)
		{
			u8_flag_2s = 0;
		}
	}
}


void mode_power_on_normal_program(void)
{
	      // calculate raw active pulse
      if (temp_set > 0) {
        /* Compute new control signal */

        Flow_in = g_u16CounterFlow;
        Tin = temp_in;
				
        for (i = 0; i < 5; i++) {
          if (Flow_in < Flow_sample_arr[0]) {
            break;
          } else if ((Flow_in >= Flow_sample_arr[i]) && (Flow_in < Flow_sample_arr[i + 1])) {
            Flow_sample_index_l = i;
            Flow_sample_index_h = i + 1;
            break;
          } else if ((Flow_in >= Flow_sample_arr[4])) {
            Flow_sample_index_l = 3;
            Flow_sample_index_h = 4;
          }
        }

        for (i = 0; i < MAX_TIN_SAMPLE; i++) {
          if (Tin < Tin_sample_arr[0]) {
            F1 = FULSE_Tset_to_55[0][Flow_sample_index_l];
            F2 = FULSE_Tset_to_55[0][Flow_sample_index_h];
            break;
          } else if ((Tin >= Tin_sample_arr[i]) && (Tin < Tin_sample_arr[i + 1])) {
            delta_t_l = FULSE_Tset_to_55[i][Flow_sample_index_l] - FULSE_Tset_to_55[i + 1][Flow_sample_index_l];

            delta_t_h = FULSE_Tset_to_55[i][Flow_sample_index_h] - FULSE_Tset_to_55[i + 1][Flow_sample_index_h];

            F1 = FULSE_Tset_to_55[i][Flow_sample_index_l] - (Tin - Tin_sample_arr[i]) * delta_t_l / 2.0;

            F2 = FULSE_Tset_to_55[i][Flow_sample_index_h] - (Tin - Tin_sample_arr[i]) * delta_t_h / 2.0;

            break;
          } else if ((Tin >= Tin_sample_arr[MAX_TIN_SAMPLE - 1])) {
            F1 = FULSE_Tset_to_55[MAX_TIN_SAMPLE - 1][Flow_sample_index_l];
            F2 = FULSE_Tset_to_55[MAX_TIN_SAMPLE - 1][Flow_sample_index_h];
          }
        }

        F4 = F1 - (F1 - F2) * (Flow_in - Flow_sample_arr[Flow_sample_index_l]) / 5.0;
        F = (temp_set - Tin) * F4 * 10 / (55 - Tin);
        if ((int)F % 10 >= 5) {
          fulse = F / 10 + 1;
        } else {
          fulse = F / 10;
        }

        if (temp_set > temp_in) {
          u16NumActive = fulse;
        } else
          u16NumActive = 0;

        if ((temp_out > temp_set)) {
          u16NumActive = u16NumActive * 9 / 10;
        }
        if ((temp_out < temp_set)) {
          if (temp_out < temp_set - 12) {
            b_boost = 1;	
          }
          if (temp_out < temp_set - 6) {
            if (b_boost == 1) u16NumActive = u16NumActive * 14 / 10;
          } else if (temp_out < temp_set - 1) {
            if (b_boost == 1) u16NumActive = u16NumActive * 12 / 10;
          } else {
            b_boost = 0;
            u16NumActive = u16NumActive * 11 / 10;
          }
        }

        if (u16NumActive > 200) u16NumActive = 200;
				
        num_active_pulse = u16NumActive / 10;
        num_addition = u16NumActive - (num_active_pulse * 10);

        if (num_active_pulse > 20) {
          num_active_pulse = 20;
          num_addition = 0;
        }
      } else {
        u16NumActive = 0;
        num_active_pulse = 0;
        num_addition = 0;
      }
}



