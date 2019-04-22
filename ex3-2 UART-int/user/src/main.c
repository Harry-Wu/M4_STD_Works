/*****************************************************************************
@Description: uart1采用中断方式处理接收到的字符，以是否用换行回车符判断字符串
是否发完，然后根据接收到的字符串执行相应的指令
@History: 
		V1.0: 初始版本
		V1.1：加上printf提示文字
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "io_bit.h"
#include "stdio.h"
#include "string.h"


int main(void)
{
//	u8 key, rec_data;  //保存按键按下状态
	//硬件初始化
	led_init();
	key_init();
	uart_init(115200);
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
		
	}
}
