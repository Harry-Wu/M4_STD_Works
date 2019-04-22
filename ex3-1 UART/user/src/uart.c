#include "stm32f4xx.h"
#include "delay.h"

void uart_init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//GPIO和串口时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//设置uart1对应端口PA9(TX), PA10(RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//UART模块初始化
	USART_StructInit(&USART_InitStruct);  //初始化结构体，给它赋予系统默认值
	USART_InitStruct.USART_BaudRate = baud;  //然后将默认值需要更改的项重新赋值
	USART_Init(USART1, &USART_InitStruct);
	
	//使能串口
	USART_Cmd(USART1, ENABLE);	
}
