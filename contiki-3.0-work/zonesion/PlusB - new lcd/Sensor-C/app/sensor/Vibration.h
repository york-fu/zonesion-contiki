/*********************************************************************************************
* 文件：Infrared.h
* 作者：Lixm 2017.10.17
* 说明：震动传感器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __VIBRATION_H__
#define __VIBRATION_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "stm32f4xx.h"

#define SHAKE_RCC   RCC_AHB1Periph_GPIOB
#define SHAKE_GPIO  GPIOB
#define SHAKE_PIN   GPIO_Pin_9

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void vibration_init(void);                                        //人体红外传感器初始化
unsigned char get_vibration_status(void);                        //获取人体红外传感器状态

#endif /*__VIBRATION_H__*/

