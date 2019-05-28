/*
*********************************************************************************************************
*
*
*                                       Communicate with function Node
*                                         与功能子节点通信
* File : CommNode.c
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : 此功能是在2.4G无线通信的基础上来实现与功能子节点的通信，本程序文件主要定义了数据发送，数据接收和数
*   据处理的API接口
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
#include "CommNode.h"
#include "MsgManage.h"
#include "nrf24l01.h"


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
  * @brief  初始化与功能子节点通信相关的内容
  * @param  None
  * @note   主控器节点与各功能子节点是通过2.4G无线通信模块NRF24L01实现的
  * @retval None
  */
int CommNodeInit(void)
{
    /* 配置2.4G无线通信模块 */
    
    /* 初始化与功能节点通信相关的变量或标识符 */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNodeSendData(
    uint8_t     nodeID,
    uint8_t*    pBuff,
    uint16_t    len
)
{
    /* 根据nodeID查找其对应的通信信道 */
    /* 调用2.4G模块的数据发送函数，将数据发送到指定得节点 */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNodeDealData()
{
    return 0;
}

