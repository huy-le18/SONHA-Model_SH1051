#include "library_i2c.h"
#include "driver_app.h"
#include "library_debug.h"
#include "ms51_16k.h"

#define WRITE						0
#define READ						1
#define ADDRESS_SLAVE		0xA0

extern volatile uint8_t addSlaveAndRW;

void i2c_master_write(void)
{
	addSlaveAndRW = ADDRESS_SLAVE|WRITE;
}
void i2c_master_read(void)
{
	addSlaveAndRW = ADDRESS_SLAVE|READ;
}

unsigned char CRC8(volatile unsigned char *Data, unsigned char length) {
  unsigned char crc = 0x00;
  unsigned char extract;
  unsigned char sum;
  unsigned char i;
  unsigned char tempI;

  for (i = 0; i < length; i++) {
    extract = *Data;
    for (tempI = 8; tempI; tempI--) {
      sum = (crc ^ extract) & 0x01;
      crc >>= 1;
      if (sum)
        crc ^= 0x8C;
      extract >>= 1;
    }
    Data++;
  }
  return crc;
}
