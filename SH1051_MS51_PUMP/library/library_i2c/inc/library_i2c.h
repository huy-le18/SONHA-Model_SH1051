#ifndef LIBRARY_I2C_H_
#define LIBRARY_I2C_H_



void i2c_master_write(void);
void i2c_master_read(void);

unsigned char CRC8(volatile unsigned char *Data, unsigned char length);

#endif /**/