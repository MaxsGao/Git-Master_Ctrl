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
/* ���������������̷������� */					
#define EXAMPLE_NAME	"GPIO��ģ�⴮��"
#define EXAMPLE_DATE	"2012-11-12"
#define DEMO_VER		"1.0"

#define TXD_high()		GPIO_SetBits(GPIOG, GPIO_Pin_1)
#define TXD_low()		GPIO_ResetBits(GPIOG, GPIO_Pin_1)
#define RXD()           GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_0)

#define BaudRateUsed	115200
#define SendingDelay	8


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
uint8_t dataReceived[100];
__IO uint8_t receivedFlag;
__IO uint8_t receivedNum, tmpreceivedNum;


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
  	TIM_TimeBaseStructure.TIM_Period        = SendingDelay;
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

static void Delay_Us(__IO uint32_t nTime)
{
	uint16_t tmp;

	tmp = TIM_GetCounter(TIM3);
	
	if(tmp + nTime <= 65535)
		while( (TIM_GetCounter(TIM3) - tmp) < nTime );
	else
	{
		TIM_SetCounter(TIM3, 0);
		while( TIM_GetCounter(TIM3) < nTime );
	}
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
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/* Disable the Selected IRQ Channels -------------------------------------*/
    	NVIC->ICER[EXTI0_IRQn >> 0x05] =
      		(uint32_t)0x01 << (EXTI0_IRQn & (uint8_t)0x1F);		

		receivedFlag = 0;

		TIM_SetCounter(TIM1, 0);
		TIM_Cmd(TIM1,ENABLE);		//����TIM1

		TIM_Cmd(TIM2,DISABLE);		//�ر�TIM2
		TIM_SetCounter(TIM2, 0);
		TIM_Cmd(TIM2,ENABLE);		//����TIM2
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}
void TIM1_UP_IRQHandler(void)
{
	uint8_t tmp;
	static uint8_t i;

	if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
	{

		tmp = RXD();
		
		if(tmp == 1)
			dataReceived[receivedNum] |= (1 << i); 
	
		i++;
				
		if(i >= 8)
		{
			i = 0;
			receivedNum++; 
	
	    	/* Enable the Selected IRQ Channels --------------------------------------*/
			EXTI_ClearITPendingBit(EXTI_Line0);
	    	NVIC->ISER[EXTI0_IRQn >> 0x05] =
	    		(uint32_t)0x01 << (EXTI0_IRQn & (uint8_t)0x1F);
	
			TIM_Cmd(TIM1,DISABLE);		//�ر�TIM1
		
			TIM_Cmd(TIM2,DISABLE);		//�ر�TIM2
			TIM_SetCounter(TIM2, 0);
			TIM_Cmd(TIM2,ENABLE);		//����TIM2
		}

		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	}
}
void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2, TIM_FLAG_Update) != RESET)
	{
		if(receivedNum != 0)
		{
			tmpreceivedNum = receivedNum;
			receivedNum	= 0;
			receivedFlag = 1;
		}

		TIM_ClearITPendingBit(TIM2, TIM_FLAG_Update);
	}
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
    GPIO_Config();
    EXTI_Config();
    TIM1_Config();
    TIM2_Config();
    TIM3_Config();
}

void SlUsart_SendOneByte(uint8_t datatoSend)
{
	uint8_t i, tmp;

	/* Start bit */
	TXD_low();
	Delay_Us(SendingDelay);	

	for(i = 0; i < 8; i++)
	{
		tmp	= (datatoSend >> i) & 0x01;

		if(tmp == 0)
		{
			TXD_low();
			Delay_Us(SendingDelay);	//0		
		}
		else
		{
			TXD_high();
			Delay_Us(SendingDelay);	//1		
		}	
	}
	
	/* Stop bit */
	TXD_high();
	Delay_Us(SendingDelay);	
}
uint32_t SlUsart_RecvBytes( uint8_t *recvBuffer,uint32_t Buf_len )
{
    int i,cnt=0;
    if( receivedFlag )
    {
        /* ���������ɱ�־λ */
        receivedFlag = 0;

        /* ���ؽ��յ������� */
        for(cnt = 0; cnt < tmpreceivedNum; cnt++){
            recvBuffer[cnt] = dataReceived[cnt];
//            SlUsart_SendOneByte(dataReceived[cnt]);
        }

        /* ��ս��ջ��� */
        for(i = 0; i < tmpreceivedNum; i++){
            dataReceived[i] = 0;
        }
        
        /* �������ݳ������� */
        tmpreceivedNum = 0;
    }
    
    return cnt;
}

