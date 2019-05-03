#include "rs485.h"	 
#include "delay.h"

#if EN_USART2_RX   		//如果使能了接收   	  	
u8 RS485_RX_BUF[64];  	//接收缓冲区,最大64个字节.
//接收到的数据长度
//u8 RS485_RX_CNT=0;  
u8 rec_sta2=0; //接收完成标志, rec_sta2=1表示接收完成
u8 rec_num2=0;  //接收字节数
void USART2_IRQHandler(void)
{
	u8 rec_data;
	//判断是否是UART1中断中的接收中断
	if((rec_sta2 == 0) && (USART_GetITStatus(USART2, USART_IT_RXNE) == SET))  //还没有收完，并且有接收到数据 
	{
		rec_data = USART_ReceiveData(USART2);  //rec_data =USART2->DR;
		if((rec_data=='\n') &&(	RS485_RX_BUF[rec_num2-1]=='\r') )  //接收完毕处理
		{
			RS485_RX_BUF[rec_num2-1]='\0'; //用\0替换\r,把收到的数据转换为一个字符串
			rec_sta2=1;   //接收完成标志置1
			rec_num2 = 0;	
		}
		else RS485_RX_BUF[rec_num2++] = rec_data;  //接收完一个字节后把数据存到数组里面			
	}
}
#endif

//初始化UART2
void RS485_Init(u32 baud)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//GPIO和串口时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//设置uart2对应端口PA2(TX), PA3(RX)
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2);
	
	//PG8初始化，485控制模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOG, &GPIO_InitStruct);
	
	//UART2模块初始化
	USART_StructInit(&USART_InitStruct);  //初始化结构体，给它赋予系统默认值
	USART_InitStruct.USART_BaudRate = baud;  //然后将默认值需要更改的项重新赋值
	USART_Init(USART2, &USART_InitStruct);
		
#if EN_USART2_RX
	//使能串口接收中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//NVIC配置
	//NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 3;  //抢占优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 3;  //响应优先级
	NVIC_Init(&NVIC_InitStruct);
#endif

	//使能串口
	USART_Cmd(USART2, ENABLE);
	RS485_TX_EN = DISABLE;  //默认为接收模式
}

//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void RS485_Send_Data(u8 *buf,u8 len)
{
	//u8 i;
	RS485_TX_EN = ENABLE;
	for(u8 i=0; i<len; i++)
	{
		while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET); //等待发送结束,死等
		USART_SendData(USART2,buf[i]); //发送数据	
	}
	while(USART_GetFlagStatus(USART2,USART_FLAG_TC)==RESET); //等待发送结束		
	rec_num2 = 0;	  
	RS485_TX_EN = DISABLE;  //设置为接收模式	
}


//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void RS485_Receive_Data(u8 *buf,u8 *len)
{
	u8 rxlen = rec_num2;
	u8 i = 0;
	*len = 0;				//默认为0
	delay_ms(10);		//等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束
	if(rxlen == rec_num2 && rxlen)//接收到了数据,且接收结束
	{
		for(i=0;i<rxlen;i++)
		{
			buf[i]=RS485_RX_BUF[i];	
		}		
		*len=rec_num2;	//记录本次数据长度
		rec_num2=0;		//清零
	}
}
