#include "i2c.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"

void IIC_Sht30_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();  
  
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, SHT30_SCL_PIN|SHT30_SDA_PIN, GPIO_PIN_SET);

  /*Configure GPIO pins : SHT_SCL_Pin SHT_SDA_Pin */
  GPIO_InitStruct.Pin = SHT30_SCL_PIN|SHT30_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

void IIC_Sht30_Start(void)
{
	IIC_SHT30_SDA(1);
	delay_us(1);
	IIC_SHT30_SCL(1);
	delay_us(1);
	IIC_SHT30_SDA(0);
	delay_us(10);
	IIC_SHT30_SCL(0);
	delay_us(10);
}

void IIC_Sht30_Stop(void)
{
	IIC_SHT30_SCL(0);
	delay_us(1);
	IIC_SHT30_SDA(0);
	delay_us(1);
	IIC_SHT30_SCL(1);
	delay_us(10);
	IIC_SHT30_SDA(1);
	delay_us(10);	
}

u8 IIC_Sht30_Write_Byte(u8 txd)
{
	u8 mask, res;
	IIC_SHT30_SCL(0);
	for(mask = 0x80; mask > 0; mask >>= 1)
	{
		if((mask & txd) == 0){
			IIC_SHT30_SDA(0);
		}
		else{
			IIC_SHT30_SDA(1);
		}
		delay_us(1);
		IIC_SHT30_SCL(1);
		delay_us(5);
		IIC_SHT30_SCL(0);
		delay_us(1);
	}
	
	IIC_SHT30_SDA(1);
	IIC_SHT30_SCL(1);
	delay_us(1);
	if(READ_SDA_SHT30 == 1){
		res = NACK;
	}
	else{
		res = ACK;
	}
    printf("%d\r\n",res);
	IIC_SHT30_SCL(0);
	delay_us(20);
	
	return res;
}

u8 IIC_Sht30_Read_Byte(u8 ack)
{
	u8 mask;
	u8 receive = 0x00;
	IIC_SHT30_SDA(1);
	IIC_SHT30_SCL(0);
	delay_us(2);
	
	for(mask = 0x80; mask > 0; mask >>= 1)
	{
		IIC_SHT30_SCL(1);
		delay_us(4);
		if(READ_SDA_SHT30 == 1){
			receive |= mask;
		}
		IIC_SHT30_SCL(0);
		delay_us(2);
	}
	
	if(ack == ACK){
		IIC_SHT30_SDA(0);
	}
	else{
		IIC_SHT30_SDA(1);
	}
	
	delay_us(1);
	IIC_SHT30_SCL(1);
	delay_us(5);
	IIC_SHT30_SCL(0);
	IIC_SHT30_SDA(1);
	delay_us(20);

	return receive;
}












