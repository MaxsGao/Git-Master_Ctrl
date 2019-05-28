/*
*********************************************************************************************************
*
*
*                                       Used to debug related file
*                                         用于调试相关的文件
* File : Debug.h
* By   : MaxsGao
* Date : 2019/05/05
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __DEBUG_H
#define   __DEBUG_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdint.h>
#include "bsp_uart.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //参数作为输入
#endif
#ifndef  __OUT_
#define  __OUT_      //参数作为输出
#endif

/**
 * @brief 串口配置相关的宏定义
 * @{
 */
#define UartConfigToDebug   USART3_Config
#define UartIDToDebug       USART3
#define UartPortToDebug     UART_PORT_3
/**
  * @}
  */
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/

/** 
 * @brief  CAN init structure definition
 */
//typedef struct
//{
//  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum. 
//                                 It ranges from 1 to 1024. */
//  
//  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
//                                 This parameter can be a value of 
//                                @ref CAN_operating_mode */
//} CAN_InitTypeDef;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
int DebugInit(void);


#ifdef __cplusplus
}
#endif

#endif
