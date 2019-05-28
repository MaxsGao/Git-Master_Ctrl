/*
*********************************************************************************************************
*
*
*                                       Message Management
*                                         文件信息管理
* File : MsgManage.c
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : 需要管理的信息文件有三类：存储自身配置信息的文件、存储功能子节点信息的文件、存储模式信息的文件
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
#include "MsgManage.h"


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
*                                       VARIABLES           变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
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
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  配置信息的加载
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confload(void)
{}
    
/**
  * @brief  配置信息的更新
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confupdate(void)
{}

/**
  * @brief  配置信息的查找
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_confselect(void)
{}

/**
  * @brief  节点信息的加载
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeload(void)
{}

/**
  * @brief  节点信息的更新
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeupdata(void)
{}

/**
  * @brief  节点信息的查找
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_nodeselect(void)
{}

/**
  * @brief  模式信息的加载
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeload(void)
{}

/**
  * @brief  模式信息的更新
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeupdate(void)
{}

/**
  * @brief  模式信息的查找
  * @param	None
  * @note   None
  * @retval	None
  */
void msg_modeselect(void)
{}




