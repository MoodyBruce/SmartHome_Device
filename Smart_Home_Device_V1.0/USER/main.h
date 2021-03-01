/**
  ******************************************************************************
  * @file    Templates/Inc/main.h 
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "sys.h"

extern u8 temp_max,temp_min,light_min;
extern u16 temp,humi;
extern u8 light;
extern u8 sensor_flag;
extern u8 device_state;

typedef enum
{
  TEMP_LOWER_ALARM   = 0x00,
  TEMP_UPPER_ALARM   = 0x01,
  TEMP_ALARM_CANCEL       = 0x02
} temp_enum;

typedef enum
{
  LIGHT_LOWER_ALARM  = 0x00,
  LIGHT_ALARM_CANCEL = 0x01
} light_enum;

extern temp_enum temp_alarm;
extern light_enum light_alarm;

void Show_TempAndHumi(u8 x1,u8 y1,u8 x2,u8 y2,u8 size);
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/ 

