#include "stm32f4xx.h"
#include "stdio.h"

void uart_init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructuct;
	
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC_SetPriority(USART1_IRQn, );
	NVIC_InitStructuct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructuct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructuct.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�
	NVIC_InitStructuct.NVIC_IRQChannelSubPriority = 3;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStructuct);
	
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//ʹ�ܴ���
	USART_Cmd(USART1, ENABLE);	
}


u8 rec_buf[100];  //���ջ�����, �������������Ŀռ�,�ͱ��븳ֵ
u8 rec_sta=0; //������ɱ�־, rec_sta=1��ʾ�������
u8 rec_num=0;  //�����ֽ���
void USART1_IRQHandler(void)
{
	u8 rec_data;
	//�ж��Ƿ���UART1�ж��еĽ����ж�
	if((rec_sta == 0) && (USART_GetITStatus(USART1, USART_IT_RXNE) == SET))  //��û�����꣬�����н��յ����� 
	{
		rec_data = USART_ReceiveData(USART1);  //rec_data =USART1->DR;
		if((rec_data=='\n') &&(	rec_buf[rec_num-1]=='\r') )  //������ϴ���
		{
			rec_buf[rec_num-1]='\0'; //��\0�滻\r,���յ�������ת��Ϊһ���ַ���
			rec_sta=1;   //������ɱ�־��1
			rec_num = 0;	
		}
		else rec_buf[rec_num++] = rec_data;  //������һ���ֽں�����ݴ浽��������			
	}
}


//�����ַ���
void uart_send_string(char *p)
{
	while(*p != '\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //���ݻ�û��ת�Ƶ���λ�Ĵ���
		USART1->DR = *p;  //USART_SendData(USART1, *p);
		p++;
	}
}

//��дfputc��������֧��printf����
int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
	USART1->DR=ch;
	return (ch);
}

