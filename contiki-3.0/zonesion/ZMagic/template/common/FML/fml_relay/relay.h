/*********************************************************************************************
* 文件：Relay.h
* 作者：
* 说明：继电器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __RELAY_H__
#define __RELAY_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "stm32f4xx.h"

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void relay_init(void);                                          //继电器传感器初始化
unsigned char relay_GetStatus(void);
void relay_control(unsigned char cmd);                        //继电器传感器控制
void relay_on(unsigned int s);
void relay_off(unsigned int s);

#endif /*__INFRARED_H__*/

