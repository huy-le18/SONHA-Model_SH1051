#ifndef MAIN_H_
#define MAIN_H_


#include "ms51_16k.h"
#include "driver_app.h"
#include "driver_debug.h"
#include "library_app.h"
#include "library_i2c.h"
#include "library_debug.h"


// define command send from slave
#define CMD_TEST_BIT	0
#define CMD_RESET_BIT	1
#define PUMP_ON_CMD 	2   // pump on
#define PUMP_OFF_CMD  3  // pump off

//#define CMD_TEST_BIT	0
//#define CMD_RESET_BIT	1
//#define PUMP_ON_CMD		2
//#define PUMP_OFF_CMD	3


#define FLOW_MIN_TO_PROTECT			20
#define TEMP_MIN_TO_PROTECT			60

#define ADC_LOW_LIMIT 10
#define ADC_HIGH_LIMIT 1000




typedef enum {
  DEVICE_MODE_START = 0,
  DEVICE_MODE_TEST,   // test ro dien
	DEVICE_MODE_POWER,    // khong test
} DeviceMode_t;


typedef enum {
	DEVICE_MODE_POWER_OFF,
	DEVICE_MODE_POWER_ON    // error accur
} DeviceModePower_t;

typedef enum {
  DEVICE_MODE_POWER_ON_NORMAL,
	DEVICE_MODE_POWER_ON_ERR,
} DeviceModePowerOn_t;

void mode_start_program(void);
void mode_test_program(void);
void mode_power_program(void);
void mode_power_off_program(void);
void mode_power_on_program(void);
void mode_power_on_err_program(void);
void mode_power_on_normal_program(void);

void handle_pump(void);
void handle_cmd_pump(void);
extern volatile unsigned char	u8_flag_200us, u8_flag_1ms, u8_flag_2ms, u8_flag_10ms,u8_flag_50ms, u8_flag_100ms,u8_flag_200ms, u8_flag_500ms, u8_flag_1s, u8_flag_2s; 
extern volatile unsigned int u16_adc_value;
extern volatile unsigned char u8_flag_adc;
extern volatile uint8_t u8_detect_zero;

extern volatile uint8_t triac_pulse_cnt;
extern volatile uint8_t num_active_pulse;           // power from 0 to 20
extern volatile uint8_t num_active_pulse_with_add;  // from 0 to 20
extern volatile uint8_t pulse_trigger_off_cnt;
extern volatile bit b_triggered;
extern volatile uint8_t num_addition;
extern volatile uint8_t additionInterval;
extern volatile uint16_t u16NumActive;  // raw number of active pulse * 10

#endif /*main.h*/