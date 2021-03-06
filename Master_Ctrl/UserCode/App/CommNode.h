/*
*********************************************************************************************************
*
*
*                                       Communicate with function Node
*                                         与功能子节点通信
* File : CommNode.h
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
#ifndef   __COMMNODE_H
#define   __COMMNODE_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/


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
 * @brief Peer IP register address
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


#ifdef __cplusplus
}
#endif

#endif
