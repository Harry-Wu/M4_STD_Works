/*****************************************************************************
@Description: uart1�����жϷ�ʽ������յ����ַ������Ƿ��û��лس����ж��ַ���
�Ƿ��꣬Ȼ����ݽ��յ����ַ���ִ����Ӧ��ָ��
@History: 
		V1.0: �ϵ�ʱ��һ��Ĭ���ַ�����RS485������ʱRS485�����յ����ַ�������

		
*****************************************************************************/
#include "stm32f4xx.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "io_bit.h"
#include "stdio.h"
#include "string.h"
#include "delay.h"
#include "rs485.h"


int main(void)
{
	u8 rs485buf[6] = "apple";  //���ڷ��͵�����
	u8 rs485buf2[14] = "�յ����ݣ�";
	u8 rs485buf3[1] = {'\n'};
	
	//Ӳ����ʼ��
	led_init();
	key_init();
	uart_init(115200);
	RS485_Init(9600);  //��ʼ��RS485��UART2
	LED5 = LED_ON;
	LED6 = LED_ON;
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	delay_ms(500);
	LED5 = LED_OFF;
	LED6 = LED_OFF;
	RS485_Send_Data(rs485buf,sizeof(rs485buf));//��������
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
		
		
		if(rec_sta2==1)
		{
			printf("���յ�������Ϊ��%s", RS485_RX_BUF);  //�����ӡ������1ȥ��
			
			RS485_Send_Data(rs485buf2,sizeof(rs485buf2));//��������
			RS485_Send_Data(rs485buf3,1);
			RS485_Send_Data(RS485_RX_BUF, sizeof(RS485_RX_BUF));			
			rec_sta2= 0;
		}
	}
}
