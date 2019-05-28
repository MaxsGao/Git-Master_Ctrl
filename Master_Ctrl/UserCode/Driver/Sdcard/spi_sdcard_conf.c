/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         ���ζ��нṹ
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "spi_sdcard_conf.h"
#include "bsp_spi.h"


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
/**@brief   SD��Ƭѡ���ų�ʼ��
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

/**@brief   Chip select signal output low.  Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void SpiSdcard_CS_Select(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_6);
}

/**@brief   Chip select signal output high. Ƭѡ�ź�����͵�ƽ
	*@param   None
	*@ret     None
	*/
void SpiSdcard_CS_Deselect(void)
{
	GPIO_SetBits(GPIOB,GPIO_Pin_6);
}

/**@brief   SD����ʼ����ʱ��,��Ҫ����
	*@param   None
	*@ret     None
	*/
void SpiSdcard_SpeedLow(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_256);//���õ�����ģʽ
}

/**@brief   SD������������ʱ��,���Ը�����
	*@param   None
	*@ret     None
	*/
void SpiSdcard_SpeedHigh(void)
{
 	SPI3_SetSpeed(SPI_BaudRatePrescaler_2);//���õ�����ģʽ
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
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
    
    reg_spi_sdcard_cris_cbfunc( SPI_CrisEnter, SPI_CrisExit );                                          ///> ע���ٽ�������
    reg_spi_sdcard_cs_cbfunc(   SpiSdcard_CS_Select, SpiSdcard_CS_Select );                             ///> ע��SD��Ƭѡ����
    reg_spi_sdcard_spi_cbfunc(  SpiSdcard_SpeedLow,SpiSdcard_SpeedHigh,SPI3_ReadByte, SPI3_WriteByte ); ///> ע��SPI�������ü�SPI��д����
}

