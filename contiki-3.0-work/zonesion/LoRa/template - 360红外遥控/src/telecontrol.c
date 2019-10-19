#include <string.h>
#include "telecontrol.h"
#include "usart.h"

#define LEARN_CMD               0xF0                            // ѧϰģʽ        
#define EXIT_CMD                0xF2                            // ң��ģʽ        
#define LEARN_SUCCESS_RET       0x00                            // ѧϰ�ɹ�
#define LEARN_ERROR_RET         0xFF                            // ѧϰʧ��

uint8_t currentMode = 0;                                        // ��ǰģʽ
uint8_t currentStatus = 0;                                      // ��ǰѧϰģʽ
uint8_t keyValue = 0;                                           // ��ǰ����ֵ
uint8_t handleFlag = 0;                                         // ѧϰģʽ����״̬

/*********************************************************************************************
* ���ƣ�uart2_set_input_call()
* ���ܣ����ô���2�ص�����
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart2_set_input_call(int (*c)(char ch))
{
  uart2_set_input(c);
}

/*********************************************************************************************
* ���ƣ�uart2_send_data()
* ���ܣ�����2��������
* ������buf-��Ҫ���͵�����/len-���ݳ���
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart2_send_data(char* buf, unsigned int len)
{
  for(unsigned int i=0; i<len; i++)
    uart2_putc(buf[i]);
}

/*********************************************************************************************
* ���ƣ�telecontrol_init()
* ���ܣ�����ң�س�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void telecontrol_init(void)
{
  uart2_init(9600);
  uart2_set_input_call(uart2_rec_handle);
}

/*********************************************************************************************
* ���ƣ�set_currentMode()
* ���ܣ����õ�ǰң��ģʽ
* ������mode-��ǰң��ģʽ
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void set_currentMode(uint8_t mode)
{
  if(mode)
    uart2_putc(LEARN_CMD);
  else
    uart2_putc(EXIT_CMD);
}

/*********************************************************************************************
* ���ƣ�get_currentMode()
* ���ܣ���ȡ��ǰң��ģʽ
* ��������
* ���أ���ǰң��ģʽ
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
uint8_t get_currentMode(void)
{
  return currentMode;
}

/*********************************************************************************************
* ���ƣ�get_currentStatus()
* ���ܣ���ȡ��ǰѧϰ״̬
* ������currentStatus-��ǰѧϰ״̬
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
uint8_t get_currentStatus(void)
{
  return currentStatus;
}

/*********************************************************************************************
* ���ƣ�set_currentStatus()
* ���ܣ����õ�ǰѧϰ״̬
* ��������
* ���أ���ǰѧϰ״̬
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void set_currentStatus(uint8_t status)
{
  currentStatus = status;
}

/*********************************************************************************************
* ���ƣ�set_keyValue()
* ���ܣ����õ�ǰ��ֵ
* ������value-��ǰ��ֵ
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void set_keyValue(uint8_t value)
{
  keyValue = value;
}

/*********************************************************************************************
* ���ƣ�send_keyValue()
* ���ܣ����ڷ��ͼ�ֵ
* ������value-��Ҫ���͵ļ�ֵ
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void send_keyValue(uint8_t value)
{
  uart2_putc(value);
}

uint8_t get_handleFlag(void)
{
  return handleFlag;
}

void set_handleFlag(uint8_t value)
{
  handleFlag = value;
}
/*********************************************************************************************
* ���ƣ�uart2_rec_handle()
* ���ܣ�����2�ص�����
* ������ch-���ڽ��յ�����
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int uart2_rec_handle(char ch)
{
  static uint8_t error = 0;
  if(ch == LEARN_CMD)
    currentMode = 1;
  if(ch == EXIT_CMD)
    currentMode = 0;
  if(ch == LEARN_SUCCESS_RET)
  {
    if(currentMode == 1)
    {
      set_currentMode(0x00);
      currentStatus = 1;
    }
  }
  if(ch == LEARN_ERROR_RET)                                     
  {
    if(currentMode == 1)                                        // ѧϰʧ��  �ٴν���ѧϰ�����ֵ    
    {
      error++;
      if(error <= 3)                                            // ѧϰʧ�ܲ���������
        send_keyValue(keyValue);    
      else                                                      // ѧϰʧ�ܳ�������
      {
        set_currentMode(0x00);                                  // �˳�ѧϰģʽ
        error = 0;
      }
    }
  }
  handleFlag = 1;
  return 1;
}












