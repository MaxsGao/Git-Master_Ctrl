/*
*********************************************************************************************************
*
*
*                                       Simulation Usart
*                                          模拟串口
* File : sl_usart_conf.h
* By   : Bear
* Date : 2018/11/23
* version : V0.1 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include "sl_usart_conf.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**@brief   配置PG1为TXD，PG0做RXD
	*@param   None
	*@ret     None
	*/
static void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能 GPIOG clock */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOG,ENABLE  );     ///> 开始复位RCC时钟复位
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOG,DISABLE );     ///> 停止复位RCC时钟复位
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE );     ///> 使能RCC时钟

	/* 防止上电后的误判 */
	GPIO_SetBits(GPIOG, GPIO_Pin_1);

	/* 配置PG1为推挽输出 */
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* 配置PG0为浮空输入 */
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**@brief   配置PG0上的下降沿触发外部中断
	*@param   None
	*@ret     None
	*/
static void EXTI_Config(void)
{
  	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the AFIO Clock */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, ENABLE );      ///> 开始时钟复位
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, DISABLE  );    ///> 停止时钟复位
  	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO, ENABLE  );

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOG, GPIO_PinSource0);

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel                      = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/**@brief   配置TIM1
	*@param   None
	*@ret     None
	*/
static void TIM1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, ENABLE );      ///> 开始时钟复位
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, DISABLE  );    ///> 停止时钟复位
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE  );

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period        = 8;
  	TIM_TimeBaseStructure.TIM_Prescaler     = 72-1;
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	
	/* 开启TIM1的中断 */
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  
    
    /* 选择TIM1的中断通道 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    
	/* 抢占式中断优先级设置为0 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    
	/* 响应式中断优先级设置为0 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
	/* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**@brief   配置TIM2
	*@param   None
	*@ret     None
	*/
static void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM2, ENABLE );      ///> 开始时钟复位
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM2, DISABLE  );    ///> 停止时钟复位
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE  );

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period        = 1000 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
	
	/* 开启TIM2的中断 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      
        
    /* 选择TIM2的中断通道 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      
    
	/* 抢占式中断优先级设置为0 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    
	/* 响应式中断优先级设置为0 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
	/* 使能中断 */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**@brief   配置TIM3
	*@param   None
	*@ret     None
	*/
static void TIM3_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period        = 65535 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_Cmd(TIM3,ENABLE);

}

/*
*********************************************************************************************************
*	函 数 名: EXTI15_10_IRQHandler
*	功能说明: 外部中断服务程序。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void EXTI0_IRQHandler(void)
{
    EXTIx_IRQHandler_cbfunc();
}
void TIM1_UP_IRQHandler(void)
{
    TIMx_Recv_IRQHandler_cbfunc();
}
void TIM2_IRQHandler(void)
{
    TIMx_Recv_Over_IRQHandler_cbfunc();
}

void TXD_High(void)
{
    GPIO_SetBits(GPIOG, GPIO_Pin_1);
}
void TXD_Low(void)
{
    GPIO_ResetBits(GPIOG, GPIO_Pin_1);
}
uint8_t RXD(void)
{
    return GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_0);
}           


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : 
*
* Arguments   : 
*
* Return      : 
*
*Note(s):
*********************************************************************************************************
*/
void SlUsart_Config(
    u32             baud,
    uint8_t         *recv_buffer
    )
{
//    SL_USART.baud           = baud;
//    SL_USART.EXTI_LineX     = EXTI_Line0;
//    SL_USART.TIMx_Send      = TIM3;
//    SL_USART.TIMx_Recv      = TIM1;
//    SL_USART.TIMx_RecvOver  = TIM2;
//    SL_USART.dataReceived   = recv_buffer;
    
    GPIO_Config();
    EXTI_Config();
    TIM1_Config();
    TIM2_Config();
    TIM3_Config();
    
    reg_sl_usart_txd_cbfunc( TXD_High,TXD_Low );
    reg_sl_usart_rxd_cbfunc( RXD );
}


