#include "bsp_TiMpwm.h"

/************************************************* 
*Function:	 PWM_Init_Tim8_CH3_CH4
*Input:      arr：自动重装值 psc：时钟预分频数
*OUTPUT:		    
*Return:		
*DESCRIPTION:背景灯PWM波初始化配置函数
*           PC8 – 65 – LED_CTL_PWM1 – TIM8_CH3 – 暖白光PWM控制
*           PC9 – 66 – LED_CTL_PWM2 – TIM8_CH4 – 冷白光PWM控制
*************************************************/
void PWM_Init_Tim8_CH3_CH4(u16 arr,u16 psc)
{
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_OCInitTypeDef           TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE)	;//打开C组GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE)  ;// 打开定时器8时钟

    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;   
    GPIO_Init(GPIOC,&GPIO_InitStructure);

	//初始化TIM8
	TIM_TimeBaseStructure.TIM_Period		= arr				 ;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler		= psc				 ;//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision	= 0					 ;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up ;//TIM向上计数模式
    TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure)				 ;//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM8 Channe3、4  PWM模式
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;

    TIM_OC3Init(TIM8,&TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable); //TIM_8---->通道3[PC8] 

    TIM_OC4Init(TIM8,&TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable); 

    TIM_CtrlPWMOutputs(TIM8,ENABLE);
    TIM_Cmd(TIM8,ENABLE);
}

/************************************************* 
*Function:	 PWM_Init_Tim8_CH3_CH4
*Input:      arr：自动重装值 psc：时钟预分频数
*OUTPUT:		    
*Return:		
*DESCRIPTION:背景灯PWM波初始化配置函数
*           PC8 – 65 – LED_CTL_PWM1 – TIM8_CH3 – 暖白光PWM控制
*           PC9 – 66 – LED_CTL_PWM2 – TIM8_CH4 – 冷白光PWM控制
*************************************************/
void PWM_Init_Tim3_CH1_CH2_CH3(u16 arr,u16 psc)
{
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_OCInitTypeDef           TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE)	;//打开C组GPIO时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE)  ;// 打开定时器8时钟

    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6|GPIO_Pin_7;   
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;   
    GPIO_Init(GPIOB,&GPIO_InitStructure);

	//初始化TIM8
	TIM_TimeBaseStructure.TIM_Period		= arr				 ;//设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler		= psc				 ;//设置用来作为TIMx时钟频率除数的预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision	= 0					 ;//设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up ;//TIM向上计数模式
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure)				 ;//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

	//初始化TIM8 Channe3、4  PWM模式
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //TIM_8---->通道3[PC8] 

    TIM_OC2Init(TIM3,&TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); 

    TIM_OC3Init(TIM3,&TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM3,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
}

