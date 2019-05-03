/*****************************************************************************
@Description: uart1采用中断方式处理接收到的字符，以是否用换行回车符判断字符串
是否发完，然后根据接收到的字符串执行相应的指令
@History: 
		V1.0: 上电时发一串默认字符串到RS485，运行时RS485将接收到的字符串发回

		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "io_bit.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "rs485.h"


int main(void)
{
	u8 rs485buf[6] = "apple";  //用于发送的数组
	u8 rs485buf2[14] = "收到数据：";
	u8 rs485buf3[1] = {'\n'};
	
	//硬件初始化
	led_init();
	key_init();
	uart_init(115200);
	RS485_Init(9600);  //初始化RS485，UART2
	LED5 = LED_ON;
	LED6 = LED_ON;
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	LED5 = LED_OFF;
	LED6 = LED_OFF;
	RS485_Send_Data(rs485buf,sizeof(rs485buf));//发送数组
	printf("请发送LED控制指令，以回车结束：\n\r");
	printf("\"open\"为开启LED5，\"close\"为关闭LED5， \"switch\"为切换LED6开关。\n\r");
	while(1)
	{
		if(rec_sta==1) //如果字符串接收完成
		{
			if(0==strcmp((const char *)rec_buf, "open"))
			{
				LED5 = LED_ON;
				printf("LED5已打开。");	
			}
			else if(0==strcmp((const char *)rec_buf, "close"))
			{
				LED5 = LED_OFF;
				printf("LED5已关闭。");	
			}
			else if(0==strcmp((const char *)rec_buf, "switch")) 
			{
				LED6 = !LED6;
				if(LED6 == LED_OFF) printf("LED6已关闭。");	
				else if(LED6 == LED_ON) printf("LED6已打开。");
			}
			rec_sta = 0;
		}
		
		
		if(rec_sta2==1)
		{
			printf("接收到的数据为：%s", RS485_RX_BUF);  //这个打印到串口1去了
			
			RS485_Send_Data(rs485buf2,sizeof(rs485buf2));//发送数组
			RS485_Send_Data(rs485buf3,1);
			RS485_Send_Data(RS485_RX_BUF, sizeof(RS485_RX_BUF));			
			rec_sta2= 0;
		}
	}
}
