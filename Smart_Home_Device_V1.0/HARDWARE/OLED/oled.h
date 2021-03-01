#ifndef __OLED_H
#define __OLED_H			  	 
#include "stm32f0xx.h"



#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DAT 1	//Ð´Êý¾Ý

#define OLED_SCK_Pin GPIO_PIN_3
#define OLED_SCK_GPIO_Port GPIOB
#define OLED_SDIN_Pin GPIO_PIN_4
#define OLED_SDIN_GPIO_Port GPIOB
#define OLED_RES_Pin GPIO_PIN_5
#define OLED_RES_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_7
#define OLED_CS_GPIO_Port GPIOB

#define OLED_SCK(x) 		HAL_GPIO_WritePin(OLED_SCK_GPIO_Port, OLED_SCK_Pin, x)
#define OLED_SDIN(x)   	    HAL_GPIO_WritePin(OLED_SDIN_GPIO_Port, OLED_SDIN_Pin, x)
#define OLED_RES(x)		    HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, x)
#define OLED_DC(x)			HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, x)
#define OLED_CS(x)			HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, x)


void OLED_Init(void);
void OLED_SET(void);
void Send_Debug(char* Message);
void OLED_Clear(void);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_Refresh_Gram(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size);
void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t px, uint8_t py,const uint8_t *index, uint16_t size);
void OLED_ScreenUDMove(uint8_t PageS, uint8_t PageE, uint8_t ColS, uint8_t ColE, uint8_t UorD, uint8_t Loop);

#endif  
	 







 

