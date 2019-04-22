/*****************************************************************************
@Description: uart1将接收到的数据发回给上位机，采用死等的方式 
@History: 
		
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "io_bit.h"


int main(void)
{
	u8 key, rec_data;  //保存按键按下状态
	//硬件初始化
	led_init();
	key_init();
	uart_init(115200);
	while(1)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);  //等待接收数据，RESET表明没收到数据
		rec_data = USART_ReceiveData(USART1);  //读取的1字节数据存起来
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //等待上次发送完毕
		USART_SendData(USART1, rec_data);  //发送数据
		
//		key = key_scanf();
//		if(key == KEY1_OK)  //如果KEY1按键按下
//		{
//			if(LED5 == LED_OFF) LED5 = LED_ON;
//			else LED5 = LED_OFF;
//		}
		
	}
}
