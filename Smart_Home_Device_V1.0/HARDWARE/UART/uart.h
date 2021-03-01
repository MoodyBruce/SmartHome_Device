#ifndef _UART_H
#define _UART_H
#include "sys.h"

void Tuya_Uart_Init(u32 bound);
void UART1_SendByte(u8 data);
#endif
