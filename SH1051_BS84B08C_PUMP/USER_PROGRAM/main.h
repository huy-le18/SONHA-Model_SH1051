#ifndef MAIN_H_
#define MAIN_H_

#define CMD_TEST_BIT	0
#define CMD_RESET_BIT	1
#define PUMP_ON_CMD		2
#define PUMP_OFF_CMD	3

typedef enum {
  DEVICE_MODE_START = 0,
  DEVICE_MODE_TEST,   // test ro dien
  DEVICE_MODE_POWER,    // khong test
} DeviceMode_t;


typedef enum {
  DEVICE_MODE_POWER_OFF,
  DEVICE_MODE_POWER_ON    
} DeviceModePower_t;

typedef enum {
  DEVICE_MODE_POWER_ON_NORMAL,
  DEVICE_MODE_POWER_ON_ERR,
} DeviceModePowerOn_t;


extern volatile bit 	b_flag_200us, b_flag_1ms, b_flag_2ms, b_flag_10ms;
extern volatile bit     b_flag_100ms, b_flag_500ms, b_flag_1s, b_flag_2s; 
extern volatile bit		b_flag2msInStart, b_flag10msInStart, b_flag100msInStart, b_flag500msInStart, b_flag1sInStart, b_flag2sInStart;
extern volatile bit		b_flag2msInTest, b_flag10msInTest, b_flag100msInTest, b_flag500msInTest;
extern volatile bit 	b_flag2msInPower, b_flag10msInPower, b_flag100msInPower, b_flag500msInPower;
extern volatile bit 	b_flag2msInPowerOn , b_flag10msInPowerOn , b_flag100msInPowerOn , b_flag500msInPowerOn ;
extern volatile bit     b_flag500msInPowerOff;

void mode_start_program(void);
void mode_test_program(void);
void mode_power_program(void);
void mode_power_off_program(void);
void mode_power_on_program(void);




#endif 