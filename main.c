/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM之STM32核心团队  JESSE 
* Version            :
* Date               :
* Description        : STM32神舟III号开发板    广州
*******************************************************************************/

#include "stm32f10x.h"
#include "ili9320.h"
#include "ili9320_api.h"
#include <stdio.h>
#include "ARMJISHU_TouchScreen_ADS7843.h"
#include "SZ_STM32F103ZE_LIB.h"
#include "I2C_MPU6050.h"
#include "led.h"

//#include "game_control.h"
extern uint8_t MoveDirection;	//移动方向
extern u8 StartFlag;
u8 timeUpFlag=0;//蛇1s移动一下，1s定时到的标志位

#define dirNone 0
#define dirLeft 1
#define dirRight 2
#define dirFront 3
#define dirBack 4
#define GAMEOVER			1
#define GAMEWAITING   2
#define GAMEING				3
#define GAMEPAUSE			4
#define NONE					0
#define LEFT					1
#define RIGHT					2
#define UP						3
#define DOWN					4
/*-------------------------------------------------------------------------------*/
#define BmpHeadSize (54)
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)

GPIO_InitTypeDef GPIO_InitStructure;

typedef struct _HEADCOLOR
{
   unsigned char scan;
   unsigned char gray;
   unsigned short w;
   unsigned short h;
   unsigned char is565;
   unsigned char rgb;
}HEADCOLOR; 

/**-------------------------------------------------------
  * @函数名 LCD_DrawBmp565Pic
  * @功能   在指定座标范围显示一幅Image2Lcd转换的RGB565图片
  * @参数   StartX     行起始座标
  *         StartY     列起始座标
  *         Width      图片宽度
  *         Height     图片高度
  *         BmpAddress 图片数据区域头指针(不含文件信息)
  * @返回值 无
  * 作者    www.armjishu.com
***------------------------------------------------------*/
void LCD_DrawBmp565Pic(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height, const uint16_t *BmpAddress)
{
//    uint32_t total;
    uint32_t i, j;
    uint32_t pointor;
    uint16_t line;

    //LCD_DEBUG_PRINTF("LCD_DrawBmp565Picture StartX %d StartY %d Width %d Height %d \n\r"
    //                              , StartX, StartY, Width, Height);

    line=StartX;
#if 0  // 这种方法绘制速度快，但是寄存器因为LCD驱动芯片的不同而不同
    total = Width * Height;

    LCD_WriteReg(0x0044,0xEF00 + StartX);        //Specify the start/end positions of the window address in the horizontal direction by an address unit
    LCD_WriteReg(0x0045,StartY);        //Specify the start positions of the window address in the vertical direction by an address unit 
    LCD_WriteReg(0x0046,StartY + Width -1 );        //Specify the end positions of the window address in the vertical direction by an address unit 

    ili9320_SetCursor(line, StartY);
  	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

    for (i=0;i<total;i++)
    {
        LCD_WriteRAM(BmpAddress[i]);
    }

    LCD_WriteReg(0x0044,0xEF00);        //Specify the start/end positions of the window address in the horizontal direction by an address unit
    LCD_WriteReg(0x0045,0x0000);        //Specify the start positions of the window address in the vertical direction by an address unit 
    LCD_WriteReg(0x0046,0x013F);        //Specify the end positions of the window address in the vertical direction by an address unit 
#else
    pointor = 0;
    for (i=0;i<Height;i++)
    {
        ili9320_SetCursor(line, StartY);
        LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
        for (j=0;j<Width;j++)
        {
            LCD_WriteRAM(BmpAddress[pointor]);
            pointor++;
        }
        line++;
    }
#endif
}

/**-------------------------------------------------------
  * @函数名 LCD_Image2LcdDrawBmp565Pic
  * @功能   在指定座标范围显示一幅Image2Lcd转换的Bmp565图片
  * @参数   StartX     行起始座标
  *         StartY     列起始座标
  *         BmpAddress 图片文件头的地址指针(含有文件信息)
  * @返回值 无
  * 作者    www.armjishu.com
***------------------------------------------------------*/
void LCD_Image2LcdDrawBmp565Pic(uint16_t StartX, uint16_t StartY, const uint8_t *BmpAddress)
{
    HEADCOLOR * BmpHeadr;

    uint16_t Width, Height;
    uint8_t *BmpData;

    BmpHeadr = (HEADCOLOR *)BmpAddress;
    Width  = BmpHeadr->w;
    Height = BmpHeadr->h;
    BmpData = (uint8_t *)(BmpAddress + sizeof(HEADCOLOR));
    if((BmpHeadr->is565 == 1) && (BmpHeadr->gray == 16))
    {
        LCD_DrawBmp565Pic(StartX, StartY, Width, Height, (const uint16_t *)BmpData);
    }
    else
    {
//        LCD_DEBUG_PRINTF("\n\r LCD_Image2LcdDrawBmp565Pic: Not Image2Lcd or Bmp565 format.");
    }
}


void STM32_Shenzhou_COMInit(USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

  /* Enable UART clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); 

  /* Configure USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* Configure USART Rx as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* USART configuration */
  USART_Init(USART1, USART_InitStruct);
    
  /* Enable USART */
  USART_Cmd(USART1, ENABLE);
}



void ili9320_DrawPicture1(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic) 
{  
	u32 i, total; 
	u16 x,y;  
	// ili9320_SetWindows(StartX,StartY,EndX,EndY); 
	// ili9320_SetCursor(StartX,StartY); 
	x=StartX; 
	y=StartY; 
	// LCD_WriteRAM_Prepare(); 
	total = EndX*(EndY-StartY+1); 
	for (i=0;i<total;i++) 
	{  
		ili9320_SetPoint(x,y,*pic++); 
		y++; 
		if(y > EndY) 	
		{  
			x++;  
			y=StartY; 
		}  
	// LCD_WriteRAM(*pic++); 
	} 
} 

void Printf_Init(void)
{
  USART_InitTypeDef USART_InitStructure;          
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM32_Shenzhou_COMInit(&USART_InitStructure);

  //printf("\n\r ############ WWW.ARMJISHU.COM! ############ ("__DATE__ " - " __TIME__ ")");
  //printf(" WWW.ARMJISHU.COM use __STM32F10X_STDPERIPH_VERSION %d.%d.%d",__STM32F10X_STDPERIPH_VERSION_MAIN,
		//	__STM32F10X_STDPERIPH_VERSION_SUB1,	__STM32F10X_STDPERIPH_VERSION_SUB2);
  //printf("\n\r 产品内部Flash大小为：%dK字节！ \t www.armjishu.com",*(__IO uint16_t*)(0x1FFFF7E0));
  SystemCoreClockUpdate();
  //printf("\n\r 系统内核时钟频率(SystemCoreClock)为：%dHz.\n\r",SystemCoreClock);
}

int main(void)
{   
	uint8_t directionTemp=0;
/*-------------------------液晶屏初始化-------------------------------*/
	Printf_Init();
  ili9320_Initializtion();
  ili9320_Clear(0x9EDD);
/*-------------------------开始欢迎界面-------------------------------*/
	gamePicInit();
	Delay(0x3ff);		
/*--------------------------加速度计模块初始化-------------------------------*/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, DISABLE);
	I2C_MPU6050_Init();
	InitMPU6050();
	LED_config();
	NVIC_GroupConfig();
		
    /* 初始化板载按键为中断模式 */
    SZ_STM32_KEYInit(KEY1, BUTTON_MODE_EXTI);
    SZ_STM32_KEYInit(KEY2, BUTTON_MODE_EXTI);	
    SZ_STM32_KEYInit(KEY3, BUTTON_MODE_EXTI);
    SZ_STM32_KEYInit(KEY4, BUTTON_MODE_EXTI);
/*---------------------------TIM5定时器初始化-----------------------------*/
  TIM5_Init();
	
	
//Init_Snake();
/*---------------------------主while循环开始-------------------------------*/
  while (1){
		if(StartFlag==GAMEWAITING) 	Init_Snake();
		if(timeUpFlag){
			timeUpFlag=0;		
/*--------------------------1s定时到------------------------------*/
			directionTemp=directionJudge();
			if(directionTemp){
				if(MoveDirection==UP && directionTemp!=DOWN) MoveDirection=directionTemp;
				else if(MoveDirection==DOWN && directionTemp!=UP) MoveDirection=directionTemp;
				else if(MoveDirection==LEFT && directionTemp!=RIGHT) MoveDirection=directionTemp;	
				else if(MoveDirection==RIGHT && directionTemp!=LEFT) MoveDirection=directionTemp;				
			}
			gameMain();
		}
  }
}
