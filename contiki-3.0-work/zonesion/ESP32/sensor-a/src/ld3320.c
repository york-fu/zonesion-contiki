/*********************************************************************************************
* �ļ���ld3320.c
* ���ߣ�zonesion
* ˵����ld3320��������
* �޸ģ�Chenkm 2017.01.04 ������ע��
* ע�ͣ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "ld3320.h"
#include "string.h"
#include "delay.h"

void uart2_init(unsigned int bpr);
void uart2_putc( char x);
void  uart2_set_input(int (*fun)( char ) );
int uart_callback(char data);
static unsigned char rx_buf = 0;
char* cmd[13] = {"yu liu zhi ling",                             //Ԥ��ָ��
"da kai tai deng",                             //��̨��
"guan bi tai deng",                            //�ر�̨��
"da kai chuang lian",                          //�򿪴���
"guan bi chuang lian",                         //�رմ���
"da kai feng shan",                            //�򿪷���
"guan bi feng shan",                           //�رշ���
"da kai kong tiao",                            //�򿪿յ�
"guan bi kong tiao",                           //�رտյ�
"da kai jia shi qi",                           //�򿪼�ʪ��
"guan bi jia shi qi",                          //�رռ�ʪ��
"zi dong mo shi",                              //�Զ�ģʽ
"shou dong mo shi",                            //�ֶ�ģʽ
//"que ren",                                     //ȷ��ָ��
};

/*********************************************************************************************
* ���ƣ�ld3320_init()
* ���ܣ�ld3320��ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void ld3320_init()
{
  char i = 0;
  uart2_init(9600); 
  uart2_set_input(uart_callback);
  ld3320_clean();
  delay_ms(100);
  ld3320_reload();
  delay_ms(100);
  for (i=0; i<13; i++){
    ld3320_add(cmd[i]);
    delay_ms(50);
  }
  delay_ms(100);
  ld3320_reload();
}
/*********************************************************************************************
* ���ƣ�ld3320_add()
* ���ܣ�����һ��ʶ�����
* ������s -- ������
* ���أ�
* �޸ģ�
* ע�ͣ����ε��������ٸ�0.1S,�ҵ���ld3320_reload���������Ż���Ч�������쳣ʱģ���̵ƻ���˸
*********************************************************************************************/
void ld3320_add(char *s)
{
  int i;
  int len = strlen(s);
  uart2_putc('{');
  uart2_putc('a');
  uart2_putc('0');
  for (i=0; i<len; i++){
    uart2_putc(s[i]);
  }
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_addrs()
* ���ܣ�����һ��ʶ����䲢�����ַ���
* ������s -- ������� r -- �������
* ���أ�
* �޸ģ�
* ע�ͣ����ε��������ٸ�0.1S,�ҵ���ld3320_reload���������Ż���Ч�������쳣ʱģ���̵ƻ���˸
*********************************************************************************************/
void ld3320_addrs(char *s,char *r)
{
  int i;
  int len = strlen(s);
  uart2_putc('{');
  uart2_putc('a');
  uart2_putc('0');
  for (i=0; i<len; i++){
    uart2_putc(s[i]);
  }
  uart2_putc('|');
  uart2_putc('s');
  uart2_putc('0');
  len = strlen(r);
  for (i=0; i<len; i++){
    uart2_putc(s[i]);
  }
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_addrx()
* ���ܣ�����һ��ʶ����䲢����һ�ֽ�16������
* ������s -- ������ x -- ������
* ���أ�
* �޸ģ�
* ע�ͣ����ε��������ٸ�0.1S,�ҵ���ld3320_reload���������Ż���Ч�������쳣ʱģ���̵ƻ���˸
*********************************************************************************************/
void ld3320_addrx(char *s,unsigned char x)
{
  int i;
  int len = strlen(s);
  uart2_putc('{');
  uart2_putc('a');
  uart2_putc('0');
  for (i=0; i<len; i++){
    uart2_putc(s[i]);
  }
  uart2_putc('|');
  uart2_putc('x');
  uart2_putc('0');
  uart2_putc(x);
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_clean()
* ���ܣ������������б�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ��������б���ģ���̵ƻ���˸
*********************************************************************************************/
void ld3320_clean(void)
{
  uart2_putc('{');
  uart2_putc('c');
  uart2_putc('0');
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_reload()
* ���ܣ����¼�������б�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ��������������¼�������б�
*********************************************************************************************/
void ld3320_reload(void)
{
  uart2_putc('{');
  uart2_putc('l');
  uart2_putc('0');
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_debug()
* ���ܣ�����/�رյ���ģʽ
* ������
* ���أ�
* �޸ģ�
* ע�ͣ��������������¼�������б�
*********************************************************************************************/
void ld3320_debug(unsigned char cmd)
{
  uart2_putc('{');
  uart2_putc('d');
  if(cmd == 1)
    uart2_putc('1');
  else
    uart2_putc('0');
  uart2_putc('}');
}

/*********************************************************************************************
* ���ƣ�ld3320_read
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
char ld3320_read(void)
{
  char x = rx_buf;
  rx_buf = 0;
  return x;
}
int uart_callback(char data)
{
  rx_buf = data;
  return 0;
}