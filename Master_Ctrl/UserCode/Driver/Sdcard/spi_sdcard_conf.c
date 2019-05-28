/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         环形队列结构
* File : spi_sdcard_conf.h
* By   : Bear
* Date : 2018/11/19
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
#include "spi_sdcard_conf.h"
#include "bsp_spi.h"


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
/**@brief   SD卡片选引脚初始化
	*@param   None
	*@ret     None
	*/
void SpiSdcard_CS_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_Init( GPIOB,&GPIO_InitStructure );
}

/**@brief   Chip select signal output low.  片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void SpiSdcard_CS_Select(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

/**@brief   Chip select signal output high. 片选信号输出低电平
	*@param   None
	*@ret     None
	*/
void SpiSdcard_CS_Deselect(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

/**@brief   SD卡初始化的时候,需要低速
	*@param   None
	*@ret     None
	*/
void SpiSdcard_SpeedLow(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_256);//设置到低速模式
}

/**@brief   SD卡正常工作的时候,可以高速了
	*@param   None
	*@ret     None
	*/
void SpiSdcard_SpeedHigh(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_2);//设置到高速模式
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        SpiSdcard_Config()
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
void SpiSdcard_Config(void)
{
    SPI3_Config();
    SpiSdcard_CS_Config();
    
    reg_spi_sdcard_cris_cbfunc( SPI_CrisEnter, SPI_CrisExit );                                          ///> 注册临界区函数
    reg_spi_sdcard_cs_cbfunc(   SpiSdcard_CS_Select, SpiSdcard_CS_Select );                             ///> 注册SD卡片选函数
    reg_spi_sdcard_spi_cbfunc(  SpiSdcard_SpeedLow,SpiSdcard_SpeedHigh,SPI3_ReadByte, SPI3_WriteByte ); ///> 注册SPI速率设置及SPI读写函数
}

