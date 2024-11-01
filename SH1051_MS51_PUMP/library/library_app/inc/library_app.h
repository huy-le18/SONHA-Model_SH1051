#ifndef LIBRARY_APP_H_
#define LIBRARY_APP_H_


#define ADC_CHANNEL_NTC_OUT					ADC_CHANNEL_3
#define ADC_CHANNEL_NTC_IN					ADC_CHANNEL_2

#define INT_OS_PIN						P04
#define RESET_LO_PIN					P11
#define LO_PIN								P12

#define CTR_OUTAC_PIN					P03
#define CTR_TRIAC_PIN					P17
#define CTR_PUMP_PIN					P05



// for counter

void task_init(void);
void flow_counter_init(void);
unsigned int flow_counter_read(void);
void flow_counter_reset(void);

void detect_zero_init(void);
// for adc
void ntc_in_adc_init(void);
unsigned int ntc_in_adc_read(void);
void ntc_in_select_channel(void);

void ntc_out_adc_init(void);
unsigned int ntc_out_adc_read(void);
void ntc_out_select_channel(void);
ADC_CHANNEL_t adc_get_ntc_in_or_ntc_out(void);

// for gpio
void lo_output_init(void);
void lo_write_high(void);
void lo_write_low(void);
void lo_write_tonggle(void);

void int_os_input_init(void);
bit int_os_read_state(void);

void reset_lo_output_init(void);
void reset_lo_write_high(void);
void reset_lo_write_low(void);

void ctr_pump_output_init(void);
void ctr_pump_write_high(void);
void ctr_pump_write_low(void);

void ctr_triac_output_init(void);
void ctr_triac_write_high(void);
void ctr_triac_write_low(void);

void ctr_outac_output_init(void);
void ctr_outac_write_high(void);
void ctr_outac_write_low(void);

unsigned int convert12BitsTo10Bit(unsigned int value); 
unsigned char getTempFromTable(unsigned int ADTEMP, volatile unsigned char *temp);


/****************API for APPLICATION level 2******************/

#endif /*LIBRARY_APP_H_*/




