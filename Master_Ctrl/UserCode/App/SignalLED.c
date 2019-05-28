#include "user_main.h"

static void LED_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
	RCC_APB2PeriphClockCmd( RCC_LED0, ENABLE);
//    RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO,ENABLE );
//    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    
  	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_LED0;	
  	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP; 
  	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIO_LED0_PORT, &GPIO_InitStructure);	
    										
	RCC_APB2PeriphClockCmd( RCC_LED1, ENABLE);
    
  	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_LED1;	
  	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP; 
  	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz; 
  	GPIO_Init(GPIO_LED1_PORT, &GPIO_InitStructure);
    
	/* 关闭所有led灯	*/
    LED0(OFF);
    LED1(OFF);
}

//LED IO初始化
void LED_Init(void)
{
	LED_GPIO_Init();
}
