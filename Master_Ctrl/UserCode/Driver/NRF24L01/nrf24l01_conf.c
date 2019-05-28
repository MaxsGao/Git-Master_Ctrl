/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission ModuleConfig
*                                         ��������ģ������
* File : nrf24l01_conf.c
* By   : 
* Date : 2018/12/6
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

#include "nrf24l01_conf.h"
#include "bsp_spi.h"

#if SYSTEM_SUPPORT_OS
    #include "includes.h"   //֧��OSʱ��ʹ��	  
#endif


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/


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
/**@brief   NRF24L01 CE���ų�ʼ��
	*@param   None
	*@ret     None
	*/
void NRF24L01_CE_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_NRF24L01_CE,ENABLE );
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_NRF24L01_CE;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init( GPIO_NRF24L01_CE,&GPIO_InitStructure );
    
    GPIO_ResetBits(GPIO_NRF24L01_CE,GPIO_Pin_NRF24L01_CE);//����
}

/**@brief   Chip select signal output low.  Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void NRF24L01_CE_Enable(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CE = 1;
}

/**@brief   Chip select signal output high. Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void NRF24L01_CE_Disable(void)
{
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CE = 0;
}

/**@brief   NRF24L01Ƭѡ���ų�ʼ��
	*@param   None
	*@ret     None
	*/
void NRF24L01_CS_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_NRF24L01_CS,ENABLE );
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_NRF24L01_CS;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init( GPIO_NRF24L01_CS,&GPIO_InitStructure );
    
    GPIO_ResetBits(GPIO_NRF24L01_CS,GPIO_Pin_NRF24L01_CS);//����
}

/**@brief   Chip select signal output low.  Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void NRF24L01_CS_Select(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CS = 0;
}

/**@brief   Chip select signal output high. Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void NRF24L01_CS_Deselect(void)
{
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CS = 1;
}

/**@brief   NRF24L01 IRQ���ų�ʼ��
	*@param   None
	*@ret     None
	*/
void NRF24L01_IRQ_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
    
    RCC_APB2PeriphClockCmd( RCC_NRF24L01_IRQ,ENABLE );
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_NRF24L01_IRQ;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init( GPIO_NRF24L01_IRQ,&GPIO_InitStructure );
    
    GPIO_ResetBits(GPIO_NRF24L01_IRQ,GPIO_Pin_NRF24L01_IRQ);//����
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��
    
    //GPIOG.6	  �ж����Լ��жϳ�ʼ������ �½��ش��� PG6
    GPIO_EXTILineConfig(GPIO_PortSource_NRF24L01_IRQ,GPIO_PinSource_NRF24L01_IRQ);
  	EXTI_InitStructure.EXTI_Line    = EXTI_Line_NRF24L01_IRQ;
  	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���
    
	NVIC_InitStructure.NVIC_IRQChannel                      = EXTI4_IRQn; //ʹ�ܰ���PG6���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0x00;	        //��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0x00;         //�����ȼ�0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;       //ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	                                    //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
}

void NRF24L01_IRQ_Trigger(void)
{
    EXTI_GenerateSWInterrupt(EXTI_Line_NRF24L01_IRQ);
}

void EXTI4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    if( EXTI_GetITStatus(EXTI_Line_NRF24L01_IRQ) != RESET )
    {
        EXTI_ClearITPendingBit(EXTI_Line_NRF24L01_IRQ);
        NRF24L01_IRQ_oper();
    }
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}

/**@brief   SD����ʼ����ʱ��,��Ҫ����
	*@param   None
	*@ret     None
	*/
void NRF24L01_SetSpeed(void)
{
// 	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);//���õ�����ģʽ
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        NRF24L01_Config()
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
void NRF24L01_Config(
    _NRF_TX_QUEUE_UNIT* pTXUnitArray,
    uint16_t            array_size,
     NrfRxHandle        rx_handle
    )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12���� ��ֹW25X�ĸ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	//��ʼ��ָ��IO
    GPIO_SetBits(GPIOB,GPIO_Pin_12);//����
    
    NRF24L01_CE_Config();
    NRF24L01_CS_Config();
    NRF24L01_IRQ_Config();
    
    SPI1_Config();    		//��ʼ��SPI
    SPI_Cmd(SPI1, DISABLE); // SPI���費ʹ��
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI����Ϊ˫��˫��ȫ˫��
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI����
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//���ͽ���8λ֡�ṹ
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//ʱ�����յ�
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//���ݲ����ڵ�1��ʱ����
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź����������
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//���ݴ����MSBλ��ʼ
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
    SPI_Init(SPI1, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

    SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����

    NRF24L01_CE_Disable();  //ʹ��24L01
    NRF24L01_CS_Deselect(); //SPIƬѡȡ��
    
    reg_nrf_ce_cbfunc   ( NRF24L01_CE_Enable, NRF24L01_CE_Disable );
    reg_nrf_cris_cbfunc ( SPI_CrisEnter, SPI_CrisExit );                        ///> ע���ٽ�������
    reg_nrf_cs_cbfunc   ( NRF24L01_CS_Select, NRF24L01_CS_Deselect );             ///> ע��SPIƬѡ����
    reg_nrf_spi_cbfunc  ( NRF24L01_SetSpeed, SPI1_ReadByte, SPI1_WriteByte );   ///> ע��SPI�������ü�SPI��д����
    reg_nrf_irq_cbfunc  ( NRF24L01_IRQ_Trigger );
    
    /* ����������� */
    NRF24L01_TX_Init( pTXUnitArray,array_size );
    
    /* ����������� */
    reg_nrf_rx_handle_cbfunc( rx_handle );
}

