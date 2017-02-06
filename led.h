#ifndef __LED_H
#define __LED_H	
#include "stm32f10x.h" 

/*神州III号LED灯相关定义*/
#define RCC_GPIO_LED                    RCC_APB2Periph_GPIOF    /*LED使用的GPIO时钟*/
#define LEDn                            4                       /*神舟III号LED数量*/
#define GPIO_LED                        GPIOF                   /*神舟III号LED灯使用的GPIO组*/

#define DS1_PIN                         GPIO_Pin_6              /*DS1使用的GPIO管脚*/
#define DS2_PIN                         GPIO_Pin_7				/*DS2使用的GPIO管脚*/
#define DS3_PIN                         GPIO_Pin_8  			/*DS3使用的GPIO管脚*/
#define DS4_PIN                         GPIO_Pin_9				/*DS4使用的GPIO管脚*/

extern uint8_t MoveDirection;

void LED_config(void);
void LED_change(uint8_t MoveDirection);	 				    
#endif


