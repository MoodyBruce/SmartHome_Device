#ifndef _RI_H
#define _RI_H
#include "sys.h"

#define RI_TXD_Pin GPIO_PIN_12
#define RI_TXD_GPIO_Port GPIOB

#define IR_TXD_Pin GPIO_PIN_13
#define IR_TXD_GPIO_Port GPIOB

#define RI_TXD(x) {HAL_GPIO_WritePin(RI_TXD_GPIO_Port, RI_TXD_Pin, x);\
                   HAL_GPIO_WritePin(IR_TXD_GPIO_Port, IR_TXD_Pin, x); }

void RI_Init(void);
void Transmition(uint8_t *s,uint8_t n);
void Device_ON(void);                   
void Device_OFF(void);
                   
#define CMD_SIZE 6                   
                   
extern u8 CMD_OPEN[CMD_SIZE];
extern u8 CMD_CLOSE[CMD_SIZE];                   
#endif
