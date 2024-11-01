#include "scan_button.h"
#include "..\USER_PROGRAM\USER_PROGRAM.INC"


flagButton_t button_detect_press(void)
{
  flagButton_t flagButton = NO_FLAG_BT;
  unsigned char dataButon;
  
  GET_KEY_BITMAP();
  dataButon = DATA_BUF[0];  
  
  if(dataButon & (1 << BT_TEST_BIT))
  {
  	flagButton = FLAG_BT_TEST;
  }
  else if(dataButon & (1 << BT_RESET_BIT))
  {
  	flagButton = FLAG_BT_RESET;
  }
  else if(dataButon & (1 << BT_PUMP_BIT))
  {
  	flagButton = FLAG_BT_PUMP;
  }
  
  return flagButton;
}

pressBt_t button_detect_release()
{
	pressBt_t pressBt  = NO_PRESS_BT;
	unsigned char dataButon;
  
 	GET_KEY_BITMAP();
    dataButon = DATA_BUF[0];  
	if(dataButon) pressBt  = PRESS_BT_TEST;
	
	return pressBt;
}