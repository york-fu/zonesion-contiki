/*********************************************************************************************
* �ļ���iic.c
* ���ߣ�zonesion
* ˵����iic��������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "stm32f4xx.h"
#include "delay.h"
#include "iic.h"
/*********************************************************************************************
* �궨��
*********************************************************************************************/
#define I2C_GPIO                GPIOB
#define I2C_CLK                 RCC_AHB1Periph_GPIOB
#define PIN_SCL                 GPIO_Pin_8 
#define PIN_SDA                 GPIO_Pin_9  

#define SDA_R                   GPIO_ReadInputDataBit(I2C_GPIO,PIN_SDA)

/*********************************************************************************************
* ���ƣ�iic_init()
* ���ܣ�I2C��ʼ������
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void iic_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(I2C_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin = PIN_SCL | PIN_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(I2C_GPIO, &GPIO_InitStructure);  
  
}

/*********************************************************************************************
* ���ƣ�sda_out()
* ���ܣ�����SDAΪ���
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sda_out(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = PIN_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(I2C_GPIO, &GPIO_InitStructure);  
}

/*********************************************************************************************
* ���ƣ�sda_in()
* ���ܣ�����SDAΪ����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sda_in(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = PIN_SDA;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  
  GPIO_Init(I2C_GPIO, &GPIO_InitStructure);  
}

/*********************************************************************************************
* ���ƣ�iic_start()
* ���ܣ�I2C��ʼ�ź�
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void iic_start(void)
{
  sda_out();
  GPIO_SetBits(I2C_GPIO,PIN_SDA);                               //����������
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                               //����ʱ����
  delay(5);                                                  //��ʱ
  GPIO_ResetBits(I2C_GPIO,PIN_SDA);                             //�����½���
  delay(5);                                                  //��ʱ
  GPIO_ResetBits(I2C_GPIO,PIN_SCL);                             //����ʱ����
}

/*********************************************************************************************
* ���ƣ�iic_stop()
* ���ܣ�I2Cֹͣ�ź�
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void iic_stop(void)
{
  sda_out();
  GPIO_ResetBits(I2C_GPIO,PIN_SDA);                             //����������
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                               //����ʱ����
  delay(5);                                                  //��ʱ5us
  GPIO_SetBits(I2C_GPIO,PIN_SDA);                               //����������
  delay(5);                                                  //��ʱ5us
}

/*********************************************************************************************
* ���ƣ�iic_send_ack()
* ���ܣ�I2C����Ӧ��
* ������ack -- Ӧ���ź�
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void iic_send_ack(int ack)
{
  sda_out();
  if(ack)
    GPIO_SetBits(I2C_GPIO,PIN_SDA);                             //дӦ���ź�
  else
    GPIO_ResetBits(I2C_GPIO,PIN_SCL); 
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                               //����ʱ����
  delay(5);                                                  //��ʱ
  GPIO_ResetBits(I2C_GPIO,PIN_SCL);                             //����ʱ����
  delay(5);                                                  //��ʱ
}

/*********************************************************************************************
* ���ƣ�iic_recv_ack()
* ���ܣ�I2C����Ӧ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int iic_recv_ack(void)
{
  int CY = 0;
  sda_in();
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                               //����ʱ����
  delay(5);                                                  //��ʱ
  CY = SDA_R;                                                   //��Ӧ���ź�
  GPIO_ResetBits(I2C_GPIO,PIN_SDA);                             //����ʱ����
  delay(5);                                                  //��ʱ
  return CY;
}

/*********************************************************************************************
* ���ƣ�iic_write_byte()
* ���ܣ�I2Cдһ���ֽ����ݣ�����ACK����NACK���Ӹߵ��ͣ����η���
* ������data -- Ҫд������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char iic_write_byte(unsigned char data)
{
  unsigned char i;
  sda_out();
  GPIO_ResetBits(I2C_GPIO,PIN_SCL);                             //����ʱ����
  for(i = 0;i < 8;i++){                                         
    if(data & 0x80){                                            //�ж��������λ�Ƿ�Ϊ1
      GPIO_SetBits(I2C_GPIO,PIN_SDA);  	                                                
    }
    else
      GPIO_ResetBits(I2C_GPIO,PIN_SDA); 
    delay(5);                                                //��ʱ5us
    GPIO_SetBits(I2C_GPIO,PIN_SCL); 	                        //���SDA�ȶ�������SCL���������أ��ӻ���⵽��������ݲ���
    delay(5);                                                //��ʱ5us
    GPIO_ResetBits(I2C_GPIO,PIN_SCL);                           //����ʱ����
    delay(5);                                                //��ʱ5us
    data <<= 1;                                                 //��������һλ
  } 
  delay(2);                                                  //��ʱ2us
  sda_in();
  GPIO_SetBits(I2C_GPIO,PIN_SDA);                               //����������
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                               //����ʱ����
  delay(2);                                                  //��ʱ2us���ȴ��ӻ�Ӧ��
  if(SDA_R){			                                //SDAΪ�ߣ��յ�NACK
    return 1;	
  }else{ 				                        //SDAΪ�ͣ��յ�ACK
    GPIO_ResetBits(I2C_GPIO,PIN_SCL); 
    delay(5);
    return 0;
  }
}

/*********************************************************************************************
* ���ƣ�iic_read_byte()
* ���ܣ�I2Cдһ���ֽ����ݣ�����ACK����NACK���Ӹߵ��ͣ����η���
* ������data -- Ҫд������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char iic_read_byte(unsigned char ack)
{
  unsigned char i,data = 0;
  sda_in();
  GPIO_ResetBits(I2C_GPIO,PIN_SCL); 
  GPIO_SetBits(I2C_GPIO,PIN_SDA);  			        //�ͷ�����	
  for(i = 0;i < 8;i++){
    GPIO_SetBits(I2C_GPIO,PIN_SCL);  		                //����������
    delay(3);	                                        //��ʱ�ȴ��ź��ȶ�
    data <<= 1;	
    if(SDA_R){ 		                                        //������ȡ����
      data |= 0x01;
    }else{
      data &= 0xfe;
    }
    delay(1);
    GPIO_ResetBits(I2C_GPIO,PIN_SCL);    		        //�½��أ��ӻ�������һλֵ
    delay(2);
  }
  sda_out();
  if(ack)
    GPIO_SetBits(I2C_GPIO,PIN_SDA);	                        //Ӧ��״̬
  else
    GPIO_ResetBits(I2C_GPIO,PIN_SDA);  
  delay(1);
  GPIO_SetBits(I2C_GPIO,PIN_SCL);                        
  delay(5);          
  GPIO_ResetBits(I2C_GPIO,PIN_SCL); 
  delay(5);
  return data;
}

/*********************************************************************************************
* ���ƣ�delay()
* ���ܣ���ʱ
* ������t -- ����ʱ��
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void delay(unsigned int t)
{
  unsigned char i;
  while(t--){
    for(i = 0;i < 200;i++);
  }		
}