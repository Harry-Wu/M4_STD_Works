/*****************************************************************************
@Description: 闪烁LED 
@History: 
		V1.0: 初始化GPIOF口为推挽输出
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "io_bit.h"

void delay(u32 i)
{
	while(i--);
}

int main(void)
{
	//硬件初始化
	led_init();
	while(1)
	{
		LED5 = LED_ON;
		LED6 = LED_ON;
		delay(0x1000000);
		LED5 = LED_OFF;
		LED6 = LED_OFF;
		delay(0x1000000);
		
	}
}
