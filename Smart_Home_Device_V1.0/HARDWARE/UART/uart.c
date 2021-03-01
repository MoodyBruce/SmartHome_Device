#include "uart.h"
#include "usart.h"
#include "wifi.h"

UART_HandleTypeDef huart1;

/***************************************************************
*
*功能：涂鸦模组通信串口初始化	 
*参数：波特率
*
*****************************************************************/ 
void Tuya_Uart_Init(u32 bound)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = bound;  
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
   printf("uart2 error\r\n");       //初始化失败，打印错误
  }

  __HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);       //使能串口接收中断

}

/***************************************************************
*
*功能：串口1中断服务函数	 
*参数：波特率
*
*****************************************************************/ 
void USART1_IRQHandler(void)
{
  unsigned char Res = 0;
   if((USART1->ISR&UART_FLAG_RXNE)!=0)
   {
    Res = USART1->RDR;
    uart_receive_input(Res);
   }
  HAL_UART_IRQHandler(&huart1);
}

/***************************************************************
*
*功能：串口1单字节发送函数	 
*参数：data数据
*
*****************************************************************/ 
void UART1_SendByte(u8 data)
{
    while((USART1->ISR &UART_FLAG_TXE)!=UART_FLAG_TXE);
    USART1->TDR = data;
}

