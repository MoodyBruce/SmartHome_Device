#ifndef _KEY_H
#define _KEY_H
#include "sys.h"

#define WK_UP_Pin GPIO_PIN_0
#define WK_UP_GPIO_Port GPIOA
#define WK_UP_EXTI_IRQn EXTI0_1_IRQn
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOA
#define KEY0_EXTI_IRQn EXTI4_15_IRQn
#define KEY1_Pin GPIO_PIN_5
#define KEY1_GPIO_Port GPIOA
#define KEY1_EXTI_IRQn EXTI4_15_IRQn

#define WK_UP_R  HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin)?1:0
#define KEY0_R  HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin)?1:0
#define KEY1_R  HAL_GPIO_ReadPin(KEY1_GPIO_Port,KEY1_Pin)?1:0

void KEY_Init(void);
#endif
