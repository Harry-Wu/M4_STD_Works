#include "stm32f4xx.h"
#include "delay.h"

void uart_init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	
	//GPIO�ʹ���ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//����uart1��Ӧ�˿�PA9(TX), PA10(RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1);
	
	//UARTģ���ʼ��
	USART_StructInit(&USART_InitStruct);  //��ʼ���ṹ�壬��������ϵͳĬ��ֵ
	USART_InitStruct.USART_BaudRate = baud;  //Ȼ��Ĭ��ֵ��Ҫ���ĵ������¸�ֵ
	USART_Init(USART1, &USART_InitStruct);
	
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	
}
