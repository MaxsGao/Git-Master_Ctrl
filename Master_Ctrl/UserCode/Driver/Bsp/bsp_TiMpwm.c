#include "bsp_TiMpwm.h"

/************************************************* 
*Function:	 PWM_Init_Tim8_CH3_CH4
*Input:      arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
*OUTPUT:		    
*Return:		
*DESCRIPTION:������PWM����ʼ�����ú���
*           PC8 �C 65 �C LED_CTL_PWM1 �C TIM8_CH3 �C ů�׹�PWM����
*           PC9 �C 66 �C LED_CTL_PWM2 �C TIM8_CH4 �C ��׹�PWM����
*************************************************/
void PWM_Init_Tim8_CH3_CH4(u16 arr,u16 psc)
{
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_OCInitTypeDef           TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE)	;//��C��GPIOʱ��
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,ENABLE)  ;// �򿪶�ʱ��8ʱ��

    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;   
    GPIO_Init(GPIOC,&GPIO_InitStructure);

	//��ʼ��TIM8
	TIM_TimeBaseStructure.TIM_Period		= arr				 ;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler		= psc				 ;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision	= 0					 ;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up ;//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM8,&TIM_TimeBaseStructure)				 ;//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM8 Channe3��4  PWMģʽ
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;

    TIM_OC3Init(TIM8,&TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM8,TIM_OCPreload_Enable); //TIM_8---->ͨ��3[PC8] 

    TIM_OC4Init(TIM8,&TIM_OCInitStructure);
    TIM_OC4PreloadConfig(TIM8,TIM_OCPreload_Enable); 

    TIM_CtrlPWMOutputs(TIM8,ENABLE);
    TIM_Cmd(TIM8,ENABLE);
}

/************************************************* 
*Function:	 PWM_Init_Tim8_CH3_CH4
*Input:      arr���Զ���װֵ psc��ʱ��Ԥ��Ƶ��
*OUTPUT:		    
*Return:		
*DESCRIPTION:������PWM����ʼ�����ú���
*           PC8 �C 65 �C LED_CTL_PWM1 �C TIM8_CH3 �C ů�׹�PWM����
*           PC9 �C 66 �C LED_CTL_PWM2 �C TIM8_CH4 �C ��׹�PWM����
*************************************************/
void PWM_Init_Tim3_CH1_CH2_CH3(u16 arr,u16 psc)
{
    GPIO_InitTypeDef            GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
    TIM_OCInitTypeDef           TIM_OCInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB,ENABLE)	;//��C��GPIOʱ��
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE)  ;// �򿪶�ʱ��8ʱ��

    GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;  
    GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_6|GPIO_Pin_7;   
    GPIO_Init(GPIOA,&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_0;   
    GPIO_Init(GPIOB,&GPIO_InitStructure);

	//��ʼ��TIM8
	TIM_TimeBaseStructure.TIM_Period		= arr				 ;//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler		= psc				 ;//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
    TIM_TimeBaseStructure.TIM_ClockDivision	= 0					 ;//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up ;//TIM���ϼ���ģʽ
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure)				 ;//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM8 Channe3��4  PWMģʽ
    TIM_OCInitStructure.TIM_OCMode      = TIM_OCMode_PWM2;
    TIM_OCInitStructure.TIM_OCPolarity  = TIM_OCPolarity_Low;
    TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse       = 0;

    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Reset;
    TIM_OCInitStructure.TIM_OCIdleState  = TIM_OCIdleState_Reset;

    TIM_OC1Init(TIM3,&TIM_OCInitStructure);
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable); //TIM_8---->ͨ��3[PC8] 

    TIM_OC2Init(TIM3,&TIM_OCInitStructure);
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable); 

    TIM_OC3Init(TIM3,&TIM_OCInitStructure);
    TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);

    TIM_CtrlPWMOutputs(TIM3,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
}

