#ifndef _fml_stepmotor_h_
#define _fml_stepmotor_h_

#include "stm32f4xx.h"
#include "mpl_tim/bsp_tim.h"

#define STEPMOTOR_ENABLE    0
#define STEPMOTOR_DISABLE   !STEPMOTOR_ENABLE

//stepmotor 1
#define STEPMOTOR1_ENABLE_RCC      RCC_AHB1Periph_GPIOE
#define STEPMOTOR1_ENABLE_GPIO     GPIOE
#define STEPMOTOR1_ENABLE_PIN      GPIO_Pin_5

#define STEPMOTOR1_DIR_RCC         RCC_AHB1Periph_GPIOD
#define STEPMOTOR1_DIR_GPIO        GPIOD
#define STEPMOTOR1_DIR_PIN         GPIO_Pin_13

#define STEPMOTOR1_STEP_RCC        RCC_AHB1Periph_GPIOB
#define STEPMOTOR1_STEP_GPIO       GPIOB
#define STEPMOTOR1_STEP_PIN        GPIO_Pin_1

#define STEPMOTOR1_ENABLE_L       (STEPMOTOR1_ENABLE_GPIO->BSRRH|=STEPMOTOR1_ENABLE_PIN)
#define STEPMOTOR1_ENABLE_H       (STEPMOTOR1_ENABLE_GPIO->BSRRL|=STEPMOTOR1_ENABLE_PIN)

#define STEPMOTOR1_DIR_L          (STEPMOTOR1_DIR_GPIO->BSRRH|=STEPMOTOR1_DIR_PIN)
#define STEPMOTOR1_DIR_H          (STEPMOTOR1_DIR_GPIO->BSRRL|=STEPMOTOR1_DIR_PIN)

#define STEPMOTOR1_STEP_L         (STEPMOTOR1_STEP_GPIO->BSRRH|=STEPMOTOR1_STEP_PIN)
#define STEPMOTOR1_STEP_H         (STEPMOTOR1_STEP_GPIO->BSRRL|=STEPMOTOR1_STEP_PIN)

//stepmotor 2
#define STEPMOTOR2_ENABLE_RCC      RCC_AHB1Periph_GPIOB
#define STEPMOTOR2_ENABLE_GPIO     GPIOB
#define STEPMOTOR2_ENABLE_PIN      GPIO_Pin_0

#define STEPMOTOR2_DIR_RCC         RCC_AHB1Periph_GPIOC
#define STEPMOTOR2_DIR_GPIO        GPIOC
#define STEPMOTOR2_DIR_PIN         GPIO_Pin_5

#define STEPMOTOR2_STEP_RCC        RCC_AHB1Periph_GPIOA
#define STEPMOTOR2_STEP_GPIO       GPIOA
#define STEPMOTOR2_STEP_PIN        GPIO_Pin_7

#define STEPMOTOR2_ENABLE_L       (STEPMOTOR2_ENABLE_GPIO->BSRRH|=STEPMOTOR2_ENABLE_PIN)
#define STEPMOTOR2_ENABLE_H       (STEPMOTOR2_ENABLE_GPIO->BSRRL|=STEPMOTOR2_ENABLE_PIN)

#define STEPMOTOR2_DIR_L          (STEPMOTOR2_DIR_GPIO->BSRRH|=STEPMOTOR2_DIR_PIN)
#define STEPMOTOR2_DIR_H          (STEPMOTOR2_DIR_GPIO->BSRRL|=STEPMOTOR2_DIR_PIN)

#define STEPMOTOR2_STEP_L         (STEPMOTOR2_STEP_GPIO->BSRRH|=STEPMOTOR2_STEP_PIN)
#define STEPMOTOR2_STEP_H         (STEPMOTOR2_STEP_GPIO->BSRRL|=STEPMOTOR2_STEP_PIN)

//stepmotor 3
#define STEPMOTOR3_ENABLE_RCC      RCC_AHB1Periph_GPIOB
#define STEPMOTOR3_ENABLE_GPIO     GPIOB
#define STEPMOTOR3_ENABLE_PIN      GPIO_Pin_6

#define STEPMOTOR3_DIR_RCC         RCC_AHB1Periph_GPIOD
#define STEPMOTOR3_DIR_GPIO        GPIOD
#define STEPMOTOR3_DIR_PIN         GPIO_Pin_6

#define STEPMOTOR3_STEP_RCC        RCC_AHB1Periph_GPIOE
#define STEPMOTOR3_STEP_GPIO       GPIOE
#define STEPMOTOR3_STEP_PIN        GPIO_Pin_6

#define STEPMOTOR3_ENABLE_L       (STEPMOTOR3_ENABLE_GPIO->BSRRH|=STEPMOTOR3_ENABLE_PIN)
#define STEPMOTOR3_ENABLE_H       (STEPMOTOR3_ENABLE_GPIO->BSRRL|=STEPMOTOR3_ENABLE_PIN)

#define STEPMOTOR3_DIR_L          (STEPMOTOR3_DIR_GPIO->BSRRH|=STEPMOTOR3_DIR_PIN)
#define STEPMOTOR3_DIR_H          (STEPMOTOR3_DIR_GPIO->BSRRL|=STEPMOTOR3_DIR_PIN)

#define STEPMOTOR3_STEP_L         (STEPMOTOR3_STEP_GPIO->BSRRH|=STEPMOTOR3_STEP_PIN)
#define STEPMOTOR3_STEP_H         (STEPMOTOR3_STEP_GPIO->BSRRL|=STEPMOTOR3_STEP_PIN)


typedef struct
{
  uint16_t step;
  uint16_t speed;
}Stepmotor_t;


void StepmotorIrq(void);

void StepmotorInit();
short Stepmotor_Run(uint8_t index,uint8_t dir,uint16_t step);
void Stepmotor_Stop(uint8_t index);
short Stepmotor_GetRunStatus(uint8_t index);

#endif