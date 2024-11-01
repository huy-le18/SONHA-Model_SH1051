#include "TM1628.h"
#include "TM16XXFonts.h"

unsigned char buffer[14];


void TM1628_init() {
  // init variable
  unsigned char i;
  
  for (i = 0; i < 14; i++) {
    buffer[i] = 0;
  }


  // setup pins are ouput
  dio_c = 0;
  clk_c = 0;
  stb_c = 0;

  // setup
  stb = 1;
  clk = 1;

  sendCommand(0x8D);  //activate TM1638 & set brightness of display
  sendCommand(0xC0);  
  sendCommand(0x44);  //set set single address mode

  stb = 0;
  send(0xC0);
  clear();
  update();
  stb = 1;
}

void begin(unsigned char active, unsigned char intensity) {
  sendCommand(0x80 | (active ? 8 : 0) | min(7, intensity));
}

void update() {
	
  sendData(8, buffer[8]);
  sendData(9, buffer[9]);
  sendData(10, buffer[10]);
  sendData(12, buffer[12]);
  sendData(13, buffer[13]);
  
}

void clear() {
  buffer[8] = 0x00;
  buffer[9] = 0x00;
  buffer[10] = 0x00;
  buffer[12] = 0x00;
  buffer[13] = 0x00;
}





unsigned char min(unsigned char a, unsigned char b) {
  return (a < b) ? a : b;
}

unsigned char max(unsigned char a, unsigned char b) {
  return (a > b) ? a : b;
}

/*********** mid level  **********/
void sendData(unsigned char addr, unsigned char data) {
//  _emi  = 0;
  // sendCommand(0x44);
  stb = 0;
  _nop();
  send(0xC0 | addr);
  send(data);
  _nop();
  stb = 1;
//  _emi = 1;
}

void sendCommand(unsigned char data) {
//  _emi  = 0;
  stb = 0;
  send(data);
  stb = 1;
//  _emi = 1;
}

void setSeg12 (unsigned char num1, unsigned char num2) {
    bitWrite(buffer[8], 0x01, bitRead(NUMBER_FONT[num1], 0));  // SEG_A - SEG2 - BIT1
    bitWrite(buffer[8], 0x00, bitRead(NUMBER_FONT[num1], 1));  // SEG_B - SEG1 - BIT0
    bitWrite(buffer[9], 0x01, bitRead(NUMBER_FONT[num1], 2));  // SEG_C - SEG10 - BIT1
    bitWrite(buffer[8], 0x07, bitRead(NUMBER_FONT[num1], 3));  // SEG_D - SEG8 - BIT7
    bitWrite(buffer[9], 0x00, bitRead(NUMBER_FONT[num1], 4));  // SEG_E - SEG9 - BIT0
    bitWrite(buffer[8], 0x02, bitRead(NUMBER_FONT[num1], 5));  // SEG_F - SEG3 - BIT2
    bitWrite(buffer[8], 0x03, bitRead(NUMBER_FONT[num1], 6));  // SEG_G - SEG4 - BIT3

    bitWrite(buffer[12], 0x01, bitRead(NUMBER_FONT[num2], 0));  // SEG_A - SEG2 - BIT1
    bitWrite(buffer[12], 0x00, bitRead(NUMBER_FONT[num2], 1));  // SEG_B - SEG1 - BIT0
    bitWrite(buffer[13], 0x01, bitRead(NUMBER_FONT[num2], 2));  // SEG_C - SEG10 - BIT1
    bitWrite(buffer[12], 0x07, bitRead(NUMBER_FONT[num2], 3));  // SEG_D - SEG8 - BIT7
    bitWrite(buffer[13], 0x00, bitRead(NUMBER_FONT[num2], 4));  // SEG_E - SEG9 - BIT0
    bitWrite(buffer[12], 0x02, bitRead(NUMBER_FONT[num2], 5));  // SEG_F - SEG3 - BIT2
    bitWrite(buffer[12], 0x03, bitRead(NUMBER_FONT[num2], 6));  // SEG_G - SEG4 - BIT3
}

// COM3
void setLedELCB(void)
{
  bitWrite(buffer[10], 0x00, 1);  // SEGB - SEG1 - BIT0
}


void setLedPump(void)
{
   bitWrite(buffer[10], 0x01, 1); // SEG_A - SEG2 - BIT1
}

void clearSeg12() {
  buffer[8] = 0x00;
  buffer[9] = 0x00;
  buffer[12] = 0x00;
  buffer[13] = 0x00;
}


void clearLedELCB() {
  bitWrite(buffer[10], 0x00, 0); 
}

void clearLedPump() {
  bitWrite(buffer[10], 0x01, 0); 
}

/************ low level **********/
void send(unsigned char data) {
  unsigned char i;
  
  for (i = 0; i < 8; i++) {
    clk = 0;
    _nop();
    dio = data & 0x01;
    _nop();
    data >>= 1;
    _nop();
    clk = 1;
    _nop();
  }
  
}

