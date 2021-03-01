#include "led.h"
#include "delay.h"
#include "usart.h"

/***************************************************************
*
*功能：发光二极管初始化	 
*参数：无
*
*****************************************************************/ 
void LED_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
}

void USB_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, USB_CTRL_Pin, GPIO_PIN_RESET);
  
  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();    
  
  /*Configure GPIO pins : LED_Pin USB_CTRL_Pin */
  GPIO_InitStruct.Pin = USB_CTRL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}


/***************************************************************
*
*功能：灯光报警
*参数：nCount对应的时间ms
*
*****************************************************************/
void Light_Alarm(u16 nCount)
{

     LED(!LED_R);
     delay_ms(nCount);
}
