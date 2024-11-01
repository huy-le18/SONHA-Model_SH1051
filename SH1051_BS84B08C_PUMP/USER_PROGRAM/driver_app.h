#ifndef DRIVER_APP_H_
#define DRIVER_APP_H_

extern volatile unsigned int u16_timer_for_buzzer;
extern volatile bit 	b_forever_buzzer;

#define BUZZER_SET()		 _pa3 = 1
#define BUZZER_CLR()		 _pa3 = 0
#define BUZZER_OUTPUT()		 _pac3 = 0
#define BUZZER_TONGGLE() 	 _pa3 = !_pa3;
	
#define LED_UI_ON() _pa4 = 1
#define LED_UI_OFF() _pa4 = 0
#define LED_UI_TOGGLE() _pa4 = !_pa4


void TIMER_Init();
void I2C_init(void);
void IO_Init() ;
void set_buzzer_on(unsigned int timer_for_buzzer);
void set_buzzer_on_forever();
void set_buzzer_off_forever();

#endif