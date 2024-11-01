#ifndef DRIVER_APP_H_
#define DRIVER_APP_H_
/*****************************************************************************************
* For GPIO INIT setting 
*****************************************************************************************/


typedef enum
{
	ADC_CHANNEL_0 = 0,
	ADC_CHANNEL_1,
	ADC_CHANNEL_2,
	ADC_CHANNEL_3,
	ADC_CHANNEL_4,
	ADC_CHANNEL_5,
	ADC_CHANNEL_6,
	ADC_CHANNEL_7,
	ADC_CHANNEL_8,
	ADC_CHANNEL_9,
	ADC_CHANNEL_10
} ADC_CHANNEL_t;


void timer1_counter_init(void);
unsigned int timer1_counter_read(void);
void timer1_counter_reset(void);
void gpio_interrupt_init(void);

void adc_init(void);
void adc_start(void);
void adc_select_channel(ADC_CHANNEL_t adc_channel);
ADC_CHANNEL_t adc_get_channel(void);
unsigned int adc_read(void);

void timer3_init_for_task(unsigned int time_task_init);
void i2c_init_interrupt(void);
#endif  /* DRIVER_APP_H_ */