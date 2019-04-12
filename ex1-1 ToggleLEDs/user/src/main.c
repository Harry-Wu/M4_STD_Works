/*****************************************************************************
@Description: ��˸LED 
@History: 
		V1.0: ��ʼ��GPIOF��Ϊ�������
		
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
	//Ӳ����ʼ��
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
