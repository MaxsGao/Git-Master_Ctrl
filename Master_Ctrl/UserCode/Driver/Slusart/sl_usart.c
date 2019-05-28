/*
*********************************************************************************************************
*
*
*                                       Simulation Usart
*                                          ģ�⴮��
* File : sl_usart.c
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
#include "sl_usart.h"


/*
*********************************************************************************************************
*                                       LOCAL FUNCTION STATEMENT  ��̬��������
*********************************************************************************************************
*/
void    sl_usart_txd_high   (void)              {};
void    sl_usart_txd_low    (void)              {};
uint8_t sl_usart_rxd        (void)              {return 0;};


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#define DelayTime       8
#define _High() GPIO_SetBits(GPIOG, GPIO_Pin_1)
#define _LOW()  GPIO_ResetBits(GPIOG, GPIO_Pin_1)
#define _RXD()  GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_0)


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
/**
 * @\ref _SL_USART instance
 */
_SL_USART SL_USART;

/**
 * @\ref ״̬��
 */
uint8_t RecvBitStat = COM_STOP_BIT;

/**
 * @\ref ����״̬
 */
//bit15��	������ɱ�־
//bit14~0��	���յ�����Ч�ֽ���Ŀ
uint16_t SL_USART_RX_STA=0;


uint8_t dataReceived[100];
uint8_t Logo[]={"Hello, C!\r\n"};
__IO uint8_t receivedFlag=0;
__IO uint8_t receivedNum=0, tmpreceivedNum=0;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**@brief   ����Ӳ��ƽ̨��ʼ������
	*@param   None
	*@ret     None
	*/
void SendingDelay(__IO uint32_t nTime)
{
	uint16_t tmp;
//    uint32_t nTime = 1000000/(SL_USART.baud);
    
	tmp = TIM_GetCounter( TIM3 );
	
	if(tmp + nTime <= 65535)
    {
		while( (TIM_GetCounter( TIM3 ) - tmp) < nTime );
    }else
	{
		TIM_SetCounter( TIM3, 0 );
		while( TIM_GetCounter( TIM3 ) < nTime );
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
* Description : To initialize the ring queue.    ��ʼ�����ζ���
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               pbuf     pointer to the buffer(an array);             ָ���Զ���Ļ�����(ʵ�ʾ��Ǹ�����)
*               bufSize  the Size of the buffer;                      �������Ĵ�С;
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                           RQ_ERR_NONE                                       
*                           RQ_ERR_SIZE_ZERO
*                           RQ_ERR_POINTER_NULL
*
* Return      : the pointer to the ring queue control block;        ����ָ���ζ��п��ƿ��ָ��
*               0x00 if any error;                                  ����������򷵻�NULL
*
*Note(s):
*********************************************************************************************************
*/
void reg_sl_usart_txd_cbfunc ( VoidFuncPtr txd_high, VoidFuncPtr txd_low )
{
    if( !txd_high || !txd_low )
    {
        SL_USART.TXD_high  = sl_usart_txd_high;
        SL_USART.TXD_low   = sl_usart_txd_low;
    }else
    {
        SL_USART.TXD_high  = txd_high;
        SL_USART.TXD_low   = txd_low;
    }
}
void reg_sl_usart_rxd_cbfunc ( U8FuncPtr rxd )
{
    if( !rxd )
    {
        SL_USART.RXD = sl_usart_rxd;
    }else
    {
        SL_USART.RXD = rxd;
    }
}
void SL_USART_SendOneByte( uint8_t datatoSend )
{
    uint8_t i,tmp;
    
    /* Start bit */
//    SL_USART.TXD_low();
    _LOW();
    SendingDelay(DelayTime);
    
    /* Data bit */
    for( i=0;i<8;i++ )
    {
        tmp = ( datatoSend >> i ) & 0x01;
        
        if( tmp==0 )
        {
            _LOW();    ///< 0
            SendingDelay(DelayTime);
        }else
        {
            _High();
            SendingDelay(DelayTime);
        }
    }
    
    /* Stop bit */
    _High();
    SendingDelay(DelayTime);
}

void SL_USART_SendBytes( uint8_t *send_datas,uint32_t send_len )
{
    int cnt;
    for( cnt=0;cnt<send_len;cnt++ )
    {
        SL_USART_SendOneByte( send_datas[cnt] );
    }
}

uint32_t SL_USART_RecvBytes(uint8_t *readBuffer,uint32_t Buf_len)
{
    int i,cnt=0;
    
//    if( SL_USART_RX_STA&0x8000 )
//    {
//        /* ���ؽ��յ������� */
//        for(cnt = 0; cnt < (SL_USART_RX_STA&0x7FFF); cnt++)
//            readBuffer[cnt] = SL_USART.dataReceived[cnt];

//        /* ��ս��ջ��� */
//        for(i = 0; i < (SL_USART_RX_STA&0x7FFF); i++)
//            SL_USART.dataReceived[i] = 0;

//        /* �������ݳ������� */
//        SL_USART_RX_STA = 0;
//    }
//    return cnt;
    
    
		if(receivedFlag)
		{
			/* ���������ɱ�־λ */
			receivedFlag = 0;

			/* ���ؽ��յ������� */
			for(cnt = 0; cnt < tmpreceivedNum; cnt++)
                SL_USART_SendOneByte(dataReceived[cnt]);
//				readBuffer[cnt] = dataReceived[cnt];

			/* ��ս��ջ��� */
			for(i = 0; i < tmpreceivedNum; i++)
				dataReceived[i] = 0;

			/* ����ģ��Ի����� */
//			PrintfAnswer();

			/* �������ݳ������� */
			tmpreceivedNum = 0;
		}
        return cnt;
}


void EXTIx_IRQHandler_cbfunc(void)
{
//    if( EXTI_GetITStatus(SL_USART.EXTI_LineX)!=RESET )
//    {
//        if( !_RXD() )
//        {
//            if( RecvBitStat==COM_STOP_BIT )
//            {
//                RecvBitStat = COM_START_BIT;
//                
//                for(int i=0;i<10;i++);
//                
//                TIM_SetCounter( SL_USART.TIMx_Recv, 0 );
//                TIM_Cmd( SL_USART.TIMx_Recv,ENABLE );		//����TIM1
//                
//                TIM_Cmd(SL_USART.TIMx_RecvOver,DISABLE);		//�ر�TIM2
//                TIM_SetCounter(SL_USART.TIMx_RecvOver, 0);
//                TIM_Cmd(SL_USART.TIMx_RecvOver,ENABLE);		//����TIM2
//            }
//        }
//        EXTI_ClearITPendingBit( SL_USART.EXTI_LineX );
//    }
    
	if(EXTI_GetFlagStatus(EXTI_Line0) != RESET)
	{
		/* Disable the Selected IRQ Channels -------------------------------------*/
        NVIC_DisableIRQ(EXTI0_IRQn);
//    	NVIC->ICER[EXTI0_IRQn >> 0x05] =
//      		(uint32_t)0x01 << (EXTI0_IRQn & (uint8_t)0x1F);		

		receivedFlag = 0;

		TIM_SetCounter(TIM1, 0);
		TIM_Cmd(TIM1,ENABLE);		//����TIM1

		TIM_Cmd(TIM2,DISABLE);		//�ر�TIM2
		TIM_SetCounter(TIM2, 0);
		TIM_Cmd(TIM2,ENABLE);		//����TIM2
		
		EXTI_ClearITPendingBit(EXTI_Line0);
	
	}
}

void TIMx_Recv_IRQHandler_cbfunc(void)
{
//    if( TIM_GetITStatus(SL_USART.TIMx_Recv,TIM_IT_Update)!=RESET )
//    {
//        TIM_ClearITPendingBit( SL_USART.TIMx_Recv,TIM_IT_Update );
//        
//        RecvBitStat++;
//        if( RecvBitStat==COM_STOP_BIT )
//        {
//			SL_USART_RX_STA++;
//            
//			TIM_Cmd(SL_USART.TIMx_Recv,DISABLE);        //�ر�TIM1
//		
//			TIM_Cmd(SL_USART.TIMx_RecvOver,DISABLE);    //�ر�TIM2
//			TIM_SetCounter(SL_USART.TIMx_RecvOver, 0);
//			TIM_Cmd(SL_USART.TIMx_RecvOver,ENABLE);		//����TIM2
//            return;
//        }
//        if( _RXD() )
//        {
//            SL_USART.dataReceived[SL_USART_RX_STA&0x7FFF] |= (1<<(RecvBitStat-1));
//        }else
//        {
//            SL_USART.dataReceived[SL_USART_RX_STA&0x7FFF] &= ~(1<<(RecvBitStat-1));
//        }
//    }

	uint8_t tmp;
	static uint8_t i;

	if(TIM_GetFlagStatus(TIM1, TIM_FLAG_Update) != RESET)
	{

		tmp = _RXD();
		
		if(tmp == 1)
			dataReceived[receivedNum] |= (1 << i); 
	
		i++;
				
		if(i >= 8)
		{
			i = 0;
			receivedNum++; 
	
	    	/* Enable the Selected IRQ Channels --------------------------------------*/
			EXTI_ClearITPendingBit(EXTI_Line0);
            NVIC_EnableIRQ(EXTI0_IRQn);
//	    	NVIC->ISER[EXTI0_IRQn >> 0x05] =
//	    		(uint32_t)0x01 << (EXTI0_IRQn & (uint8_t)0x1F);
	
			TIM_Cmd(TIM1,DISABLE);		//�ر�TIM1
		
			TIM_Cmd(TIM2,DISABLE);		//�ر�TIM2
			TIM_SetCounter(TIM2, 0);
			TIM_Cmd(TIM2,ENABLE);		//����TIM2
		}

		TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
	}
}

void TIMx_Recv_Over_IRQHandler_cbfunc(void)
{
//	if(TIM_GetFlagStatus(SL_USART.TIMx_RecvOver, TIM_FLAG_Update) != RESET)
//	{
//		if( (SL_USART_RX_STA&0x7FFF) != 0 )
//		{
////			SL_USART.tmpreceivedNum = SL_USART.receivedNum;
////			SL_USART.receivedNum	= 0;
////			SL_USART.receivedFlag = 1;
//            SL_USART_RX_STA |= 0x8000;
//		}

//		TIM_ClearITPendingBit(SL_USART.TIMx_RecvOver, TIM_FLAG_Update);
//	}
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



