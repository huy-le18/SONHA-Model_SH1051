#include "driver_debug.h"
#include "library_debug.h"
#include "ms51_16k.h"

void uart_sw_init(void)
{
	P16_PUSHPULL_MODE;
}
void uart_sw_tx_high(void)
{
	P16 = 1;
}
void uart_sw_tx_low(void)
{
	P16 = 0;
}

void uart_sw_send_byte(unsigned char TxData)
{
	unsigned char i;

	uart_sw_tx_low();   // start 
	timer2_delay_us(BAUDRATE);
	for (i = 0; i < 8; i++)			 //send 8-bit data
	{

		if (TxData & 0x01)
		{
			uart_sw_tx_high();
		}
		else
		{
			uart_sw_tx_low();
		}
		timer2_delay_us(BAUDRATE);
		TxData >>= 1;
	}
	uart_sw_tx_high();
	timer2_delay_us(BAUDRATE);
}


void uart_sw_send_string(unsigned char *TxData, unsigned long length)
{
	uint32_t temp = 0;

	for(temp = 0; temp < length; temp ++)
	{
		uart_sw_send_byte(TxData[temp]);
		timer2_delay_us(DELAY_FRAME);
	}
}


void uart_sw_send_number(unsigned long number)
{
	uint8_t temp = 0;

	temp = number / 1000;
	uart_sw_send_byte(temp + 48);
	timer2_delay_us(DELAY_FRAME);

	temp = number % 1000 / 100;
	uart_sw_send_byte(temp + 48);
	timer2_delay_us(DELAY_FRAME);

	temp = number % 100 / 10;
	uart_sw_send_byte(temp + 48);
	timer2_delay_us(DELAY_FRAME);

	temp = number % 10;
	uart_sw_send_byte(temp + 48);
	timer2_delay_us(DELAY_FRAME);
}

