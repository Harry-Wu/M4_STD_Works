/*****************************************************************************
@Description: uart1�����жϷ�ʽ������յ����ַ������Ƿ��û��лس����ж��ַ���
�Ƿ��꣬Ȼ����ݽ��յ����ַ���ִ����Ӧ��ָ��
@History: 
		V1.0: ��ʼ�汾
		V1.1������printf��ʾ����
		
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
//	u8 key, rec_data;  //���水������״̬
	//Ӳ����ʼ��
	led_init();
	key_init();
	uart_init(115200);
	printf("�뷢��LED����ָ��Իس�������\n\r");
	printf("\"open\"Ϊ����LED5��\"close\"Ϊ�ر�LED5�� \"switch\"Ϊ�л�LED6���ء�\n\r");
	while(1)
	{
		if(rec_sta==1) //����ַ����������
		{
			if(0==strcmp((const char *)rec_buf, "open"))
			{
				LED5 = LED_ON;
				printf("LED5�Ѵ򿪡�");	
			}
			else if(0==strcmp((const char *)rec_buf, "close"))
			{
				LED5 = LED_OFF;
				printf("LED5�ѹرա�");	
			}
			else if(0==strcmp((const char *)rec_buf, "switch")) 
			{
				LED6 = !LED6;
				if(LED6 == LED_OFF) printf("LED6�ѹرա�");	
				else if(LED6 == LED_ON) printf("LED6�Ѵ򿪡�");
			}
			rec_sta = 0;
		}
		
	}
}
