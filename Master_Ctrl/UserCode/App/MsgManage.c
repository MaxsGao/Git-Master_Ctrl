/*
*********************************************************************************************************
*
*
*                                       Message Management
*                                         �ļ���Ϣ����
* File : MsgManage.c
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : ��Ҫ�������Ϣ�ļ������ࣺ�洢����������Ϣ���ļ����洢�����ӽڵ���Ϣ���ļ����洢ģʽ��Ϣ���ļ�
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
#include "MsgManage.h"


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
  * @brief  ������Ϣ�ļ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confload(void)
{}
    
/**
  * @brief  ������Ϣ�ĸ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confupdate(void)
{}

/**
  * @brief  ������Ϣ�Ĳ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confselect(void)
{}

/**
  * @brief  �ڵ���Ϣ�ļ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeload(void)
{}

/**
  * @brief  �ڵ���Ϣ�ĸ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeupdata(void)
{}

/**
  * @brief  �ڵ���Ϣ�Ĳ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeselect(void)
{}

/**
  * @brief  ģʽ��Ϣ�ļ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeload(void)
{}

/**
  * @brief  ģʽ��Ϣ�ĸ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeupdate(void)
{}

/**
  * @brief  ģʽ��Ϣ�Ĳ���
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeselect(void)
{}




