#ifndef TM1628_h
#define TM1628_h

#include "BS84B08A-3.h"

//flags for turn on/off
#define ON 1
#define OFF 0

#define BLINKING 1
#define NO_BLINK 0

//flags for indicating LEDs
#define LED_D5 0x00
#define LED_D18 0x01
#define LED_D17 0x02
#define LED_D15 0x03
#define LED_D2 0x04
#define LED_D10 0x05
#define LED_D11 0x06
#define LED_D8 0x07
#define LED_D21 0x08
#define LED_D25 0x09
#define LED_D23 0x0A
#define LED_D3 0x0B
#define LED_D20 0x0C
#define LED_D9 0x0D
#define LED_D24 0x0E
#define LED_D16 0x0F
#define LED_D13 0x10
#define LED_D14 0x11
#define LED_D12 0x12
#define LED_D1 0x13
#define LED_D19 0x14
#define LED_D6 0x15
#define LED_D22 0x16
#define LED_D4 0x17


#define dio         _pb0
#define dio_c       _pbc0
#define clk         _pb1
#define clk_c       _pbc1
#define stb         _pb2
#define stb_c       _pbc2

#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

// init
void TM1628_init();

void begin(unsigned char active, unsigned char intensity);
void update();
void clear();
void setCursor(unsigned char pos);
void write(unsigned char chr);

void sendData(unsigned char addr, unsigned char data);
void sendCommand(unsigned char data);
void send(unsigned char data);
unsigned char min(unsigned char a, unsigned char b);
unsigned char max(unsigned char a, unsigned char b);

void setSeg12 (unsigned char num1,unsigned char num2 );
void setLedELCB(void);
void setLedPump(void);

void clearSeg12(void);
void clearLedELCB(void);
void clearLedPump(void);

#endif
