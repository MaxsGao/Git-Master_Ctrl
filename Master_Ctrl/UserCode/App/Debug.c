/*
*********************************************************************************************************
*
*
*                                       Used to debug related file
*                                         ���ڵ�����ص��ļ�
* File : Debug.c
* By   : MaxsGao
* Date : 2019/05/05
* version : V1.0
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
#include "Debug.h"


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

///** @defgroup SPI_Private_Defines
//  * @{
//  */

///* SPI SPE mask */
//#define CR1_SPE_Set          ((uint16_t)0x0040)
//#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

///* I2S I2SE mask */
//#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
//#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

///**
//  * @}
//  */


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
static uint32_t  UartBaudToDebug;   ///< ���Դ��ڵ�ͨ�Ų�����

uint8_t gUartSendBufToDebug[256];  ///< ���ڴ���1��ʼ��ʱ�ķ��ͻ�����


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  * @param  SPI_I2S_FLAG: specifies the SPI flag to clear. 
  *   This function clears only CRCERR flag.
  * @note
  *   - OVR (OverRun error) flag is cleared by software sequence: a read 
  *     operation to SPI_DR register (SPI_I2S_ReceiveData()) followed by a read 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - UDR (UnderRun error) flag is cleared by a read operation to 
  *     SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - MODF (Mode Fault) flag is cleared by software sequence: a read/write 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()) followed by a 
  *     write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int DebugInit(void)
{
    UartBaudToDebug = 9600;
    
    /* Config Debug Uart */
    Uart_Config(
        UartConfigToDebug,
        UartPortToDebug,
        UART_TTL_EN,
        UartBaudToDebug,
        gUartSendBufToDebug,
        sizeof(gUartSendBufToDebug),
        0
    );
    
    return 0;
}

