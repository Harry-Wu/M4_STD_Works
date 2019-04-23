#ifndef _UART_H_
#define _UART_H_
#include "stm32f4xx.h"

extern u8 rec_buf[100];   //接收缓冲区
extern u8 rec_sta;      //接收完成标志rec_sta=1表明接收完成

void uart_init(u32 bond);
void uart_send_string(u8 *p);

#endif
