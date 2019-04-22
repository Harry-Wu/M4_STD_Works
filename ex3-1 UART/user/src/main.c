/*****************************************************************************
@Description: uart1�����յ������ݷ��ظ���λ�����������ȵķ�ʽ 
@History: 
		
		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "io_bit.h"


int main(void)
{
	u8 key, rec_data;  //���水������״̬
	//Ӳ����ʼ��
	led_init();
	key_init();
	uart_init(115200);
	while(1)
	{
		while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);  //�ȴ��������ݣ�RESET����û�յ�����
		rec_data = USART_ReceiveData(USART1);  //��ȡ��1�ֽ����ݴ�����
		while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);  //�ȴ��ϴη������
		USART_SendData(USART1, rec_data);  //��������
		
//		key = key_scanf();
//		if(key == KEY1_OK)  //���KEY1��������
//		{
//			if(LED5 == LED_OFF) LED5 = LED_ON;
//			else LED5 = LED_OFF;
//		}
		
	}
}
