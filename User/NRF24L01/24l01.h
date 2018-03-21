/*��ֲ������ԭ�ӽ̳�*/
#ifndef __24L01_H
#define __24L01_H             
#include "stm32f10x.h"

/*----------------------------------------------------------------------------*/
//NRF24L01�Ĵ�����������
#define NRF_READ_REG    0x00  //�����üĴ���,��5λΪ�Ĵ�����ַ
#define NRF_WRITE_REG   0x20  //д���üĴ���,��5λΪ�Ĵ�����ַ
#define RD_RX_PLOAD     0x61  //��RX��Ч����,1~32�ֽ�
#define WR_TX_PLOAD     0xA0  //дTX��Ч����,1~32�ֽ�
#define FLUSH_TX        0xE1  //���TX FIFO�Ĵ���.����ģʽ����
#define FLUSH_RX        0xE2  //���RX FIFO�Ĵ���.����ģʽ����
#define REUSE_TX_PL     0xE3  //����ʹ����һ������,CEΪ��,���ݰ������Ϸ���.
#define NOP             0xFF  //�ղ���,����������״̬�Ĵ���   
//SPI(NRF24L01)�Ĵ�����ַ
#define CONFIG          0x00  //���üĴ�����ַ;bit0:1����ģʽ,0����ģʽ;bit1:��ѡ��;bit2:CRCģʽ;bit3:CRCʹ��;
                              //bit4:�ж�MAX_RT(�ﵽ����ط������ж�)ʹ��;bit5:�ж�TX_DSʹ��;bit6:�ж�RX_DRʹ��
#define EN_AA           0x01  //ʹ���Զ�Ӧ����  bit0~5,��Ӧͨ��0~5
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
#define SETUP_AW        0x03  //���õ�ַ���(��������ͨ��):bit1,0:00,3�ֽ�;01,4�ֽ�;02,5�ֽ�;
#define SETUP_RETR      0x04  //�����Զ��ط�;bit3:0,�Զ��ط�������;bit7:4,�Զ��ط���ʱ 250*x+86us
#define RF_CH           0x05  //RFͨ��,bit6:0,����ͨ��Ƶ��;
#define RF_SETUP        0x06  //RF�Ĵ���;bit3:��������(0:1Mbps,1:2Mbps);bit2:1,���书��;bit0:�������Ŵ�������
#define STATUS          0x07  //״̬�Ĵ���;bit0:TX FIFO����־;bit3:1,��������ͨ����(���:6);bit4,�ﵽ�����ط�
                              //bit5:���ݷ�������ж�;bit6:���������ж�;
#define MAX_TX          0x10  //�ﵽ����ʹ����ж�
#define TX_OK           0x20  //TX��������ж�
#define RX_OK           0x40  //���յ������ж�

#define OBSERVE_TX      0x08  //���ͼ��Ĵ���,bit7:4,���ݰ���ʧ������;bit3:0,�ط�������
#define CD              0x09  //�ز����Ĵ���,bit0,�ز����;
#define RX_ADDR_P0      0x0A  //����ͨ��0���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P1      0x0B  //����ͨ��1���յ�ַ,��󳤶�5���ֽ�,���ֽ���ǰ
#define RX_ADDR_P2      0x0C  //����ͨ��2���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P3      0x0D  //����ͨ��3���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P4      0x0E  //����ͨ��4���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define RX_ADDR_P5      0x0F  //����ͨ��5���յ�ַ,����ֽڿ�����,���ֽ�,����ͬRX_ADDR_P1[39:8]���;
#define TX_ADDR         0x10  //���͵�ַ(���ֽ���ǰ),ShockBurstTMģʽ��,RX_ADDR_P0��˵�ַ���
#define RX_PW_P0        0x11  //��������ͨ��0��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P1        0x12  //��������ͨ��1��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P2        0x13  //��������ͨ��2��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P3        0x14  //��������ͨ��3��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P4        0x15  //��������ͨ��4��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define RX_PW_P5        0x16  //��������ͨ��5��Ч���ݿ��(1~32�ֽ�),����Ϊ0��Ƿ�
#define NRF_FIFO_STATUS 0x17  //FIFO״̬�Ĵ���;bit0,RX FIFO�Ĵ����ձ�־;bit1,RX FIFO����־;bit2,3,����
                              //bit4,TX FIFO�ձ�־;bit5,TX FIFO����־;bit6,1,ѭ��������һ���ݰ�.0,��ѭ��;
/*----------------------------------------------------------------------------*/
/*SPI�ӿڶ���*/
#define NRF_SPIx                        SPI1
#define NRF_SPI_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define NRF_SPI_CLK                     RCC_APB2Periph_SPI1

//SCK����
#define NRF_SPI_SCK_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define NRF_SPI_SCK_CLK                 RCC_APB2Periph_GPIOA   
#define NRF_SPI_SCK_PORT                GPIOA   
#define NRF_SPI_SCK_PIN                 GPIO_Pin_5
//MOSI����
#define NRF_SPI_MOSI_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define NRF_SPI_MOSI_CLK                RCC_APB2Periph_GPIOA    
#define NRF_SPI_MOSI_PORT               GPIOA 
#define NRF_SPI_MOSI_PIN                GPIO_Pin_7
//MISO����
#define NRF_SPI_MISO_APBxClock_FUN      RCC_APB2PeriphClockCmd
#define NRF_SPI_MISO_CLK                RCC_APB2Periph_GPIOA    
#define NRF_SPI_MISO_PORT               GPIOA 
#define NRF_SPI_MISO_PIN                GPIO_Pin_6
//CSN����
#define NRF_SPI_CSN_APBxClock_FUN       RCC_APB2PeriphClockCmd
#define NRF_SPI_CSN_CLK                 RCC_APB2Periph_GPIOF   
#define NRF_SPI_CSN_PORT                GPIOF
#define NRF_SPI_CSN_PIN                 GPIO_Pin_0
//CE���� 
#define NRF_CE_APBxClock_FUN            RCC_APB2PeriphClockCmd
#define NRF_CE_CLK                      RCC_APB2Periph_GPIOA   
#define NRF_CE_PORT                     GPIOA
#define NRF_CE_PIN                      GPIO_Pin_4
//IRQ����
#define NRF_IRQ_APBxClock_FUN           RCC_APB2PeriphClockCmd
#define NRF_IRQ_CLK                     RCC_APB2Periph_GPIOF   
#define NRF_IRQ_PORT                    GPIOF
#define NRF_IRQ_PIN                     GPIO_Pin_0
//24L01������
#define SPI_NRF_CSN_LOW()               GPIO_ResetBits( NRF_SPI_CSN_PORT, NRF_SPI_CSN_PIN )
#define SPI_NRF_CSN_HIGH()              GPIO_SetBits( NRF_SPI_CSN_PORT, NRF_SPI_CSN_PIN )
#define NRF_CE_HIGH()                   GPIO_SetBits( NRF_CE_PORT, NRF_CE_PIN)
#define NRF_CE_LOW()                    GPIO_ResetBits( NRF_CE_PORT, NRF_CE_PIN)
#define NRF_IRQ                         GPIO_ReadInputDataBit( NRF_IRQ_PORT, NRF_IRQ_PIN)
/*�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT               ((uint32_t)0x1000)
#define SPIT_LONG_TIMEOUT               ((uint32_t)(10 * SPIT_FLAG_TIMEOUT))
/*��Ϣ���*/
#define FLASH_DEBUG_ON         1

#define FLASH_INFO(fmt,arg...)           printf("<<-FLASH-INFO->> "fmt"\n",##arg)
#define FLASH_ERROR(fmt,arg...)          printf("<<-FLASH-ERROR->> "fmt"\n",##arg)
#define FLASH_DEBUG(fmt,arg...)          do{\
                                          if(FLASH_DEBUG_ON)\
                                          printf("<<-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                          }while(0)
#define Dummy_Byte                      0xFF

//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5       //5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5       //5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  32      //32�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  32      //32�ֽڵ��û����ݿ��
                                           

void NRF24L01_Config(void);//��ʼ��
void NRF24L01_RX_Mode(void);//����Ϊ����ģʽ
void NRF24L01_TX_Mode(void);//����Ϊ����ģʽ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);//д������
uint8_t NRF24L01_Read_Buf(uint8_t reg, uint8_t *pBuf, uint8_t u8s);//��������         
uint8_t NRF24L01_Read_Reg(uint8_t reg);         //���Ĵ���
uint8_t NRF24L01_Write_Reg(uint8_t reg, uint8_t value);//д�Ĵ���
uint8_t NRF24L01_Check(void);//���24L01�Ƿ����
uint8_t NRF24L01_TxPacket(uint8_t *txbuf);//����һ����������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf);//����һ����������

uint8_t NRF24L01_SPI_ReadByte(void);
uint8_t NRF24L01_SPI_SendByte(uint8_t byte);
#endif











