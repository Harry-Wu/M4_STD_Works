/*****************************************************************************
@Description: ����KEY1����LED5����ʱ������ʽ 
@History: 
		
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "io_bit.h"


int main(void)
{
	u8 key;  //���水������״̬
	//Ӳ����ʼ��
	led_init();
	key_init();
	while(1)
	{
		key = key_scanf();
		if(key == KEY1_OK)  //���KEY1��������
		{
			if(LED5 == LED_OFF) LED5 = LED_ON;
			else LED5 = LED_OFF;
		}
		
	}
}
