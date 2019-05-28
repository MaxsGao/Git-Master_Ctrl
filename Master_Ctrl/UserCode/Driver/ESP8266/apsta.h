/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         环形队列结构
* File : Blank.h
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2017/10/16
* version : V1.1 
* NOTE(s) : 这段程序用来对一个给定的缓冲区进行模拟环形队列的管理
*                   程序本身不会自动分配缓冲区空间,用户需要自己负责分配空间,并且要保证不直接访问缓存区
*                   // 在某处分配内存空间
*                   RQTYPE buffer[BUFFER_SIZE];
*                   RING_QUEUE que,*ptr_que;
*                   unsigned char err;
*                   // 初始化
*                   ptr_que = RingQueueInit(&que,buffer,BUFFER_SIZE,&err);
*                   if(err == RQ_ERR_NONE  ){
*                     // 初始化成功,使用其他函数
*                   }
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*
*
*********************************************************************************************************
*/
#ifndef   __BLANK_H
#define   __BLANK_H

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

//  uint8_t CAN_SJW;          /*!< Specifies the maximum number of time quanta 
//                                 the CAN hardware is allowed to lengthen or 
//                                 shorten a bit to perform resynchronization.
//                                 This parameter can be a value of 
//                                 @ref CAN_synchronisation_jump_width */

//  uint8_t CAN_BS1;          /*!< Specifies the number of time quanta in Bit 
//                                 Segment 1. This parameter can be a value of 
//                                 @ref CAN_time_quantum_in_bit_segment_1 */

//  uint8_t CAN_BS2;          /*!< Specifies the number of time quanta in Bit 
//                                 Segment 2.
//                                 This parameter can be a value of 
//                                 @ref CAN_time_quantum_in_bit_segment_2 */
//  
//  FunctionalState CAN_TTCM; /*!< Enable or disable the time triggered 
//                                 communication mode. This parameter can be set 
//                                 either to ENABLE or DISABLE. */
//  
//  FunctionalState CAN_ABOM;  /*!< Enable or disable the automatic bus-off 
//                                  management. This parameter can be set either 
//                                  to ENABLE or DISABLE. */

//  FunctionalState CAN_AWUM;  /*!< Enable or disable the automatic wake-up mode. 
//                                  This parameter can be set either to ENABLE or 
//                                  DISABLE. */

//  FunctionalState CAN_NART;  /*!< Enable or disable the no-automatic 
//                                  retransmission mode. This parameter can be 
//                                  set either to ENABLE or DISABLE. */

//  FunctionalState CAN_RFLM;  /*!< Enable or disable the Receive FIFO Locked mode.
//                                  This parameter can be set either to ENABLE 
//                                  or DISABLE. */

//  FunctionalState CAN_TXFP;  /*!< Enable or disable the transmit FIFO priority.
//                                  This parameter can be set either to ENABLE 
//                                  or DISABLE. */
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
