/*
*********************************************************************************************************
*
*
*                                       ESP8266 Moudle Config
*                                         ESP8266模块配置
* File : esp_conf.c
* By   : 
* Date : 2019/01/11
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
#include "esp_conf.h"


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
/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : To initialize the ring queue.    初始化环形队列
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
*               pbuf     pointer to the buffer(an array);             指向自定义的缓冲区(实际就是个数组)
*               bufSize  the Size of the buffer;                      缓冲区的大小;
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                           RQ_ERR_NONE                                       
*                           RQ_ERR_SIZE_ZERO
*                           RQ_ERR_POINTER_NULL
*
* Return      : the pointer to the ring queue control block;        返回指向环形队列控制块的指针
*               0x00 if any error;                                  如果出错了则返回NULL
*
*Note(s):
*********************************************************************************************************
*/

