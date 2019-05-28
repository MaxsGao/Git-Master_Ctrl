#include "bsp_TiMbase.h"

unsigned int W5500_Send_Delay_Counter=0; //W5500发送延时计数变量(ms)

//通用定时器3中断初始化
//这里时钟选择为APB1的2倍，而APB1为36M
//arr：自动重装值。
//psc：时钟预分频数
//这里使用的是定时器3!
void TIM3_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef        NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
	
	//定时器TIM3初始化
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设置在下一个更新事件装入活动的自动重装载寄存器周期的值	
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据指定的参数初始化TIMx的时间基数单位
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE );                       //使能指定的TIM3中断,允许更新中断

	//中断优先级NVIC设置
	NVIC_InitStructure.NVIC_IRQChannel                      = TIM3_IRQn;//TIM3中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0;        //先占优先级0级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 3;        //从优先级3级
	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;   //IRQ通道被使能
	NVIC_Init(&NVIC_InitStructure);                                     //初始化NVIC寄存器

	TIM_Cmd(TIM3, ENABLE);  //使能TIMx					 
}

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		W5500_Send_Delay_Counter++;
	}
}
/*******************************************************************************
* 函数名  : Timer2_Init_Config
* 描述    : Timer2初始化配置
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void Timer2_Init_Config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;						//定义NVIC初始化结构体
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//使能Timer2时钟
  	/* Set the Vector Table base location at 0x08000000 */
  	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	TIM_TimeBaseStructure.TIM_Period = 9;						//设置在下一个更新事件装入活动的自动重装载寄存器周期的值(计数到10为1ms)
	TIM_TimeBaseStructure.TIM_Prescaler = 7199;					//设置用来作为TIMx时钟频率除数的预分频值(10KHz的计数频率)
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//设置时钟分割:TDTS = TIM_CKD_DIV1
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	 
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE ); 				//使能TIM2指定的中断
	
//	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);				//设置中断优先级组为1，优先组(可设0～4位)
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;				//设置中断向量号
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//设置抢先优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;			//设置响应优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;				//使能NVIC
	NVIC_Init(&NVIC_InitStructure);
    
	TIM_Cmd(TIM2, ENABLE);  									//使能TIMx外设
}

/*******************************************************************************
* 函数名  : TIM2_IRQHandler
* 描述    : 定时器2中断断服务函数
* 输入    : 无
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//		W5500_Send_Delay_Counter++;
//	}
//}
