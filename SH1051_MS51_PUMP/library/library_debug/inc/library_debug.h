#ifndef LIBRARY_DEBUG_H_
#define LIBRARY_DEBUG_H_

#define TX_PIN		P16

#define DELAY_FRAME 200
#define BAUDRATE 81	

void uart_sw_tx_high(void);
void uart_sw_tx_low(void);
void uart_sw_init(void);
void uart_sw_send_byte(unsigned char TxData);
void uart_sw_send_string(unsigned char *TxData, unsigned long length);
void uart_sw_send_number(unsigned long number);

#endif /*LIBRARY_DEBUG_H_*/