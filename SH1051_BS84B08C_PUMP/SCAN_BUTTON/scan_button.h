#ifndef SCAN_BUTTON_H_
#define SCAN_BUTTON_H_

#define MAX_BUTTON 3

#define BT_TEST_BIT			3
#define BT_RESET_BIT		4
#define BT_PUMP_BIT			5


typedef enum
{
	NO_FLAG_BT = 0,
	FLAG_BT_TEST = 1,
	FLAG_BT_RESET,
	FLAG_BT_PUMP
}flagButton_t;

typedef enum
{
	NO_PRESS_BT = 0,
	PRESS_BT_TEST = 1,
	PRESS_BT_RESET,
	PRESS_BT_PUMP
}pressBt_t;

flagButton_t button_detect_press(void);
pressBt_t button_detect_release();

#define DEBOUNCE_TIME 2      // 20ms

#define NULL 0

#endif