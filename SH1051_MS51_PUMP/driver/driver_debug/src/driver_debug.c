#include "driver_debug.h"
#include "ms51_16k.h"

void timer2_delay_us(unsigned int time_delay)
{
	T2MOD |= (1 << T2DIV1);   // f = fsys/16 = 1MHz
	
	TL2 = (65535 - time_delay ) & 0xff;
	TH2 = (65535 - time_delay ) >> 8;
	T2CON |= (1 << TR2);
	while((1 & (T2CON >> TF2)) == 0);
	T2CON &= ~(1 << TF2);
	T2CON &= ~(1 << TR2);
}

void timer2_delay_ms(unsigned int time_delay)
{
	T2MOD |= (1 << T2DIV1);   // f = fsys/16 = 1MHz
	while(time_delay)
	{
		TL2 = (65535 - 1000 ) & 0xff;
		TH2 = (65535 - 1000 ) >> 8;
		T2CON |= (1 << TR2);
		while((1 & (T2CON >> TF2)) == 0);
		T2CON &= ~(1 << TF2);
		T2CON &= ~(1 << TR2);
		time_delay --;
	}
}

void timer2_delay_193us(void)
{
	T2MOD |= (1 << T2DIV1);   // f = fsys/16 = 1MHz
	
	TL2 = (65535 - 200 ) & 0xff;
	TH2 = (65535 - 200 ) >> 8;
	T2CON |= (1 << TR2);
	while((1 & (T2CON >> TF2)) == 0);
	T2CON &= ~(1 << TF2);
	T2CON &= ~(1 << TR2);
}
