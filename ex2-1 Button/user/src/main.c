/*****************************************************************************
@Description: 按键KEY1开关LED5，延时消抖方式 
@History: 
		
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "io_bit.h"


int main(void)
{
	u8 key;  //保存按键按下状态
	//硬件初始化
	led_init();
	key_init();
	while(1)
	{
		key = key_scanf();
		if(key == KEY1_OK)  //如果KEY1按键按下
		{
			if(LED5 == LED_OFF) LED5 = LED_ON;
			else LED5 = LED_OFF;
		}
		
	}
}
