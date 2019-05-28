/*
*********************************************************************************************************
*
*
*                                       Message Management
*                                         文件信息管理
* File : MsgManage.h
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : 需要管理的信息文件有三类：存储自身配置信息的文件、存储功能子节点信息的文件、存储模式信息的文件
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __MSGMANGAGE_H
#define   __MSGMANGAGE_H

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
void msg_confload(void);
void msg_confupdate(void);
void msg_confselect(void);

void msg_nodeload(void);
void msg_nodeupdata(void);
void msg_nodeselect(void);

void msg_modeload(void);
void msg_modeupdate(void);
void msg_modeselect(void);



#ifdef __cplusplus
}
#endif

#endif
