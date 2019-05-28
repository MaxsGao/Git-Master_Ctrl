#ifndef __LED_H
#define __LED_H
#include "user_main.h"

#ifndef ON
    #define ON  1
#endif
#ifndef OFF
    #define OFF 0
#endif

#if 0

#define RCC_LED0				RCC_APB2Periph_GPIOE
#define GPIO_LED0_PORT			GPIOE
#define GPIO_Pin_LED0			GPIO_Pin_5

#define RCC_LED1				RCC_APB2Periph_GPIOE
#define GPIO_LED1_PORT			GPIOE
#define GPIO_Pin_LED1			GPIO_Pin_6

#define LED_0 PEout(5)// PE5
#define LED_1 PEout(6)// PE6

//带参宏，可以像内联函数一样使用
#define LED0(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED0_PORT,GPIO_Pin_LED0);\
					else		\
					GPIO_SetBits(GPIO_LED0_PORT,GPIO_Pin_LED0)

#define LED1(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED1_PORT,GPIO_Pin_LED1);\
					else		\
					GPIO_SetBits(GPIO_LED1_PORT,GPIO_Pin_LED1)

#else

#define RCC_LED0				RCC_APB2Periph_GPIOA
#define GPIO_LED0_PORT			GPIOA
#define GPIO_Pin_LED0			GPIO_Pin_1

#define RCC_LED1				RCC_APB2Periph_GPIOA
#define GPIO_LED1_PORT			GPIOA
#define GPIO_Pin_LED1			GPIO_Pin_2

#define LED_0 PAout(1)// PA1
#define LED_1 PAout(2)// PA2

//带参宏，可以像内联函数一样使用
#define LED0(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED0_PORT,GPIO_Pin_LED0);\
					else		\
					GPIO_SetBits(GPIO_LED0_PORT,GPIO_Pin_LED0)

#define LED1(a)	if (a)	\
					GPIO_ResetBits(GPIO_LED1_PORT,GPIO_Pin_LED1);\
					else		\
					GPIO_SetBits(GPIO_LED1_PORT,GPIO_Pin_LED1)
#endif
                    
void LED_Init(void);//初始化
	 				    
#endif
