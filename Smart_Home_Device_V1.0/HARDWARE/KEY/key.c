#include "key.h"
#include "delay.h"
#include "led.h"
#include "usart.h"
#include "wifi.h"
#include "main.h"
#include "ri.h"
#include "wkup.h"



/***************************************************************
*
*功能：按键初始化	 
*参数：无
*
*****************************************************************/ 
void KEY_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  
  /*Configure GPIO pin : WK_UP_Pin */
  GPIO_InitStruct.Pin = WK_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(WK_UP_GPIO_Port, &GPIO_InitStruct);

  WKUP_Init();                   //初始化待机唤醒
  
  /*Configure GPIO pins : KEY0_Pin KEY1_Pin */
  GPIO_InitStruct.Pin = KEY0_Pin|KEY1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  
  
  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_1_IRQn);

  HAL_NVIC_SetPriority(EXTI4_15_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI4_15_IRQn);
  

}


/***************************************************************
*
*功能：中断服务函数	 
*参数：无
*
*****************************************************************/ 
void EXTI0_1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(WK_UP_Pin);		//调用中断处理公用函数
}

void EXTI4_15_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(KEY0_Pin);		//调用中断处理公用函数
    HAL_GPIO_EXTI_IRQHandler(KEY1_Pin);
}



/***************************************************************
*
*功能：中断回调函数（重写）	 
*参数：由中断服务程序中调用，在HAL库中所有的外部中断服务函数都会调用此函数
*
*****************************************************************/ 
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    u8 times = 0;
    delay_ms(100);      //消抖
    switch(GPIO_Pin)
    {
        case GPIO_PIN_0:
            if(WK_UP_R==1) //待机
            {
               if(Check_WKUP())//关机
               {
                 Sys_Enter_Standby();//进入待机模式
               }
                

            }
            break;
        case GPIO_PIN_4:
            if(KEY0_R==0)  //松开按键后重置wifi
            {
                while(!KEY0_R);
                mcu_reset_wifi();
                
            }
            break;
        case GPIO_PIN_5:
            if(KEY1_R==0)  //按键配置WIFI接入模式
            {
                while(!KEY1_R)    //按下LED0开始闪烁
                {
                    delay_ms(500);
                    times++;
                }
                if(times < 3)   mcu_set_wifi_mode(0);   //闪烁3次以下 Smart模式
                else            mcu_set_wifi_mode(1);   //闪烁3次以上 AP模式
            }
            break;
    }
}

/***************************************************************
*
*功能：按键扫描
*参数：模式 0—不支持连按 1—支持连按
*
*****************************************************************/
//uint8_t Key_Scan(uint8_t mode)
//{
//    static uint8_t flag = 1;
//    __IO uint8_t IO_WK_UP = HAL_GPIO_ReadPin(WK_UP_GPIO_Port,WK_UP_Pin);
//    __IO uint8_t IO_KEY0 = HAL_GPIO_ReadPin(KEY0_GPIO_Port,KEY0_Pin);
//    
//    if(mode == 1) flag = 1;
//    if(flag == 1 && ((IO_WK_UP == GPIO_PIN_SET)||(IO_KEY0 == GPIO_PIN_RESET)))
//    {   
//        HAL_DelayMs(10);//消抖
//        flag = 0;
//        if(IO_WK_UP == GPIO_PIN_SET)   return WK_UP;
//        else if(IO_KEY0 == GPIO_PIN_RESET) return KEY0;
//    }else if((IO_WK_UP == GPIO_PIN_RESET)&&(IO_KEY0 == GPIO_PIN_SET)) flag = 1;
//   
//    return 0xff;
//}