/*********************************************************************************************
* �ļ���htu21d.h
* ���ߣ�zonesion
* ˵����htu21dͷ�ļ�
* �޸ģ�Chenkm 2017.01.10 �޸Ĵ����ʽ�����Ӵ���ע�ͺ��ļ�˵��
* ע�ͣ�
*********************************************************************************************/
#ifndef __EXT_HTU21D_H__
#define __EXT_HTU21D_H__

/*********************************************************************************************
* �궨��
*********************************************************************************************/
#define	   EXT_HTU21DADDR           0x80		                //HTU21��IIC��ַ
#define	   EXT_TEMPERATURE          0XF3		                //HTU21���¶ȵ�ַ
#define	   EXT_HUMIDITY             0XF5		                //HTU21��ʪ�ȵ�ַ
/*********************************************************************************************
* �ⲿԭ�ͺ���
*********************************************************************************************/
void  ext_htu21d_init(void);
unsigned char ext_htu21d_read_reg(unsigned char cmd);
int   ext_htu21d_get_data(unsigned char order);

#endif //__HTU21D_H__