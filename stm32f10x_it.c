/**
  ******************************************************************************
  * @file    USART/Printf/stm32f10x_it.c 
  * @author  MCD Application Team
  * @version V3.3.0
  * @date    04/16/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include <stdio.h>
#include "led.h"

/** @addtogroup STM32F10x_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_Printf
  * @{
  */ 



/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/**-------------------------------------------------------
  * @函数名 EXTI0_IRQHandler
  * @功能   处理外部中断线0的中断，按键4中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void EXTI0_IRQHandler(void)  /* Key 4 */
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        /* 延迟去抖 */
        delay(150000);
        if(KEY4IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_9);//第一灯亮
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);	 //第3灯灭
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);	 //第4灯灭
        }
        /* 等待按键释放 */
        while(KEY4IBB == 0);
        /* 延迟去抖 */
        delay(150000);        

        /* Clear the EXTI Line 0 */
        /* 清除中断挂起标志位，否则会被认为中断没有被处理而循环再次进入中断 */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

/**-------------------------------------------------------
  * @函数名 EXTI15_10_IRQHandler
  * @功能   处理外部中断线10-15的中断，按键3中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void EXTI15_10_IRQHandler(void) /* Key 3 */
{
    if(EXTI_GetITStatus(EXTI_Line13) != RESET)
    {
        /* 延迟去抖 */
        delay(150000);
            if(KEY3IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_8);//第一灯亮
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
        }
        /* 等待按键释放 */
        while(KEY3IBB == 0);
        /* 延迟去抖 */
        delay(150000);

        /* Clear the EXTI Line 13 */  
        /* 清除中断挂起标志位，否则会被认为中断没有被处理而循环再次进入中断 */
        EXTI_ClearITPendingBit(EXTI_Line13);
    }
}

/**-------------------------------------------------------
  * @函数名 EXTI9_5_IRQHandler
  * @功能   处理外部中断线8的中断，按键1中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void EXTI9_5_IRQHandler(void) /* Key 1 */
{
    if(EXTI_GetITStatus(EXTI_Line8) != RESET)
    {
        /* 延迟去抖 */
        delay(150000);
        if(KEY1IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);//第一灯亮
				GPIO_SetBits(GPIOF,GPIO_Pin_7);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
				if(StartFlag==GAMEOVER)	StartFlag=GAMEWAITING;
        }
        /* 等待按键释放 */
				seed=0;
        while(KEY1IBB == 0) {seed++;if(seed>5000) seed=0;}
				
        /* 延迟去抖 */
        delay(150000);        

        /* Clear the EXTI Line 4 */
        /* 清除中断挂起标志位，否则会被认为中断没有被处理而循环再次进入中断 */
        EXTI_ClearITPendingBit(EXTI_Line8);
    }
}

/**-------------------------------------------------------
  * @函数名 EXTI3_IRQHandler
  * @功能   处理外部中断线3的中断，按键2中断处理函数
  * @参数   无
  * @返回值 无
***------------------------------------------------------*/
void EXTI3_IRQHandler(void) /* Key 2 */
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET)
    {
        /* 延迟去抖 */
        delay(150000);
        if(KEY2IBB == 0)
        {
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);//第一灯亮
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);	 //第二灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_8);	 //第3灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_9);	 //第4灯灭
						if(StartFlag==GAMEING){   //如果在游戏中
								StartFlag=GAMEPAUSE;
							  //TIM_Cmd(TIM5, DISABLE); 
						}
						else if(StartFlag==GAMEPAUSE){   //如果在游戏中
								StartFlag=GAMEING;
								//TIM_Cmd(TIM5, ENABLE); 
				}						
        }
        /* 等待按键释放 */
        while(KEY2IBB == 0);
        /* 延迟去抖 */
        delay(150000);        

        /* Clear the EXTI Line 4 */
        /* 清除中断挂起标志位，否则会被认为中断没有被处理而循环再次进入中断 */
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}

