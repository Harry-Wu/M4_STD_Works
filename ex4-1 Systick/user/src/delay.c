#include "stm32f4xx.h"
#include "delay.h"

//��ʱn΢��
//����n: us, �����ʱ798915us
void delay_us(u32 n)
{
	SysTick->LOAD =n*SYSCLK/8; //�趨��������
	SysTick->VAL =0;     //��װ�أ���װ�ؼĴ�����ֵ���Ƶ���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��systick��ʱ��
	while(!(SysTick->CTRL & (SysTick_CTRL_COUNTFLAG_Msk)));  //���Systickû�Ƶ�0������
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); //��systick��ʱ��
}

//��ʱn����
//����n: ms, �����ʱ798.915ms
void delay_ms(u32 n)
{
	SysTick->LOAD =n*1000*SYSCLK/8; //�趨��������
	SysTick->VAL =0;     //��װ�أ���װ�ؼĴ�����ֵ���Ƶ���������
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //��systick��ʱ��
	while(!(SysTick->CTRL & (SysTick_CTRL_COUNTFLAG_Msk)));  //���Systickû�Ƶ�0������
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); //��systick��ʱ��
}
