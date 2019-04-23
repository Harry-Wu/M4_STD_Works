#include "stm32f4xx.h"
#include "stdio.h"

void uart_init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStructuct;
	
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
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	//NVIC_SetPriority(USART1_IRQn, );
	NVIC_InitStructuct.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructuct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructuct.NVIC_IRQChannelPreemptionPriority = 3;  //抢占优先级
	NVIC_InitStructuct.NVIC_IRQChannelSubPriority = 3;  //响应优先级
	NVIC_Init(&NVIC_InitStructuct);
	
	//使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//使能串口
	USART_Cmd(USART1, ENABLE);	
}


u8 rec_buf[100];  //接收缓冲区, 如果不声明申请的空间,就必须赋值
u8 rec_sta=0; //接收完成标志, rec_sta=1表示接收完成
u8 rec_num=0;  //接收字节数
void USART1_IRQHandler(void)
{
	u8 rec_data;
	//判断是否是UART1中断中的接收中断
	if((rec_sta == 0) && (USART_GetITStatus(USART1, USART_IT_RXNE) == SET))  //还没有收完，并且有接收到数据 
	{
		rec_data = USART_ReceiveData(USART1);  //rec_data =USART1->DR;
		if((rec_data=='\n') &&(	rec_buf[rec_num-1]=='\r') )  //接收完毕处理
		{
			rec_buf[rec_num-1]='\0'; //用\0替换\r,把收到的数据转换为一个字符串
			rec_sta=1;   //接收完成标志置1
			rec_num = 0;	
		}
		else rec_buf[rec_num++] = rec_data;  //接收完一个字节后把数据存到数组里面			
	}
}


//发送字符串
void uart_send_string(char *p)
{
	while(*p != '\0')
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //数据还没被转移到移位寄存器
		USART1->DR = *p;  //USART_SendData(USART1, *p);
		p++;
	}
}

//重写fputc函数，以支持printf函数
int fputc(int ch, FILE *f) 
{
	while((USART1->SR &(0X01<<7))==0);
	USART1->DR=ch;
	return (ch);
}

