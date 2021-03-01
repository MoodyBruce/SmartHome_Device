#ifndef __I2C_H__
#define __I2C_H__
#include "sys.h"

#define SHT30_SCL_PIN	GPIO_PIN_8
#define SHT30_SDA_PIN	GPIO_PIN_9
#define SHT30_IIC_GPIO 	GPIOB

#define IIC_SHT30_SCL(x)	HAL_GPIO_WritePin(SHT30_IIC_GPIO, SHT30_SCL_PIN, x)
#define IIC_SHT30_SDA(x)	HAL_GPIO_WritePin(SHT30_IIC_GPIO, SHT30_SDA_PIN, x)
#define READ_SDA_SHT30	HAL_GPIO_ReadPin(SHT30_IIC_GPIO,SHT30_SDA_PIN)?1:0
#define READ_SCL_SHT30	HAL_GPIO_ReadPin(SHT30_IIC_GPIO,SHT30_SCL_PIN)?1:0

#define ACK		0
#define NACK 	1

void IIC_Sht30_Init(void);
void IIC_Sht30_Start(void);
void IIC_Sht30_Stop(void);
u8 IIC_Sht30_Write_Byte(u8 txd);
u8 IIC_Sht30_Read_Byte(u8 ack);

#endif
