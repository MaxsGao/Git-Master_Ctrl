/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission ModuleConfig
*                                         无线数传模块配置
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/

#include "nrf24l01_conf.h"
#include "bsp_spi.h"

#if SYSTEM_SUPPORT_OS
    #include "includes.h"   //支持OS时，使用	  
#endif


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/


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
/**@brief   NRF24L01 CE引脚初始化
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
    
    GPIO_ResetBits(GPIO_NRF24L01_CE,GPIO_Pin_NRF24L01_CE);//上拉
}

/**@brief   Chip select signal output low.  片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void NRF24L01_CE_Enable(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CE = 1;
}

/**@brief   Chip select signal output high. 片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void NRF24L01_CE_Disable(void)
{
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CE = 0;
}

/**@brief   NRF24L01片选引脚初始化
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
    
    GPIO_ResetBits(GPIO_NRF24L01_CS,GPIO_Pin_NRF24L01_CS);//上拉
}

/**@brief   Chip select signal output low.  片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void NRF24L01_CS_Select(void)
{
//	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CS = 0;
}

/**@brief   Chip select signal output high. 片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void NRF24L01_CS_Deselect(void)
{
//	GPIO_SetBits(GPIOB,GPIO_Pin_6);
    Pout_NRF24L01_CS = 1;
}

/**@brief   NRF24L01 IRQ引脚初始化
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
    
    GPIO_ResetBits(GPIO_NRF24L01_IRQ,GPIO_Pin_NRF24L01_IRQ);//上拉
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟
    
    //GPIOG.6	  中断线以及中断初始化配置 下降沿触发 PG6
    GPIO_EXTILineConfig(GPIO_PortSource_NRF24L01_IRQ,GPIO_PinSource_NRF24L01_IRQ);
  	EXTI_InitStructure.EXTI_Line    = EXTI_Line_NRF24L01_IRQ;
  	EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器
    
	NVIC_InitStructure.NVIC_IRQChannel                      = EXTI4_IRQn; //使能按键PG6所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority    = 0x00;	        //抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority           = 0x00;         //子优先级0 
  	NVIC_InitStructure.NVIC_IRQChannelCmd                   = ENABLE;       //使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	                                    //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

void NRF24L01_IRQ_Trigger(void)
{
    EXTI_GenerateSWInterrupt(EXTI_Line_NRF24L01_IRQ);
}

void EXTI4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    if( EXTI_GetITStatus(EXTI_Line_NRF24L01_IRQ) != RESET )
    {
        EXTI_ClearITPendingBit(EXTI_Line_NRF24L01_IRQ);
        NRF24L01_IRQ_oper();
    }
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
#else
    INTX_ENABLE();
#endif
}

/**@brief   SD卡初始化的时候,需要低速
	*@param   None
	*@ret     None
	*/
void NRF24L01_SetSpeed(void)
{
// 	SPI1_SetSpeed(SPI_BaudRatePrescaler_16);//设置到低速模式
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
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
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PB12上拉 防止W25X的干扰
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	//初始化指定IO
    GPIO_SetBits(GPIOB,GPIO_Pin_12);//上拉
    
    NRF24L01_CE_Config();
    NRF24L01_CS_Config();
    NRF24L01_IRQ_Config();
    
    SPI1_Config();    		//初始化SPI
    SPI_Cmd(SPI1, DISABLE); // SPI外设不使能
    
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//SPI主机
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;		//时钟悬空低
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;	//数据捕获于第1个时钟沿
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由软件控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//定义波特率预分频的值:波特率预分频值为16
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
    SPI_Init(SPI1, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

    SPI_Cmd(SPI1, ENABLE); //使能SPI外设

    NRF24L01_CE_Disable();  //使能24L01
    NRF24L01_CS_Deselect(); //SPI片选取消
    
    reg_nrf_ce_cbfunc   ( NRF24L01_CE_Enable, NRF24L01_CE_Disable );
    reg_nrf_cris_cbfunc ( SPI_CrisEnter, SPI_CrisExit );                        ///> 注册临界区函数
    reg_nrf_cs_cbfunc   ( NRF24L01_CS_Select, NRF24L01_CS_Deselect );             ///> 注册SPI片选函数
    reg_nrf_spi_cbfunc  ( NRF24L01_SetSpeed, SPI1_ReadByte, SPI1_WriteByte );   ///> 注册SPI速率设置及SPI读写函数
    reg_nrf_irq_cbfunc  ( NRF24L01_IRQ_Trigger );
    
    /* 发送相关配置 */
    NRF24L01_TX_Init( pTXUnitArray,array_size );
    
    /* 接收相关配置 */
    reg_nrf_rx_handle_cbfunc( rx_handle );
}

