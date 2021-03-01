#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "uart.h"
#include "wifi.h"
#include "sht30.h"
#include "oled.h"
#include "timer.h"
#include "text.h"
#include "main.h"
#include "i2c.h"
#include "ri.h"
#include "adc.h"
#include "wkup.h"

temp_enum temp_alarm = TEMP_ALARM_CANCEL;
light_enum light_alarm = LIGHT_ALARM_CANCEL;


u8 light = 0;
u16 temp = 0,humi = 0;
u8 temp_max = 60,temp_min = 0,light_min = 0;
u8 sensor_flag = 0;
u8 device_state = 0x00;    //0x00 待机 0x01 关闭 0x02 开启

void Show_TempAndHumi(u8 x1,u8 y1,u8 x2,u8 y2,u8 size)
{
    u8 temp_H,temp_L,humi_H,humi_L;
    temp_H = temp/10; temp_L = temp%10;
    humi_H = humi/10; humi_L = humi%10;
    OLED_ShowNum(x1-16,y1,temp_H,2,size);
    OLED_ShowChar(x1,y1,'.',size,1);
    OLED_ShowNum(x1+8,y1,temp_L,1,size);
    OLED_ShowString(x1+8+(size),y1,"%C",size);
    
    OLED_ShowNum(x2-16,y2,humi_H,2,size);
    OLED_ShowChar(x2,y2,'.',size,1);
    OLED_ShowNum(x2+8,y2,humi_L,1,size);
    OLED_ShowString(x2+8+(size),y2,"RH%",size);
    
   
    
    OLED_Refresh_Gram();
}

/***************************************************************
*
*功能：main函数	 
*参数：无
*
*****************************************************************/ 
int main(void)
{
    uint8_t times = 0;
    float t = 0.0,h = 0.0;
    HAL_Init();                    //初始化HAL库    
    Stm32_Clock_Init();   		   //初始化时钟
    delay_init(48);                //初始化延时函数
    uart_init(115200);             //初始化串口通信
    
    
    LED_Init();                    //初始化LED	
    KEY_Init();                    //初始化按键
    USB_Init();                    //初始化USB继电器
    RI_Init();                     //初始化红外控制
    TIM3_Init();                   //初始化定时器3
    OLED_Init();                   //初始化OLED
    Tuya_Uart_Init(9600);          //涂鸦通信串口初始化
    wifi_protocol_init();          //初始化涂鸦模组
    MY_ADC_Init();                 //初始化ADC采样
    Sht30_Singleshot_Init();       //初始化SHT30
    delay_ms(50);
    
                      
    
    OLED_Show_Title(4,0,16);       //绘制OLED显示界面
    
    while(1)
    {
      
      if(sensor_flag == 1)  //每隔3S采样温湿度
      {
        //清空标志
        sensor_flag = 0;    
        //传感器数据采集  
        Sht30_GetTH_Singleshot_Poll(&t, &h, CMD_MEAS_POLLING_H);
        light = Get_Adc_Average();
        //数据处理和OLED显示
        temp = t*10; humi = h*10;
        Show_TempAndHumi(70,28,70,48,16);
        //温度数据报警检测
        if(t > temp_max) temp_alarm = TEMP_UPPER_ALARM;
        else if(t < temp_min) temp_alarm = TEMP_LOWER_ALARM;
        else temp_alarm = TEMP_ALARM_CANCEL;
        //亮度数值报警检测
        if(light < light_min) light_alarm = LIGHT_LOWER_ALARM;
        else light_alarm  = LIGHT_ALARM_CANCEL;
        //数据上报
        mcu_dp_value_update(DPID_TEMP_CURRENT,temp); //VALUE型数据上报;
        mcu_dp_value_update(DPID_HUMIDITY_VALUE,humi); //VALUE型数据上报;
        mcu_dp_value_update(DPID_LIGHT_VALUE,light); //VALUE型数据上报;  
        mcu_dp_enum_update(DPID_TEMP_ALARM,temp_alarm); //枚举型数据上报;
        mcu_dp_enum_update(DPID_LIGHT_ALARM,light_alarm); //枚举型数据上报; 
      }
      
      if(device_state != 0x00)
      {
        if(device_state == 0x01) Device_OFF();
        else Device_ON();
        device_state = 0x00;
      }
      

      wifi_uart_service();
      
      switch(mcu_get_wifi_work_state())
      {
          case SMART_CONFIG_STATE:    Light_Alarm(100);break;//Smart
          case AP_STATE          :    Light_Alarm(500);break;//AP
          case WIFI_NOT_CONNECTED:    LED(1);break;//未连接
          case WIFI_CONNECTED    :    LED(0);break;//已连接
          case WIFI_CONN_CLOUD   :    LED(0);break;//已连接
          case WIFI_LOW_POWER    :    LED(1);break;//低功耗
          case SMART_AND_AP_STATE:    Light_Alarm(100);break;//Smart/AP共存
          default:;break;
      }
      
      times++;


	}
}
