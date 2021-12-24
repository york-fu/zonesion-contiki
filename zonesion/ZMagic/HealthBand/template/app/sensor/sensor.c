/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�
* ˵����ͨ�ô��������ƽӿڳ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
#include "sensor.h"
#include "Relay.h"
#include "BatteryVoltage.h"
#include "lte_zhiyun.h"
#include "SysCalendar.h"
#include "fml_imu/lis3dh.h"
#include "fml_InfraredTemp/mlx90615.h"	
#include "apl_HeartRate/apl_HeartRate.h"
#include "AlarmClockDLG.h"
#include "bloodpressure.h"

unsigned char D0 = 0xff;                                        // �����ϱ�ʹ��
unsigned char D1 = 0;                                           // Ĭ�Ϲرտ����ഫ����
unsigned int V0 = 30;                                           // �����ϱ�ʱ��������λ��,0�������ϱ�
float A0 = 0;                                                   // ����
int32_t A1 = 0;                                                 // Ѫ��
int32_t A2 = 0;                                                 // ����
int16_t A3 = 0;                                                 // Ѫѹ����ѹ
int16_t A4 = 0;                                                 // Ѫѹ����ѹ
int A5 = 0;                                                     // ����״̬
int A6 = 0;                                                     // ����
float A7=0;                                                     // ��ѹ

//��������ֵ
float Threshold_TempMin = 20;
float Threshold_TempMax = 40;
int32_t Threshold_HeartMin = 50;
int32_t Threshold_HeartMax = 150;
int32_t Threshold_Spo2Min = 80;
int32_t Threshold_Spo2Max = 100;
/*********************************************************************************************
* ���ƣ�sensor_type()
* ���ܣ�
* ������
* ���أ����ش��������ͣ�3�ֽ��ַ�����ʾ
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
char *sensor_type(void)
{
  return SENSOR_TYPE;                                           //���ش���������
}

/*********************************************************************************************
* ���ƣ�updateA0
* ���ܣ�����A0��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA0(void)
{
  mlx90615_GetTemp(&A0);
}

/*********************************************************************************************
* ���ƣ�updateA1
* ���ܣ�����A1��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA1(void)
{
  Spo2_Get(&A1);
}

/*********************************************************************************************
* ���ƣ�updateA2
* ���ܣ�����A2��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA2(void)
{
  HeartRate_Get(&A2);
}

/*********************************************************************************************
* ���ƣ�updateA3
* ���ܣ�����A3��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA3(void)
{
  A3 = SBP_Get();
}

/*********************************************************************************************
* ���ƣ�updateA4
* ���ܣ�����A4��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA4(void)
{
  A4 = DBP_Get();
}

/*********************************************************************************************
* ���ƣ�updateA5
* ���ܣ�����A5��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA5(void)
{
  static clock_time_t time = 0;
  
  if(time == 0)
  {
    if(get_lis3dh_tumbleStatus() == 1)
    {
      A5 = 1;
      time = clock_time();
    }
    else
      A5 = 0;
  }
  else if((clock_time() - time) > 3000)
    time = 0;
}

/*********************************************************************************************
* ���ƣ�updateA6
* ���ܣ�����A6��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA6(void)
{
  A6 = get_lis3dh_stepCount();
}

/*********************************************************************************************
* ���ƣ�updateA7
* ���ܣ�����A7��ֵ
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateA7(void)
{
  A7 = BatteryVotage_Get();                                   // ����A7 ��ص�ѹ
}

/*********************************************************************************************
* ���ƣ�sensor_init()
* ���ܣ�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensor_init(void)
{
  relay_init();                                                 //��ʼ�������ഫ�������̵�����
  blood_init();
}


/*********************************************************************************************
* ���ƣ�sensor_control()
* ���ܣ�����������
* ������cmd - ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensor_control(unsigned char cmd)
{
  
  if(cmd & 0x40)
  {
    relay_on(1);
  }
  else
  {
    relay_off(1);
  }
  if(cmd & 0x80)
  {
    relay_on(2);
  }
  else
  {
    relay_off(2);
  }
}

/*********************************************************************************************
* ���ƣ�sensorLinkOn
* ���ܣ�
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorLinkOn()
{
  char buf[64]={0};
  updateA0();
  updateA1();
  updateA2();
  updateA3();
  updateA4();
  updateA5();
  updateA6();
  updateA7();
  
  zxbeeBegin();
  if (D0 & 0x01)
  {
    sprintf(buf, "%.1f", A0);
    zxbeeAdd("A0", buf);
  }
  if (D0 & 0x02)
  {
    sprintf(buf, "%d", A1);
    zxbeeAdd("A1", buf);
  }
  if (D0 & 0x04)
  {
    sprintf(buf, "%d", A2);
    zxbeeAdd("A2", buf);
  }
  if (D0 & 0x08)
  {
    sprintf(buf, "%d", A3);
    zxbeeAdd("A3", buf);
  }
  if (D0 & 0x10)
  {
    sprintf(buf, "%d", A4);
    zxbeeAdd("A4", buf);
  }
  if (D0 & 0x20)
  {
    sprintf(buf, "%d", A5);
    zxbeeAdd("A5", buf);
  }
  if (D0 & 0x40)
  {
    sprintf(buf, "%d", A6);
    zxbeeAdd("A6", buf);
  }
  if (D0 & 0x80)
  {
    sprintf(buf, "%.1f", A7);
    zxbeeAdd("A7", buf);
  }
  char *p = zxbeeEnd();
  if (p != NULL)
  {
    RFSendData(p);                                              //��������
  }
}

/*********************************************************************************************
* ���ƣ�sensor_poll()
* ���ܣ���ѯ���������������ϱ�����������
* ������t: ���ô���
* ���أ�
* �޸ģ�
* ע�ͣ��˺���ÿ���ӵ���1�Σ�tΪ���ô���
*********************************************************************************************/
void sensor_poll(unsigned int t)
{
  char buf[64]={0};
  updateA0();
  updateA1();
  updateA2();
  updateA3();
  updateA4();
  updateA5();
  updateA6();
  updateA7();
  
  if (V0 != 0)
  {
    if (t % V0 == 0)
    {
      zxbeeBegin();
      if (D0 & 0x01)
      {
        sprintf(buf, "%.1f", A0);
        zxbeeAdd("A0", buf);
      }
      if (D0 & 0x02)
      {
        sprintf(buf, "%d", A1);
        zxbeeAdd("A1", buf);
      }
      if (D0 & 0x04)
      {
        sprintf(buf, "%d", A2);
        zxbeeAdd("A2", buf);
      }
      if (D0 & 0x08)
      {
        sprintf(buf, "%d", A3);
        zxbeeAdd("A3", buf);
      }
      if (D0 & 0x10)
      {
        sprintf(buf, "%d", A4);
        zxbeeAdd("A4", buf);
      }
      if (D0 & 0x20)
      {
        sprintf(buf, "%d", A5);
        zxbeeAdd("A5", buf);
      }
      if (D0 & 0x40)
      {
        sprintf(buf, "%d", A6);
        zxbeeAdd("A6", buf);
      }
      if (D0 & 0x80)
      {
        sprintf(buf, "%.1f", A7);
        zxbeeAdd("A7", buf);
      }
      char *p = zxbeeEnd();
      if (p != NULL)
      {
        RFSendData(p);                                          //��������
      }
    }
  }
}

/*********************************************************************************************
* ���ƣ�sensor_check()
* ���ܣ������Լ�麯�������趨��ѯʱ��
* ��������
* ���أ�������ѯ��ʱ�䣬��λms,��Χ:1-65535
* �޸ģ�
* ע�ͣ��˺���������Ҫ����������Ӧ�Ĵ�����
*********************************************************************************************/
unsigned short sensor_check()
{
  static uint8_t LastA5 = 0;
  char buf[32] = {0};
  
  updateA5();
  if((A5 != LastA5))
  {
    LastA5 = A5;
    
    zxbeeBegin();
    if (D0 & 0x20)
    {
      sprintf(buf, "%d", A5);
      zxbeeAdd("A5", buf);
    }
    char *p = zxbeeEnd();
    if (p != NULL)
    {
      RFSendData(p);                                            //��������
    }
  }
  return 100;                                                   //����ֵ�����´ε���ʱ�䣬�˴�Ϊ100ms
}

/*********************************************************************************************
* ���ƣ�z_process_command_call()
* ���ܣ������ϲ�Ӧ�÷�������ָ��
* ������ptag: ָ���ʶ D0��D1�� A0 ...
*       pval: ָ��ֵ�� ��������ʾ��ȡ��
*       obuf: ָ��������ŵ�ַ
* ���أ�>0ָ�������������ݳ��ȣ�0��û�з������ݣ�<0����֧��ָ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int z_process_command_call(char* ptag, char* pval, char* obuf)
{
  int ret = -1;
  
  if (memcmp(ptag, "D0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "D0=%d", D0);
    }
  }
  if (memcmp(ptag, "CD0", 3) == 0)
  {
    int v = atoi(pval);
    if (v > 0)
    {
      D0 &= ~v;
    }
  }
  if (memcmp(ptag, "OD0", 3) == 0)
  {
    int v = atoi(pval);
    if (v > 0)
    {
      D0 |= v;
    }
  }
  if (memcmp(ptag, "D1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "D1=%d", D1);
    }
  }
  if (memcmp(ptag, "CD1", 3) == 0)  				            //����⵽CD1ָ��
  {
    int v = atoi(pval);                                     //��ȡCD1����
    D1 &= ~v;                                               //����D1����
    sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
  }
  if (memcmp(ptag, "OD1", 3) == 0)  				            //����⵽OD1ָ��
  {
    int v = atoi(pval);                                     //��ȡOD1����
    D1 |= v;                                                //����D1����
    sensor_control(D1);                                     //���µ�ŷ�ֱ�����״̬
  }
  if (memcmp(ptag, "V0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "V0=%d", V0);
    }
    else
    {
      V0 = atoi(pval);
    }
  }
  if (memcmp(ptag, "V1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V1[128] = {0};
      Calender_ZxbeeGet(V1);
      ret = sprintf(obuf, "V1=%s", V1);
    }
    else
    {
      Calender_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V2", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V2[128] = {0};
      Alarm_ZxbeeGet(V2);
      ret = sprintf(obuf, "V2=%s", V2);
    }
    else
    {
      Alarm_ZxbeeSet(pval);
    }
  }
  if (memcmp(ptag, "V3", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V3[64] = {0};
      sprintf(V3,"%.2f&%.2f",Threshold_TempMin,Threshold_TempMax);
      ret = sprintf(obuf, "V3=%s", V3);
    }
    else
    {
      char* ptr = pval;
      Threshold_TempMin = atof(ptr);
      ptr = strstr(ptr,"&");
      Threshold_TempMax = atof(ptr++);
    }
  }
  if (memcmp(ptag, "V4", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V4[64] = {0};
      sprintf(V4,"%d&%d",Threshold_HeartMin,Threshold_HeartMax);
      ret = sprintf(obuf, "V4=%s", V4);
    }
    else
    {
      char* ptr = pval;
      Threshold_HeartMin = atoi(ptr);
      ptr = strstr(ptr,"&");
      Threshold_HeartMax = atoi(ptr++);
    }
  }
  if (memcmp(ptag, "V5", 2) == 0)
  {
    if (pval[0] == '?')
    {
      char V5[64] = {0};
      sprintf(V5,"%d&%d",Threshold_Spo2Min,Threshold_Spo2Max);
      ret = sprintf(obuf, "V5=%s", V5);
    }
    else
    {
      char* ptr = pval;
      Threshold_Spo2Min = atoi(ptr);
      ptr = strstr(ptr,"&");
      Threshold_Spo2Max = atoi(ptr++);
    }
  }
  if (memcmp(ptag, "A0", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A0=%.1f", A0);
    }
  }
  if (memcmp(ptag, "A1", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A1=%d", A1);
    }
  }
  if (memcmp(ptag, "A2", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A2=%d", A2);
    }
  }
  if (memcmp(ptag, "A3", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A3=%d", A3);
    }
  }
  if (memcmp(ptag, "A4", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A4=%d", A4);
    }
  }
  if (memcmp(ptag, "A5", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A5=%d", A5);
    }
  }
  if (memcmp(ptag, "A6", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A6=%d", A6);
    }
  }
  if (memcmp(ptag, "A7", 2) == 0)
  {
    if (pval[0] == '?')
    {
      ret = sprintf(obuf, "A7=%.1f", A7);
    }
  }
  return ret;
}
