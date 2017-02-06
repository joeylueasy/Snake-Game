/******************** (C) COPYRIGHT 2013 STMicroelectronics ********************
* File Name          : main.c
* Author             : WWW.ARMJISHU.COM之STM32核心团队  JESSE 
* Version            :
* Date               : 
* Description        : STM32神舟III号开发板    广州
*******************************************************************************/
#include "led.h"

/*初始化4个LED灯*/
void LED_config(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure;

	  /* 配置神舟III号LED灯使用的GPIO管脚模式*/
  	RCC_APB2PeriphClockCmd(RCC_GPIO_LED, ENABLE); /*使能LED灯使用的GPIO时钟*/

  	GPIO_InitStructure.GPIO_Pin = DS1_PIN|DS2_PIN|DS3_PIN|DS4_PIN; 
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  
  	GPIO_Init(GPIO_LED, &GPIO_InitStructure);  /*神州III号使用的LED灯相关的GPIO口初始化*/
		GPIO_SetBits(GPIOF,GPIO_Pin_6);//第一灯亮
		GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第二灯灭
		GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
		GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
}

void LED_change(uint8_t MoveDirection){
	switch(MoveDirection){
 			case 0:
				GPIO_SetBits(GPIOF,GPIO_Pin_6);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				break;
			case 1:
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				break;	
			case 2:
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				break;		
			case 3:
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				break;				
			case 4:
				GPIO_ResetBits(GPIOF,GPIO_Pin_9);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第4灯灭
				break;	
			default:
				GPIO_SetBits(GPIOF,GPIO_Pin_6);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				break;
	}
}


