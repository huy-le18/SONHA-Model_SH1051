#include "driver_app.h"
#include "library_app.h"
#include "library_debug.h"
#include "ms51_16k.h"
/***************for task**************/

const unsigned int tempArray[56] = {
  851,  // 10 degree
  844,
  837,
  830,
  822,
  814,
  807,
  799,
  790,
  782,
  774,
  765,
  756,
  747,
  738,
  731,
  723,
  714,
  704,
  695,
  685,
  675,
  666,
  656,
  646,
  636,  // 35
  626,
  616,
  606,
  595,
  585,  // 40
  575,
  565,
  555,
  545,
  535,
  525,
  515,
  505,
  495,
  485,
  475,
  466,
  456,
  447,
  437,  // 55 degree
  428,
  419,
  410,
  401,
  392,  // 60 degree
  384,
  375,
  367,
  358,
  350,  // 65 degree
};


void task_init(void)
{
	timer3_init_for_task(200);
}

/***************for flow**************/
void flow_counter_init(void)
{
	timer1_counter_init();
}
unsigned int flow_counter_read(void)
{
	return timer1_counter_read();
}
void flow_counter_reset(void)
{
	timer1_counter_reset();
}

void detect_zero_init(void)
{
	gpio_interrupt_init();
}
/***************for adc**************/
void ntc_in_adc_init(void)
{
	adc_init();
}
unsigned int ntc_in_adc_read(void)
{
	return adc_read();
}

void ntc_in_select_channel(void)
{
	adc_select_channel(ADC_CHANNEL_NTC_IN);
	adc_start();
}

ADC_CHANNEL_t adc_get_ntc_in_or_ntc_out(void)
{
	return adc_get_channel();
}


void ntc_out_adc_init(void)
{
	adc_init();
}
unsigned int ntc_out_adc_read(void)
{
	return adc_read();
}
void ntc_out_select_channel(void)
{
	adc_select_channel(ADC_CHANNEL_NTC_OUT);
	adc_start();
}

/***************for gpio**************/

//#define INT_OS_PIN						P04
//#define RESET_LO_PIN					P11
//#define LO_PIN								P12

//#define CTR_OUTAC_PIN					P03
//#define CTR_TRIAC_PIN					P17
//#define CTR_PUMP_PIN					P05

//#define SCL_PIN								P13
//#define SDA_PIN								P14

void lo_output_init(void)
{
	P12_PUSHPULL_MODE;
}
void lo_write_high(void)
{
	LO_PIN = 1; 
}
void lo_write_low(void)
{
	LO_PIN = 0;
}
void lo_write_tonggle(void)
{
	LO_PIN = !LO_PIN;
}


void int_os_input_init(void)
{
	P04_INPUT_MODE;
}

bit int_os_read_state(void)
{
	return INT_OS_PIN;
}

void reset_lo_output_init(void)
{
	P11_PUSHPULL_MODE;
}
void reset_lo_write_high(void)
{
	RESET_LO_PIN = 1;
}
void reset_lo_write_low(void)
{
	RESET_LO_PIN = 0;
}

void ctr_pump_output_init(void)
{
	P05_PUSHPULL_MODE;
}
void ctr_pump_write_high(void)
{
	CTR_PUMP_PIN = 1;
}
void ctr_pump_write_low(void)
{
	CTR_PUMP_PIN = 0;
}


void ctr_triac_output_init(void)
{
	P17_PUSHPULL_MODE;
}
void ctr_triac_write_high(void)
{
	CTR_TRIAC_PIN = 1;
}
void ctr_triac_write_low(void)
{
	CTR_TRIAC_PIN = 0;
}

void ctr_outac_output_init(void)
{
	P03_PUSHPULL_MODE;
}
void ctr_outac_write_high(void)
{
	CTR_OUTAC_PIN = 1;
}
void ctr_outac_write_low(void)
{
	CTR_OUTAC_PIN = 0;
}

uint16_t convert12BitsTo10Bit(uint16_t value) {
  uint32_t temp = 0;

  temp = ((uint32_t)value * 1023) / 4095;

  return (uint16_t)temp;
}

uint8_t getTempFromTable(uint16_t ADTEMP, volatile uint8_t *temp) {
  uint8_t i;

  if ((ADTEMP > tempArray[0]) || (ADTEMP < tempArray[55])) {
    *temp = 0;
    return 0;
  }

  for (i = 1; i <= 55; i++) {
    if (tempArray[i] <= ADTEMP) {
      break;
    }
  }

  *temp = i + 10 /* - 1 */;
  return 1;
}

