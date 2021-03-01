#ifndef _LED_H
#define _LED_H
#include "sys.h"

#define LED_Pin GPIO_PIN_6
#define LED_GPIO_Port GPIOA

#define USB_CTRL_Pin GPIO_PIN_7
#define USB_CTRL_GPIO_Port GPIOA

#define USB_ON()      HAL_GPIO_WritePin(USB_CTRL_GPIO_Port, USB_CTRL_Pin, 1)
#define USB_OFF()     HAL_GPIO_WritePin(USB_CTRL_GPIO_Port, USB_CTRL_Pin, 0)  

#define LED(x) HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, x)
#define LED_R  HAL_GPIO_ReadPin(LED_GPIO_Port,LED_Pin)?1:0


void LED_Init(void);
void USB_Init(void);
void Light_Alarm(u16 nCount);
#endif
