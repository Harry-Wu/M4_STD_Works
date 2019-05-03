#include "rs485.h"	 
#include "delay.h"

#if EN_USART2_RX   		//���ʹ���˽���   	  	
u8 RS485_RX_BUF[64];  	//���ջ�����,���64���ֽ�.
//���յ������ݳ���
//u8 RS485_RX_CNT=0;  
u8 rec_sta2=0; //������ɱ�־, rec_sta2=1��ʾ�������
u8 rec_num2=0;  //�����ֽ���
void USART2_IRQHandler(void)
{
	u8 rec_data;
	//�ж��Ƿ���UART1�ж��еĽ����ж�
	if((rec_sta2 == 0) && (USART_GetITStatus(USART2, USART_IT_RXNE) == SET))  //��û�����꣬�����н��յ����� 
	{
		rec_data = USART_ReceiveData(USART2);  //rec_data =USART2->DR;
		if((rec_data=='\n') &&(	RS485_RX_BUF[rec_num2-1]=='\r') )  //������ϴ���
		{
			RS485_RX_BUF[rec_num2-1]='\0'; //��\0�滻\r,���յ�������ת��Ϊһ���ַ���
			rec_sta2=1;   //������ɱ�־��1
			rec_num2 = 0;	
		}
		else RS485_RX_BUF[rec_num2++] = rec_data;  //������һ���ֽں�����ݴ浽��������			
	}
}
#endif

//��ʼ��UART2
void RS485_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//GPIO�ʹ���ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//����uart2��Ӧ�˿�PA2(TX), PA3(RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	//PG8��ʼ����485����ģʽ
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	//UART2ģ���ʼ��
	USART_StructInit(&USART_InitStruct);  //��ʼ���ṹ�壬��������ϵͳĬ��ֵ
	USART_InitStruct.USART_BaudRate = baud;  //Ȼ��Ĭ��ֵ��Ҫ���ĵ������¸�ֵ
	USART_Init(USART2, &USART_InitStruct);
		
#if EN_USART2_RX
	//ʹ�ܴ��ڽ����ж�
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//NVIC����
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;  //��Ӧ���ȼ�
	NVIC_Init(&NVIC_InitStruct);
#endif

	//ʹ�ܴ���
	USART_Cmd(USART2, ENABLE);
	RS485_TX_EN = DISABLE;  //Ĭ��Ϊ����ģʽ
}

//RS485����len���ֽ�.
//buf:�������׵�ַ
//len:���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ����64���ֽ�)
void RS485_Send_Data(u8 *buf,u8 len)
{
	//u8 i;
	RS485_TX_EN = ENABLE;
	for(u8 i=0; i<len; i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET); //�ȴ����ͽ���,����
		USART_SendData(USART2,buf[i]); //��������	
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //�ȴ����ͽ���		
	rec_num2 = 0;	  
	RS485_TX_EN = DISABLE;  //����Ϊ����ģʽ	
}


//RS485��ѯ���յ�������
//buf:���ջ����׵�ַ
//len:���������ݳ���
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen = rec_num2;
	u8 i = 0;
	*len = 0;				//Ĭ��Ϊ0
	delay_ms(10);		//�ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս���
	if(rxlen == rec_num2 && rxlen)//���յ�������,�ҽ��ս���
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=rec_num2;	//��¼�������ݳ���
		rec_num2=0;		//����
	}
}
