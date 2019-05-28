/*
*********************************************************************************************************
*
*
*                                       Simulation Usart
*                                          ģ�⴮��
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include "sl_usart_conf.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**@brief   ����PG1ΪTXD��PG0��RXD
	*@param   None
	*@ret     None
	*/
static void GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* ʹ�� GPIOG clock */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOG,ENABLE  );     ///> ��ʼ��λRCCʱ�Ӹ�λ
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_GPIOG,DISABLE );     ///> ֹͣ��λRCCʱ�Ӹ�λ
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOG, ENABLE );     ///> ʹ��RCCʱ��

	/* ��ֹ�ϵ������� */
	GPIO_SetBits(GPIOG, GPIO_Pin_1);

	/* ����PG1Ϊ������� */
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	/* ����PG0Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
}

/**@brief   ����PG0�ϵ��½��ش����ⲿ�ж�
	*@param   None
	*@ret     None
	*/
static void EXTI_Config(void)
{
  	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the AFIO Clock */
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, ENABLE );      ///> ��ʼʱ�Ӹ�λ
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_AFIO, DISABLE  );    ///> ֹͣʱ�Ӹ�λ
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

/**@brief   ����TIM1
	*@param   None
	*@ret     None
	*/
static void TIM1_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, ENABLE );      ///> ��ʼʱ�Ӹ�λ
    RCC_APB2PeriphResetCmd( RCC_APB2Periph_TIM1, DISABLE  );    ///> ֹͣʱ�Ӹ�λ
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_TIM1, ENABLE  );

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period        = 8;
  	TIM_TimeBaseStructure.TIM_Prescaler     = 72-1;
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	
	/* ����TIM1���ж� */
    TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);  
    
    /* ѡ��TIM1���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
    
	/* ��ռʽ�ж����ȼ�����Ϊ0 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    
	/* ��Ӧʽ�ж����ȼ�����Ϊ0 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
	/* ʹ���ж� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


/**@brief   ����TIM2
	*@param   None
	*@ret     None
	*/
static void TIM2_Config(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM2, ENABLE );      ///> ��ʼʱ�Ӹ�λ
    RCC_APB1PeriphResetCmd( RCC_APB1Periph_TIM2, DISABLE  );    ///> ֹͣʱ�Ӹ�λ
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM2, ENABLE  );

  	/* Time base configuration */
  	TIM_TimeBaseStructure.TIM_Period        = 1000 - 1;
  	TIM_TimeBaseStructure.TIM_Prescaler     = 72 - 1;
 	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;

  	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
	
	/* ����TIM2���ж� */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);      
        
    /* ѡ��TIM2���ж�ͨ�� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;      
    
	/* ��ռʽ�ж����ȼ�����Ϊ0 */
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    
	/* ��Ӧʽ�ж����ȼ�����Ϊ0 */
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    
	/* ʹ���ж� */
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/**@brief   ����TIM3
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
*	�� �� ��: EXTI15_10_IRQHandler
*	����˵��: �ⲿ�жϷ������
*	��    �Σ���
*	�� �� ֵ: ��
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
*                                       API FUNCTIONS       API�ӿں���
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


