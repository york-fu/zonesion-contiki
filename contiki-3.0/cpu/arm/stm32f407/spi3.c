#include "spi.h"	 
	 
//������SPIģ��ĳ�ʼ�����룬���ó�����ģʽ 						  
/*********************************************************************************************
* ���ƣ�SPI3_Init
* ���ܣ�SPI3��ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void SPI3_Init(void)
{	 
  GPIO_InitTypeDef  GPIO_InitStructure;
  SPI_InitTypeDef  SPI_InitStructure;
	
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);//ʹ��GPIOBʱ��
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);//ʹ��SPI3ʱ��
 
  //GPIOB3��4��5��ʼ������
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;//PB3~5���ù������	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//���ù���
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
  GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��
	
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI3); //PB3����Ϊ SPI3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI3); //PB4����Ϊ SPI3
  GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI3); //PB5����Ϊ SPI3	
 
  //����ֻ���SPI�ڳ�ʼ��
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,ENABLE);//��λSPI3
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3,DISABLE);//ֹͣ��λSPI3

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ�����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
  SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
  SPI_Init(SPI3, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
  SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����

  SPI3_ReadWriteByte(0xff);//��������		 
}   

/*********************************************************************************************
* ���ƣ�SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
* ���ܣ�SPI3�ٶ����ú���
* ������SPI_BaudRatePrescaler
* ���أ�
* �޸ģ�
* ע�ͣ�SPI�ٶ�=fAPB2/��Ƶϵ��
	SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256  
	fAPB2ʱ��һ��Ϊ84Mhz
*********************************************************************************************/
void SPI3_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));//�ж���Ч��
	SPI3->CR1&=0XFFC7;//λ3-5���㣬�������ò�����
	SPI3->CR1|=SPI_BaudRatePrescaler;	//����SPI1�ٶ� 
	SPI_Cmd(SPI2,ENABLE); //ʹ��SPI1
} 

/*********************************************************************************************
* ���ƣ�SPI3_ReadWriteByte(u8 TxData)
* ���ܣ�SPI3 ��дһ���ֽ�
* ������TxData:Ҫд����ֽ�
* ���أ���ȡ�����ֽ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
u8 SPI3_ReadWriteByte(u8 TxData)
{		 			 
 
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET){}//�ȴ���������  
	
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ��byte  ����
		
  while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET){} //�ȴ�������һ��byte  
 
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����	
 		    
}