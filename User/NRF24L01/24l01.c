#include "./NRF24L01/24l01.h"

    
const uint8_t TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const uint8_t RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
static __IO uint32_t  SPITimeout = SPIT_LONG_TIMEOUT;    
static uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode);

//��ʼ��24L01��IO��
void NRF24L01_Config(void)
{  
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure; 
    
    /* ʹ��SPIʱ�� */
    NRF_SPI_APBxClock_FUN( NRF_SPI_CLK, ENABLE );
    
    /* ʹ��SPI������ص�ʱ�� */
    NRF_SPI_CSN_APBxClock_FUN( NRF_SPI_CSN_CLK, ENABLE );
    NRF_SPI_SCK_APBxClock_FUN( NRF_SPI_SCK_CLK | NRF_SPI_MISO_CLK | NRF_SPI_MOSI_CLK, ENABLE);
    NRF_CE_APBxClock_FUN( NRF_CE_CLK, ENABLE);
    NRF_IRQ_APBxClock_FUN( NRF_IRQ_CLK, ENABLE);

    /* ����SPI�� CSN���ţ���ͨIO */
    GPIO_InitStructure.GPIO_Pin = NRF_SPI_CSN_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    // �������
    GPIO_Init(NRF_SPI_CSN_PORT, &GPIO_InitStructure);

    /* ����NRF��CE���� */
    GPIO_InitStructure.GPIO_Pin = NRF_CE_PIN;
    GPIO_Init(NRF_CE_PORT, &GPIO_InitStructure);

    /* ����SPI�� SCK����*/
    GPIO_InitStructure.GPIO_Pin = NRF_SPI_SCK_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     // �����������
    GPIO_Init(NRF_SPI_SCK_PORT, &GPIO_InitStructure);

    /* ����SPI�� MISO����*/
    GPIO_InitStructure.GPIO_Pin = NRF_SPI_MISO_PIN;
    GPIO_Init(NRF_SPI_MISO_PORT, &GPIO_InitStructure);

    /* ����SPI�� MOSI����*/
    GPIO_InitStructure.GPIO_Pin = NRF_SPI_MOSI_PIN;     
    GPIO_Init(NRF_SPI_MOSI_PORT, &GPIO_InitStructure);

    /* ����NRF��IRQ���� */
    GPIO_InitStructure.GPIO_Pin = NRF_IRQ_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU  ;   //��������
    GPIO_Init(NRF_IRQ_PORT, &GPIO_InitStructure);
    /* ʹ��NRF CE=0*/
    NRF_CE_LOW();
    /* ֹͣ�ź� CSN���Ÿߵ�ƽ*/
    SPI_NRF_CSN_HIGH();

    /* SPI ģʽ���� */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;        //����SPI����ģʽ:����Ϊ��SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;        //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;        //ѡ���˴���ʱ�ӵ���̬:ʱ�����յ͵�ƽ
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;    //���ݲ����ڵ�һ��ʱ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;        //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;        //���岨����Ԥ��Ƶ��ֵ
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;    //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRCֵ����Ķ���ʽ
    SPI_Init(NRF_SPIx, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
    /* ʹ�� SPI  */
    SPI_Cmd(NRF_SPIx , ENABLE);
}

//���24L01�Ƿ����
//����ֵ:0-�ɹ�;1-ʧ��    
uint8_t NRF24L01_Check(void)
{
    uint8_t buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
    uint8_t i;
    ////spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��        
    //xw- SPI1_SetSpeed(SPI_BaudRatePrescaler_8); 
    //д��5���ֽڵĵ�ַ.    
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,buf,5);
    //����д��ĵ�ַ  
    NRF24L01_Read_Buf(TX_ADDR,buf,5); 
    for(i=0;i<5;i++){
      if(buf[i]!=0XA5){
        break;                                    
      }
    }
    if(i!=5){
      return 1;   //���24L01����    
    }
    return 0;     //��⵽24L01
}          

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
uint8_t NRF24L01_Write_Reg(uint8_t reg,uint8_t value)
{
    uint8_t status;                    
    SPI_NRF_CSN_LOW();                    //ʹ��SPI����
    status =NRF24L01_SPI_SendByte(reg);   //���ͼĴ����� 
    NRF24L01_SPI_SendByte(value);         //д��Ĵ�����ֵ
    SPI_NRF_CSN_HIGH();                   //��ֹSPI����       
    return(status);                       //����״ֵ̬
}

//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
uint8_t NRF24L01_Read_Reg(uint8_t reg)
{
    uint8_t reg_val;        
    SPI_NRF_CSN_LOW();                    //ʹ��SPI����        
    NRF24L01_SPI_SendByte(reg);           //���ͼĴ�����
    reg_val=NRF24L01_SPI_ReadByte();      //��ȡ�Ĵ�������
    SPI_NRF_CSN_HIGH();                   //��ֹSPI����            
    return(reg_val);                      //����״ֵ̬
}    

//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
uint8_t NRF24L01_Read_Buf(uint8_t reg,uint8_t *pBuf,uint8_t len)
{
    uint8_t status,u8_ctr;           
    SPI_NRF_CSN_LOW();                    //ʹ��SPI����
    status=NRF24L01_SPI_SendByte(reg);    //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬          
    for(u8_ctr=0;u8_ctr<len;u8_ctr++){
      pBuf[u8_ctr]=NRF24L01_SPI_ReadByte(); //��������
    }
    SPI_NRF_CSN_HIGH();                   //�ر�SPI����
    return status;                        //���ض�����״ֵ̬
}

//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
uint8_t NRF24L01_Write_Buf(uint8_t reg, uint8_t *pBuf, uint8_t len)
{
    uint8_t status,u8_ctr;        
    SPI_NRF_CSN_LOW();                    //ʹ��SPI����
    status = NRF24L01_SPI_SendByte(reg);  //���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
    for(u8_ctr=0; u8_ctr<len; u8_ctr++){
      NRF24L01_SPI_SendByte(*pBuf++);     //д������     
    }
    SPI_NRF_CSN_HIGH();                   //�ر�SPI����
    return status;                        //���ض�����״ֵ̬
}    

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
uint8_t NRF24L01_TxPacket(uint8_t *txbuf)
{
    uint8_t sta;
    ////spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
    //-xw SPI1_SetSpeed(SPI_BaudRatePrescaler_8);
    NRF_CE_LOW();
    //д���ݵ�TX BUF  32���ֽ�
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);
    NRF_CE_HIGH();                        //��������       
    while(NRF_IRQ!=0);                    //�ȴ��������
    sta=NRF24L01_Read_Reg(STATUS);        //��ȡ״̬�Ĵ�����ֵ       
    //���TX_DS��MAX_RT�жϱ�־
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 
    //�ﵽ����ط�����
    if(sta&MAX_TX){
        NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
        return MAX_TX; 
    }
    //�������
    if(sta&TX_OK){
        return TX_OK;
    }
    //����ԭ����ʧ��
    return 0xff;
}

//����NRF24L01����һ������
//rxbuf:�������ݴ���׵�ַ
//����ֵ:0��������ɣ��������������
uint8_t NRF24L01_RxPacket(uint8_t *rxbuf)
{
    uint8_t sta;                                           
    ////spi�ٶ�Ϊ9Mhz��24L01�����SPIʱ��Ϊ10Mhz��   
    //-xw SPI1_SetSpeed(SPI_BaudRatePrescaler_8); 
    //��ȡ״̬�Ĵ�����ֵ         
    sta=NRF24L01_Read_Reg(STATUS);  
    //���TX_DS��MAX_RT�жϱ�־
    NRF24L01_Write_Reg(NRF_WRITE_REG+STATUS,sta); 
    //���յ�����
    if(sta&RX_OK){
        //��ȡ����
        NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);
        //���RX FIFO�Ĵ��� 
        NRF24L01_Write_Reg(FLUSH_RX,0xff);
        return 0; 
    }       
    //û�յ��κ�����
    return 1;
}   

//��ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������           
void NRF24L01_RX_Mode(void)
{
    NRF_CE_LOW();      
    //дRX�ڵ��ַ
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,
                       (u8*)RX_ADDRESS,RX_ADR_WIDTH);
  
    //ʹ��ͨ��0���Զ�Ӧ��    
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);        
    //ʹ��ͨ��0�Ľ��յ�ַ       
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01);
    //����RFͨ��Ƶ��          
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);            
    //ѡ��ͨ��0����Ч���ݿ��         
    NRF24L01_Write_Reg(NRF_WRITE_REG+RX_PW_P0,RX_PLOAD_WIDTH);
    //����TX�������,0db����,2Mbps,���������濪��   
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);    
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG, 0x0f);        
    //CEΪ��,�������ģʽ 
    NRF_CE_HIGH(); 
}      

//��ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������           
//CEΪ�ߴ���10us,����������.     
void NRF24L01_TX_Mode(void)
{                                                         
    NRF_CE_LOW();        
    //дTX�ڵ��ַ 
    NRF24L01_Write_Buf(NRF_WRITE_REG+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);
    //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK      
    NRF24L01_Write_Buf(NRF_WRITE_REG+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); 

    //ʹ��ͨ��0���Զ�Ӧ��    
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_AA,0x01);     
    //ʹ��ͨ��0�Ľ��յ�ַ  
    NRF24L01_Write_Reg(NRF_WRITE_REG+EN_RXADDR,0x01); 
    //�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
    NRF24L01_Write_Reg(NRF_WRITE_REG+SETUP_RETR,0x1a);
    //����RFͨ��Ϊ40
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_CH,40);       
    //����TX�������,0db����,2Mbps,���������濪��   
    NRF24L01_Write_Reg(NRF_WRITE_REG+RF_SETUP,0x0f);  
    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
    NRF24L01_Write_Reg(NRF_WRITE_REG+CONFIG,0x0e);    
    //CEΪ��,10us����������
    NRF_CE_HIGH();
}

/*��������������Fire Tutorial--------------------------------------------------------*/

 /**
  * @brief  ʹ��SPI��ȡһ���ֽڵ�����
  * @param  ��
  * @retval ���ؽ��յ�������
  */
uint8_t NRF24L01_SPI_ReadByte(void)
{
  return (NRF24L01_SPI_SendByte(Dummy_Byte));
}

 /**
  * @brief  ʹ��SPI����һ���ֽڵ�����
  * @param  byte��Ҫ���͵�����
  * @retval ���ؽ��յ�������
  */
uint8_t NRF24L01_SPI_SendByte(uint8_t byte)
{
   SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ͻ�����Ϊ�գ�TXE�¼� */
  while (SPI_I2S_GetFlagStatus(NRF_SPIx , SPI_I2S_FLAG_TXE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(0);
   }

  /* д�����ݼĴ�������Ҫд�������д�뷢�ͻ����� */
  SPI_I2S_SendData(NRF_SPIx, byte);

  SPITimeout = SPIT_FLAG_TIMEOUT;
  /* �ȴ����ջ������ǿգ�RXNE�¼� */
  while (SPI_I2S_GetFlagStatus(NRF_SPIx, SPI_I2S_FLAG_RXNE) == RESET)
  {
    if((SPITimeout--) == 0) return SPI_TIMEOUT_UserCallback(1);
   }

  /* ��ȡ���ݼĴ�������ȡ���ջ��������� */
  return SPI_I2S_ReceiveData(NRF_SPIx );
}

static  uint16_t SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
  /* �ȴ���ʱ��Ĵ���,���������Ϣ */
  FLASH_ERROR("SPI �ȴ���ʱ!errorCode = %d",errorCode);
  return 0;
}





