/*********************************************************************************************
* 文件：sensor.c
* 作者：
* 说明：通用传感器控制接口程序
* 修改：
* 注释：
*********************************************************************************************/
#include "sensor.h"

unsigned char D0 = 0xe1;                                        // 主动上报使能，默认只允许A0主动上报
unsigned char D1 = 0;                                           // 默认关闭控制类传感器
unsigned int V0 = 30;                                           // 主动上报时间间隔，单位秒,0不主动上报
unsigned char A0=0;                                             // 接近开关
float A5 =0, A6=0, A7=0;                                        // 板载温度、湿度、电压


/*********************************************************************************************
* 名称：proximitySwitchInit
* 功能：接近开关初始化
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void  proximitySwitchInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;                         //初始化结构体

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);         //初始化时钟
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;                     //选择端口
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;                  //输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;                //设置为开漏模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;              //速度配置
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                  //配置为上拉
    GPIO_Init(GPIOC, &GPIO_InitStructure);                        //初始化结构体配置
}

/*********************************************************************************************
* 名称：sensor_init()
* 功能：
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void sensor_init(void)
{
    relay_init();                                                 //初始化控制类传感器（继电器）
    proximitySwitchInit();
}

/*********************************************************************************************
* 名称：sensor_type()
* 功能：
* 参数：
* 返回：返回传感器类型，3字节字符串表示
* 修改：
* 注释：
*********************************************************************************************/
char *sensor_type(void)
{
    return SENSOR_TYPE;                                           //返回传感器类型
}

/*********************************************************************************************
* 名称：sensor_control()
* 功能：传感器控制
* 参数：cmd - 控制命令
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensor_control(unsigned char cmd)
{
    if(cmd & 0x01)
    {
        relay_on(1);
    }
    else
    {
        relay_off(1);
    }
    if(cmd & 0x02)
    {
        relay_on(2);
    }
    else
    {
        relay_off(2);
    }
}

/*********************************************************************************************
* 名称：updateA0
* 功能：更新A0的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA0(void)
{
    A0 = !GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4);
}

/*********************************************************************************************
* 名称：updateA5
* 功能：更新A5的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA5(void)
{
    A5 = Htu21dTemperature_Get();                               // 更新A5 板载温度
}

/*********************************************************************************************
* 名称：updateA6
* 功能：更新A6的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA6(void)
{
    A6 = Htu21dHumidity_Get();                                  // 更新A6 板载湿度
}

/*********************************************************************************************
* 名称：updateA7
* 功能：更新A7的值
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void updateA7(void)
{
    A7 = BatteryVotage_Get();                                   // 更新A7 电池电压
}

/*********************************************************************************************
* 名称：sensor_poll()
* 功能：轮询传感器，并主动上报传感器数据
* 参数：t: 调用次数
* 返回：
* 修改：
* 注释：此函数每秒钟调用1次，t为调用次数
*********************************************************************************************/
void sensor_poll(unsigned int t)
{
    char buf[16]={0};
    
    updateA0();
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
                sprintf(buf, "%d", A0);
                zxbeeAdd("A0", buf);
            }
            if (D0 & 0x02)
            {
               //A1值打包
            }
            if (D0 & 0x04)
            {
               //A2值打包
            }
            if (D0 & 0x08)
            {
               //A3值打包
            }
            if (D0 & 0x10)
            {
               //A4值打包
            }
            if (D0 & 0x20)
            {
                sprintf(buf, "%.1f", A5);
                zxbeeAdd("A5", buf);
            }
            if (D0 & 0x40)
            {
                sprintf(buf, "%.1f", A6);
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
                rfUartSendData(p);                              //发送无线数据
            }
        }
    }
}

/*********************************************************************************************
* 名称：sensor_check()
* 功能：周期性检查函数，可设定轮询时间
* 参数：无
* 返回：设置轮询的时间，单位ms,范围:1-65535
* 修改：
* 注释：此函数用于需要快速做出相应的传感器
*********************************************************************************************/
unsigned short sensor_check()
{
    char buf[16]={0};
    static uint8_t last_A0 = 0;
    static uint8_t tick = 0;
    
    updateA0();
    if(A0 != last_A0)                                               //如果此次状态与上次检测状态不一致
    {
        last_A0 = A0;
        tick = 0;
        
        if (D0 & 0x01)
        {
            zxbeeBegin();                                           //装填ZXBee协议格式
            sprintf(buf, "%u", A0);                                 //将A0的值以%d的格式赋予A0
            zxbeeAdd("A0", buf);                                    //将数据写入发送队列
            rfUartSendData(zxbeeEnd());                             //发送数据
        }
    }
    if(A0 == 1)
    {
        tick++;
        if(tick>29)
        {
            tick = 0;
            if (D0 & 0x01)
            {
                zxbeeBegin();                                           //装填ZXBee协议格式
                sprintf(buf, "%u", A0);                                 //将A0的值以%d的格式赋予A0
                zxbeeAdd("A0", buf);                                    //将数据写入发送队列
                rfUartSendData(zxbeeEnd());                             //发送数据
            }
        }
    }
        
    return 100;                                                 //返回值决定下次调用时间，此处为100ms
}

/*********************************************************************************************
* 名称：z_process_command_call()
* 功能：处理上层应用发过来的指令
* 参数：ptag: 指令标识 D0，D1， A0 ...
*       pval: 指令值， “？”表示读取，
*       obuf: 指令处理结果存放地址
* 返回：>0指令处理结果返回数据长度，0：没有返回数据，<0：不支持指令。
* 修改：
* 注释：
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
    if (memcmp(ptag, "CD1", 3) == 0)  				            //若检测到CD1指令
    {
        int v = atoi(pval);                                     //获取CD1数据
        D1 &= ~v;                                               //更新D1数据
        sensor_control(D1);                                     //更新电磁阀直流电机状态
    }
    if (memcmp(ptag, "OD1", 3) == 0)  				            //若检测到OD1指令
    {
        int v = atoi(pval);                                     //获取OD1数据
        D1 |= v;                                                //更新D1数据
        sensor_control(D1);                                     //更新电磁阀直流电机状态
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
    if (memcmp(ptag, "A0", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A0=%d", A0);
        }
    }
    if (memcmp(ptag, "A1", 2) == 0)
    {
        if (pval[0] == '?')
        {
            //
        }
    }
    if (memcmp(ptag, "A2", 2) == 0)
    {
        if (pval[0] == '?')
        {
            //
        }
    }
    if (memcmp(ptag, "A3", 2) == 0)
    {
        if (pval[0] == '?')
        {
           //
        }
    }
    if (memcmp(ptag, "A4", 2) == 0)
    {
        if (pval[0] == '?')
        {
            //
        }
    }
    if (memcmp(ptag, "A5", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A5=%.1f", A5);
        }
    }
    if (memcmp(ptag, "A6", 2) == 0)
    {
        if (pval[0] == '?')
        {
            ret = sprintf(obuf, "A6=%.1f", A6);
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

