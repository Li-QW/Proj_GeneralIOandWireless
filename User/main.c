#include "stm32f10x.h"
#include "./SysTick/bsp_SysTick.h"
#include "./usart/bsp_usart.h"
#include "./led/bsp_led.h"
#include "./key/bsp_key.h"
#include "./NRF24L01/24l01.h"
/*
 * ��������main
 * ����  ��������
 * ����  ����
 * ���  ����
 */
int main(void)
{
	uint8_t key, mode;
	uint16_t t = 0;
	uint8_t tmp_buf[33];
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
	/* ���ô���Ϊ��115200 8-N-1 */
	USART_Config();
	/* ��ʼ����LED���ӵ�Ӳ���ӿ� */
	LED_GPIO_Config();
	/* ������ʼ�� */										
	Key_GPIO_Config();
	/* ����ģ���ʼ�� */
	NRF24L01_Config();

	/* �������ģ���Ƿ���λ */
	printf("\r\n��ʼ�������ģ���Ƿ���λ...\r\n");
	while (NRF24L01_Check()) 
	{
		printf("\rNRF24L01 Error");
		Delay_ms(200);
		printf("\r               ");
		Delay_ms(200);
	}
	printf("\rNRF24L01 Check - OK");

	/* ���ݰ���ȷ�������ĸ�ģʽ!*/
	while (1) 
	{
		key = Key_Scan(0);
		if (key == KEY0_PRES)
		{
			mode = 0;	// RXģʽ
			break;
		}
		else if (key == KEY1_PRES)
		{
			mode = 1;	// TXģʽ
			break;
		}
		/* ��˸��ʾ������Ϣ */
		t++;
		if (t == 100)
			printf("\r\n> KEY0:RX_Mode  KEY1:TX_Mode"); 
		if (t == 200)
		{
			printf("\r                               ");
			t = 0;
		}
		Delay_ms(5);
	}
	printf("\r                               "); //��հ�����ʾ
	
	/* ��ΪRXģʽ */
	if (mode == 0)
	{
		printf("\rNRF24L01 RX_Mode\r\n");
		printf("Received DATA:\r\n");
		NRF24L01_RX_Mode();
		while (1)
		{
			/* һ�����յ���Ϣ,����ʾ����. */
			if (NRF24L01_RxPacket(tmp_buf) == 0) 
			{
				tmp_buf[32] = 0; //�����ַ���������
				printf("%s", tmp_buf);
			}
			else
				delay_us(100);
			t++;
			if (t == 10000) //��Լ1s�Ӹı�һ��LED0״̬
			{
				t = 0;
				LED0_TOGGLE;
			}
		}
	}
	/* ��֮����ΪTXģʽ */
	else 
	{
		printf("\rNRF24L01 TX_Mode\r\n");
		NRF24L01_TX_Mode();
		//�����͵��ַ����ӿո����ʼ
		mode = ' '; 
		while (1)
		{
			if (NRF24L01_TxPacket(tmp_buf) == TX_OK)
			{
				printf("Sended DATA:");
				printf("%s", tmp_buf);
				key = mode;
				for (t = 0; t < 32; t++)
				{
					key++;
					if (key > ('~'))
						key = ' ';
					tmp_buf[t] = key;
				}
				mode++;
				if (mode > '~')
					mode = ' ';
				tmp_buf[32] = 0; //���������
			}
			else
			{
				printf("Send Failed ");
			}
			LED0_TOGGLE;
			Delay_ms(1500);
		}
	}
}