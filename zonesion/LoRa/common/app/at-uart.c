/*********************************************************************************************
* �ļ���au-uart.c
* ���ߣ�xuzhy 
* ˵����
*       
*       
*      
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include <stdio.h>
#include "usart.h"

/*********************************************************************************************
* ���ƣ�at_uart_init
* ���ܣ�at���ڳ�ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void at_uart_init(void)
{
}

/*********************************************************************************************
* ���ƣ�at_uart_write
* ���ܣ���at����дһ���ֽ�
* ������ch���������ֽ�
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void at_uart_write(char ch)
{
#if defined(WITH_xLab_AP) || defined(LORA_Serial)
  uart2_putc(ch);
#else
  uart3_putc(ch);
#endif
}

/*********************************************************************************************
* ���ƣ�at_uart_set_input_call
* ���ܣ�����at���ڽ������ݴ�������
* ������c���������ݴ�������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void at_uart_set_input_call(int (*c)(char ch))
{
#if defined(WITH_xLab_AP) || defined(LORA_Serial) 
  uart2_set_input(c);
#else
  uart3_set_input(c);
#endif  
}
