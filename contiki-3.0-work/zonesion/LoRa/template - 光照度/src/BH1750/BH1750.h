/*********************************************************************************************
* 文件：BH1750.h
* 作者：zonesion
* 说明：BH1750头文件
* 修改：Chenkm 2017.01.10 修改代码格式，增加代码注释和文件说明
* 注释：
*********************************************************************************************/
#ifndef __BH1750_H__
#define __BH1750_H__
#include "stm32f10x.h"
#include "iic.h"
#include "delay.h"

/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define uint                    unsigned int 
#define uchar                   unsigned char
#define DPOWR                   0X00                            //断电
#define POWER                   0X01                            //上电
#define RESET                   0X07                            //重置
#define CHMODE                  0X10                            //连续H分辨率
#define CHMODE2                 0X11                            //连续H分辨率2
#define CLMODE                  0X13                            //连续低分辨
#define H1MODE                  0X20                            //一次H分辨率
#define H1MODE2                 0X21                            //一次H分辨率2
#define L1MODE                  0X23                            //一次L分辨率模式
#define	SlaveAddress            0x46                            //定义器件在IIC总线中的从地址,根据ALT  ADDRESS地址引脚不同修改
//ALT  ADDRESS引脚接地时地址为0xA6，接电源时地址为0x3A

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
uchar bh1750_send_byte(uchar sla,uchar c);
uchar bh1750_read_nbyte(uchar sla,uchar *s,uchar no);
void bh1750_init(void);                                         //初始
float bh1750_get_data(void);

#endif //__BH1750_H__