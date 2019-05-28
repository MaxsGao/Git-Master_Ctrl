/*
*********************************************************************************************************
*
*
*                                       QueueStruct
*                                         队列结构
* File : queue.h
* By   : 
* Date : 2019/01/03
* version : V0.1 
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
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __QUEUE_H
#define   __QUEUE_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
/**
 * @brief 各种错误枚举定义
 */
typedef enum
{
    //标准错误定义
    QUEUE_OK            = (0x00), /*!<  */
    QUEUE_ERR           = (0x01),
	//特殊错误定义 
    QUEUE_ERR_EMPTY     = (0xBF), /*!< 队列为空 */
    QUEUE_ERR_FULL      = (0xA0), /*!< 队列为满 */
	QUEUE_CREATE_FAIL   = (0xA1), /*!< 队列创建失败 */
	QUEUE_DELETE_FAIL   = (0xA2), /*!< 队列删除失败 */
}QUEUE_Error;

typedef enum
{
    QUEUE_STATUS_ERROR  = (0x00), /*!< 错误状态 */
    QUEUE_STATUS_IDLE   = (0x01), /*!< 空闲状态 */
    QUEUE_STATUS_BUSY   = (0x02), /*!< 繁忙状态 */
}QUEUE_Status;

typedef enum
{
    ISFULL_NOT_IN       = (0x00),
    ISFULL_DEL_OLD      = (0x01),
}QUEUE_Opt;
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/

/** 
 * @brief  CAN init structure definition
 */
typedef struct circlequeue{
    QUEUE_Status sta;   ///< 队列状态
    
    void*   pBase;      ///< 缓冲区基地址
    void*   pEnd;       ///< 缓冲区末尾地址
    int     maxSize;    ///< 缓冲区最大大小
    
    void*   pHead;      ///< 缓冲区有效数据队头地址
    void*   pTail;      ///< 缓冲区有效数据队尾地址
    int     valLen;     ///< 缓冲区有效数据长度
}Queue;

typedef struct
{
    uint8_t a[5];
    uint16_t b;
    uint8_t c[128];
}queue_test;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
/*环形队列 操作函数*/
QUEUE_Error Queue_Create( Queue* pQueue,void* pArray,int maxsize,int unit_len );
QUEUE_Error Queue_Delete( Queue* pQueue );
QUEUE_Error Queue_In    ( Queue* pQueue,void* pUnit_In,int unit_len,QUEUE_Opt opt,void* pUnit_Out );
QUEUE_Error Queue_Out   ( Queue* pQueue,void* pUnit_Out,int unit_len );

QUEUE_Error Queue_Test( void );


#ifdef __cplusplus
}
#endif

#endif
