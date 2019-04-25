#include "stm32f4xx.h"
#include "delay.h"

//延时n微秒
//参数n: us, 最大延时798915us
void delay_us(u32 n)
{
	SysTick->LOAD =n*SYSCLK/8; //设定计数次数
	SysTick->VAL =0;     //重装载（重装载寄存器的值复制到计数器）
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开systick定时器
	while(!(SysTick->CTRL & (SysTick_CTRL_COUNTFLAG_Msk)));  //如果Systick没计到0就死等
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); //关systick定时器
}

//延时n毫秒
//参数n: ms, 最大延时798.915ms
void delay_ms(u32 n)
{
	SysTick->LOAD =n*1000*SYSCLK/8; //设定计数次数
	SysTick->VAL =0;     //重装载（重装载寄存器的值复制到计数器）
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk; //开systick定时器
	while(!(SysTick->CTRL & (SysTick_CTRL_COUNTFLAG_Msk)));  //如果Systick没计到0就死等
	SysTick->CTRL &= ~(SysTick_CTRL_ENABLE_Msk); //关systick定时器
}
