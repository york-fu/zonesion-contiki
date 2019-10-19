/*********************************************************************************************
* 文件：FAN.h
* 作者：Lixm 2017.10.17
* 说明：风扇驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __FAN_H__
#define __FAN_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "stm32f4xx.h"

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void fan_init(void);                                            //风扇传感器初始化
void fan_control(unsigned int cmd);                            //风扇传感器控制

#endif /*__FAN_H__*/

