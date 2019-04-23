#include "stm32f4xx.h"
#include "key.h"
#include "delay.h"

void key_init(void)
{
	//����GPIO��ʼ���ṹ��
	GPIO_InitTypeDef GPIO_InitStruct;
	//GPIOA, E�˿�ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE);
	//�˿����ò���ʼ��
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

//ͨ��key_scanf�ķ���ֵ�ж���û�а������£�����ʶ�����ĸ���������
//����ֵ0xff--�ް������£�1--KEY1���£�2--KEY2���£�δ���壩������
u8 key_scanf(void)
{
	u8 key = NO_KEY;  //��ʼΪû�а�������
	if(KEY1)  //���KEY1 IO��ʶ��Ϊ�ߵ�ƽ
	{
		delay_ms(10);  //��ʱ����
		if(KEY1)  //�ٴ��ж��Ƿ�Ϊ�ߵ�ƽ�����Ϊ����ִ���������
		{
			key = KEY1_OK;
			while (KEY1); //�ȴ������ͷţ����ͷž�ͣ������
		}
	}
	return key;
}
