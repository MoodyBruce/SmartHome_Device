#include "oled.h"
#include "oledfont.h" 
#include "string.h"
#include "delay.h"

//OLED的显存
//存放格式如下.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 		   
volatile static uint8_t OLED_GRAM[128][8];


//写数据/指令 cmd 1-数据，0-指令 

void OLED_Write(uint8_t byte ,uint8_t cmd)
{ 
    uint8_t i;
	OLED_CS(0);
	OLED_DC(cmd);
    for(i=0;i<8;i++)
       { 
	     OLED_SCK(0);
         
		 OLED_SDIN((byte&0x80)>>7);
         
		 byte=byte<<1;
	     
		 delay_us(10);
		 OLED_SCK(1);
        }
	OLED_DC(1);
	OLED_CS(1);
}

//开启OLED显示    
void OLED_Display_On(void)
{
	OLED_Write(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_Write(0X14,OLED_CMD);  //DCDC ON
	OLED_Write(0XAF,OLED_CMD);  //DISPLAY ON
}
//关闭OLED显示     
void OLED_Display_Off(void)
{
	OLED_Write(0X8D,OLED_CMD);  //SET DCDC命令
	OLED_Write(0X10,OLED_CMD);  //DCDC OFF
	OLED_Write(0XAE,OLED_CMD);  //DISPLAY OFF
}

//更新显示屏，将改变后的GRAM数组写入1306显存
void OLED_Refresh_Gram()
{

		uint8_t i,n;
		for(i=0;i<8;i++)
	{
		OLED_Write(0xB0+i,OLED_CMD);
		OLED_Write(0x00,OLED_CMD);
		OLED_Write(0x10,OLED_CMD);
		
		for(n=0;n<128;n++)
		OLED_Write(OLED_GRAM[n][i],OLED_DAT);
	}
}

//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OLED_Clear(void)  
{  
	uint8_t i,n;  
	for(i=0;i<8;i++)for(n=0;n<128;n++)OLED_GRAM[n][i]=0X00;  
	OLED_Refresh_Gram();//更新显示
}

//画点 
//x:0~127
//y:0~63
//t:1 填充 0,清空	
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t)
{
		uint8_t pos,bx,temp = 0;
		if(x>127||y>63)return;//超出屏幕区域
		pos = 7-y/8;			  //获取页地址
		bx = y%8;				  //当前页的行数
		temp = 1<<(7-bx);		  
		if(t) OLED_GRAM[x][pos] |= temp;
		else  OLED_GRAM[x][pos]	&= ~temp;
}

//x1,y1,x2,y2 填充区域的对角坐标
//确保x1<=x2;y1<=y2 0<=x1<=127 0<=y1<=63	 	 
//dot:0,清空;1,填充	  
void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot)  
{  
	uint8_t x,y;  
	for(x=x1;x<=x2;x++)
	{
		for(y=y1;y<=y2;y++)OLED_DrawPoint(x,y,dot);
	}													    
	OLED_Refresh_Gram();//更新显示
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 12/16/24
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size,uint8_t mode)
{      			    
	uint8_t temp,t,t1;
	uint8_t y0=y;
	uint8_t csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数
	chr=chr-' ';//得到偏移后的值		 
    for(t=0;t<csize;t++)
    {   
		if(size==12)temp=asc2_1206[chr][t]; 	 	//调用1206字体
		else if(size==16)temp=asc2_1608[chr][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[chr][t];	//调用2412字体
		else return;								//没有的字库
        for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
    }          
}	

//m^n函数
uint32_t mypow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{         	
	uint8_t t,temp;
	uint8_t enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/mypow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ',size,1);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0',size,1); 
	}
} 
//显示字符串
//x,y:起点坐标  
//size:字体大小 
//*p:字符串起始地址 
void OLED_ShowString(uint8_t x,uint8_t y,const uint8_t *p,uint8_t size)
{	
    while((*p<='~')&&(*p>=' '))//判断是不是非法字符!
    {       
        if(x>(128-(size/2))){x=0;y+=size;}
        if(y>(64-size)){y=x=0;OLED_Clear();}
        OLED_ShowChar(x,y,*p,size,1);	 
        x+=size/2;
        p++;
    }  
	
}	


	

		
void OLED_Init(){
    
  GPIO_InitTypeDef GPIO_InitStruct = {0};
        
  __HAL_RCC_GPIOB_CLK_ENABLE();
        
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, OLED_SCK_Pin|OLED_SDIN_Pin|OLED_RES_Pin|OLED_DC_Pin 
                          |OLED_CS_Pin, GPIO_PIN_SET);
        
  /*Configure GPIO pins : OLED_SCL_Pin OLED_MOSI_Pin OLED_RES_Pin OLED_RS_Pin 
                           OLED_CS_Pin */
  GPIO_InitStruct.Pin = OLED_SCK_Pin|OLED_SDIN_Pin|OLED_RES_Pin|OLED_DC_Pin 
                          |OLED_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  
		OLED_RES(0);
		delay_ms(100);
		OLED_RES(1);
	
		
		OLED_Write(0xAE,OLED_CMD); //关闭显示
		OLED_Write(0xD5,OLED_CMD); //设置时钟分频因子,震荡频率
		OLED_Write(80,OLED_CMD);   //[3:0],分频因子;[7:4],震荡频率
		OLED_Write(0xA8,OLED_CMD); //设置驱动路数
		OLED_Write(0X3F,OLED_CMD); //默认0X3F(1/64) 
		OLED_Write(0xD3,OLED_CMD); //设置显示偏移
		OLED_Write(0X00,OLED_CMD); //默认为0

		OLED_Write(0x40,OLED_CMD); //设置显示开始行 [5:0],行数.
															
		OLED_Write(0x8D,OLED_CMD); //电荷泵设置
		OLED_Write(0x14,OLED_CMD); //bit2，开启/关闭
		OLED_Write(0x20,OLED_CMD); //设置内存地址模式
		OLED_Write(0x02,OLED_CMD); //[1:0],00，列地址模式;01，行地址模式;10,页地址模式;默认10;
		OLED_Write(0xA1,OLED_CMD); //段重定义设置,bit0:0,0->0;1,0->127;
		OLED_Write(0xC0,OLED_CMD); //设置COM扫描方向;bit3:0,普通模式;1,重定义模式 COM[N-1]->COM0;N:驱动路数
		OLED_Write(0xDA,OLED_CMD); //设置COM硬件引脚配置
		OLED_Write(0x12,OLED_CMD); //[5:4]配置
			 
		OLED_Write(0x81,OLED_CMD); //对比度设置
		OLED_Write(0xEF,OLED_CMD); //1~255;默认0X7F (亮度设置,越大越亮)
		OLED_Write(0xD9,OLED_CMD); //设置预充电周期
		OLED_Write(0xf1,OLED_CMD); //[3:0],PHASE 1;[7:4],PHASE 2;
		OLED_Write(0xDB,OLED_CMD); //设置VCOMH 电压倍率
		OLED_Write(0x30,OLED_CMD); //[6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc;

		OLED_Write(0xA4,OLED_CMD); //全局显示开启;bit0:1,开启;0,关闭;(白屏/黑屏)
		OLED_Write(0xA6,OLED_CMD); //设置显示方式;bit0:1,反相显示;0,正常显示	    						   
		OLED_Write(0xAF,OLED_CMD); //开启显示
		OLED_Clear();
}

void OLED_SET(){

	
}

//发送调试信息到OLED中
void Send_Debug(char* Message){

	static uint8_t i = 0;//第一次显示的行数
	OLED_ShowString(0,i*12,(uint8_t*)Message,12);
	i=i+1;
	OLED_Refresh_Gram();
	if(i>5) i=1;

}

//发送调试信息到OLED中
void Send_Debug_Two(char* Message)
{
	
	OLED_ScreenUDMove(2,7,0,127,'U',0);
	OLED_Refresh_Gram();
	OLED_ShowString(0,48,(uint8_t*)Message,12);
	OLED_Refresh_Gram();

}

//显示图片（通过改变索引值和图片对应的像素可以显示不同像素的图片）
//x，y:图片的起点坐标
//px,py:图片的像素（与实际取模的图片像素一致）
//index:图片索引
void OLED_ShowBMP(uint8_t x, uint8_t y, uint8_t px, uint8_t py,const uint8_t *index, uint16_t size)
{
    u8 temp,t1;
    u16 j,i;
    u8 y0=y;
    
    i = size;
 
    for(j = 0; j < i;j++)
    {
        temp = index[j];    //调用图片                          
        for(t1=0;t1<8;t1++)
        {
            if(temp&0x80)OLED_DrawPoint(x,y,1);
            else OLED_DrawPoint(x,y,0);
            temp<<= 1;
            y++;
            if((y-y0) == py)
            {
                y=y0;
                x++;
                break;
            }
        }
    }
    OLED_Refresh_Gram();
}


/**
  * @brief	:oled局部更新显示
  * @note   :--
  * @param	:PageS , 起始页
             PageE , 终止页
             ColS  , 起始列
             ColE  , 终止列
             Direct,刷新模式(1纵向 0横向)
  * @return	:void
  *
  * @date   :2016/12/24
  * @design :
  **/
void OLED_UpdatePart(uint8_t PageS, uint8_t PageE, uint8_t ColS, uint8_t ColE, uint8_t Direct)
{
	uint8_t x=0,y=0;

    if(Direct)//纵向刷新
    {        
        OLED_Write(0x20,OLED_CMD);//设置为水平寻址模式，因为页寻址下的移动操作错位严重(刷新时间太长！)	
        OLED_Write(0x00,OLED_CMD);
        OLED_Write(0x21,OLED_CMD);//设置水平寻址起始列和终止列
        OLED_Write(ColS,OLED_CMD);
        OLED_Write(ColE,OLED_CMD);
        OLED_Write(0x22,OLED_CMD);//设置水平寻址起始页和终止页
        OLED_Write(PageS,OLED_CMD);
        OLED_Write(PageE,OLED_CMD);

        for(y=PageS;y<=PageE;y++)
        {		
            for(x=ColS;x<=ColE;x++){		
                OLED_Write(OLED_GRAM[x][y],OLED_DAT);
            }
        }
    }
    else    //横向刷新
    {
        OLED_Write(0x20,OLED_CMD);//设置为垂直寻址模式，因为页寻址下的移动操作错位严重(刷新时间太长！)	
        OLED_Write(0x01,OLED_CMD);
        OLED_Write(0x21,OLED_CMD);//设置垂直寻址起始列和终止列
        OLED_Write(ColS,OLED_CMD);
        OLED_Write(ColE,OLED_CMD);
        OLED_Write(0x22,OLED_CMD);//设置垂直寻址起始页和终止页
        OLED_Write(PageS,OLED_CMD);
        OLED_Write(PageE,OLED_CMD);
        
        for(x=ColS;x<=ColE;x++)
        {		
            for(y=PageS;y<=PageE;y++){		
                OLED_Write(OLED_GRAM[x][y],OLED_DAT);
            }
        }        
    }
    
    OLED_Write(0x20,OLED_CMD);//恢复默认寻址模式，页寻址模式！
    OLED_Write(0x02,OLED_CMD);
}

/**
  * @brief	:上下移屏操作(像素坐标)
  * @note   :--UorD 大小写无妨！
  * @param	:PageS, 起始页
             PageE, 终止页
             ColS , 起始列
             ColE , 终止列
             UorD , 屏幕移动方向
             Loop , 模式选择(1,循环 0,不循环)
  * @return	:void
  *
  * @date   :2016/12/24
  * @design :
  **/
void OLED_ScreenUDMove(uint8_t PageS, uint8_t PageE, uint8_t ColS, uint8_t ColE, uint8_t UorD, uint8_t Loop)
{
    uint8_t page=0,col=0;
    uint8_t temp[8]={0};
    
    UorD |= (1<<5);//转化为小写形式！

    for(col=ColS;col<=ColE;col++)
    {
        if(UorD=='u')
        {
            for(page=PageS;page<=PageE;page++)
            {
                temp[page] = (OLED_GRAM[col][page]&0x01)<<7;//取出最低位，并移动至最高位
                OLED_GRAM[col][page] >>= 1;//数据向低位移动，即向上移动
            }
            
            for(page=PageS;page<=PageE;page++)
            {
                if(Loop)//采用循环模式
                {
                    OLED_GRAM[col][page] |= temp[(page+1)%8];
                }else{
                    OLED_GRAM[col][page] |= 0;
                }
            }    
            
            OLED_GRAM[col][PageE] |= temp[PageS]; 
        }else if(UorD=='d')
        {
            for(page=PageS;page<=PageE;page++)
            {
                temp[page] = (OLED_GRAM[col][page]&0x80)>>7;//取出最高位，并移动至最低位
                OLED_GRAM[col][page] <<= 1;//数据向高位移动，即向下移动
            }
            
            for(page=PageS;page<=PageE;page++)
            {
                if(Loop)//采用循环模式
                {
                    OLED_GRAM[col][page] |= temp[(page+7)%8];
                }else{
                    OLED_GRAM[col][page] |= 0;
                }
            }   
            
            OLED_GRAM[col][PageS] |= temp[PageE]; 
        }        
    }

    OLED_UpdatePart(PageS, PageE, ColS, ColE, 1);
}







