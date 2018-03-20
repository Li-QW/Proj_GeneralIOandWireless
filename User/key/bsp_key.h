/**
  * @Title  	bsp_key.h
  * @author 	X.W.
  * @date		2018��3��1��
  * @version	V1.0
  * @Description: ���尴��ͷ�ļ�
  */
#ifndef KEY_BSP_KEY_H_
#define KEY_BSP_KEY_H_

#include "stm32f10x.h"

// �������Ŷ���
// ����MINI��KEY0-PF2; KEY1-PF3; KEY2-PF4; KEY3-PF5(�Ӹ�)
// KEY0
#define KEY0_GPIO_CLK		RCC_APB2Periph_GPIOF
#define KEY0_GPIO_PORT		GPIOF
#define KEY0_GPIO_PIN 		GPIO_Pin_2
// KEY1
#define KEY1_GPIO_CLK		RCC_APB2Periph_GPIOF
#define KEY1_GPIO_PORT 	GPIOF
#define KEY1_GPIO_PIN		GPIO_Pin_3
// KEY2
#define KEY2_GPIO_CLK		RCC_APB2Periph_GPIOF
#define KEY2_GPIO_PORT		GPIOF
#define KEY2_GPIO_PIN 		GPIO_Pin_4
// KEY3
#define KEY3_GPIO_CLK		RCC_APB2Periph_GPIOF
#define KEY3_GPIO_PORT		GPIOF
#define KEY3_GPIO_PIN 		GPIO_Pin_5


/**
 * �������±�־��
 */
#define KEY0_PRES	0x01
#define KEY1_PRES 	0x02
#define KEY2_PRES 	0x04
#define KEY3_PRES	0x08

/**
 * ��ȡ������
 */
#define KEY0  	GPIO_ReadInputDataBit(KEY0_GPIO_PORT,KEY0_GPIO_PIN)
#define KEY1  	GPIO_ReadInputDataBit(KEY1_GPIO_PORT,KEY1_GPIO_PIN)
#define KEY2   GPIO_ReadInputDataBit(KEY2_GPIO_PORT,KEY2_GPIO_PIN)
#define KEY3   GPIO_ReadInputDataBit(KEY3_GPIO_PORT,KEY3_GPIO_PIN)

/**
 * ��������
 */
void Key_GPIO_Config(void);
uint8_t Key_Scan(uint8_t mode);

#endif /* KEY_BSP_KEY_H_ */
