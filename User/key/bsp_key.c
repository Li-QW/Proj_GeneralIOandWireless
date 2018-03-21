/**
  * @Title  	bsp_key.c
  * @author 	X.W.
  * @date		2018��3��1��
  * @version	V1.0
  * @Description: ʵ�ְ�������
  */
#include "./key/bsp_key.h"
#include "./SysTick/bsp_SysTick.h"

/**
 * @brief  ���ð����õ���I/O��
 * @param  ��
 * @retval ��
 */
void Key_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*���������˿�ʱ��*/
	RCC_APB2PeriphClockCmd(KEY0_GPIO_CLK|KEY1_GPIO_CLK|KEY2_GPIO_CLK|KEY3_GPIO_CLK, ENABLE);

	// ѡ�񰴼�����
	GPIO_InitStructure.GPIO_Pin = KEY0_GPIO_PIN;
	// ���ð�������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�����
	GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
	// ���ð�������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�����
	GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
	// ���ð�������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);

	// ѡ�񰴼�����
	GPIO_InitStructure.GPIO_Pin = KEY3_GPIO_PIN;
	// ���ð�������Ϊ��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	// ʹ�ýṹ���ʼ������
	GPIO_Init(KEY3_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * ��������Key_Scan
 * ����  ������Ƿ��а�������
 * ����  ��uint8_t mode : 0-��֧����������1-֧��������
 * ���  ��KEY0_PRES - KEY0����
 * 	  	   KEY1_PRES - KEY1����
 * 	  	   KEY2_PRES - KEY2����
 * 	  	   KEY3_PRES - KEY3����
 * ע��  ���˺�������Ӧ���ȼ���KEY0>KEY1>KEY2>KEY3!!!
 */
uint8_t Key_Scan(uint8_t mode)
{
	// �����ɿ���־
	static uint8_t keyUp=1;
	// ���mode=1����֧������
	if(mode) keyUp=1;
	// ����Ƿ��а�������
	if(keyUp && (KEY0==0||KEY1==0||KEY2==0||KEY3==1)){
		Delay_ms(10);	//��< ȥ����
		keyUp = 0;
		if(     KEY0 == 0)	return KEY0_PRES;
		else if(KEY1 == 0)	return KEY1_PRES;
		else if(KEY2 == 0) return KEY2_PRES;
		else if(KEY3 == 1) return	KEY3_PRES;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==0){
		keyUp = 1;
	}

	// �ް�������
	return 0;
}

/**
 * END OF FILE
 */
