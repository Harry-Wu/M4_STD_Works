#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{
	//定义GPIO初始化结构体
	GPIO_InitTypeDef GPIO_InitStruct;
	//GPIOA, E端口使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	//端口配置并初始化
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//通过key_scanf的返回值判断有没有按键按下，并且识别是哪个按键按下
//返回值0xff--无按键按下，1--KEY1按下，2--KEY2按下（未定义）。。。
u8 key_scanf(void)
{
	u8 key = NO_KEY;  //初始为没有按键按下
	if(KEY1)  //如果KEY1 IO口识别为高电平
	{
		delay_ms(10);  //延时消抖
		if(KEY1)  //再次判断是否为高电平，如果为高则执行条件语句
		{
			key = KEY1_OK;
			while (KEY1); //等待按键释放，不释放就停在这里
		}
	}
	return key;
}
