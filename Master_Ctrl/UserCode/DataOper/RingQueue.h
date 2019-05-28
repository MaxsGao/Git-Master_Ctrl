/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         环形队列结构
* File : RingQueue.h
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
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.
*
*
*********************************************************************************************************
*/
#ifndef   RING_QUEUE_H
#define   RING_QUEUE_H


/*
*********************************************************************************************************
*                                       INCLUDES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS     常量
*********************************************************************************************************
*/
#define   RQ_ERR_NONE                        0u

#define   RQ_ERR_POINTER_NULL                1u
#define   RQ_ERR_SIZE_ZERO                   2u

#define   RQ_ERR_BUFFER_FULL                 3u
#define   RQ_ERR_BUFFER_EMPTY                4u

#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full
#define   RQ_OPTION_WHEN_FULL_DONT_IN        1u       // discard new element when ring buffer is full

#define   RQ_OPTION_WHEN_RETURN_CAPITAL      0u
#define   RQ_OPTION_WHEN_RETURN_TAIL         1u

#define   RQ_STATUS_IDLE                     0u         //空闲状态
#define   RQ_STATUS_BUSY                     1u         //忙状态

#undef    NULL
#define   NULL 0                   /* see <stddef.h> */


/*
*********************************************************************************************************
*                                    DATA TYPE    数据类型
*********************************************************************************************************
*/
// define the data type that stores in the RingQueue.       定义存在环形缓冲区内的数据的类型
typedef unsigned char RQTYPE;
typedef RQTYPE* pRQTYPE;
typedef struct {
	unsigned short  RingBufCtr;              /* Number of characters in the ring buffer */
	unsigned short  RingBufSize;             /* Ring buffer Size */
    RQTYPE  RingBufSta;                      /* 环形队列的状态 */
	pRQTYPE RingBufInPtr;                    /* Pointer to where next character will be inserted        */  
	pRQTYPE RingBufOutPtr;                   /* Pointer from where next character will be extracted     */  
	pRQTYPE RingBuf;                         /* Ring buffer array */  
	pRQTYPE RingBufEnd;                      /* Point to the end of the buffer */
} RING_QUEUE;


/*
*********************************************************************************************************
*                                  FUNCTION PROTOTYPES 函数原型
*********************************************************************************************************
*/
/*环形队列 操作函数*/
RQTYPE  RingQueueInit (RING_QUEUE* pQueue,pRQTYPE pbuf,unsigned short bufSize);//初始化
RQTYPE  RingQueueIn   (RING_QUEUE* pQueue,RQTYPE  data,RQTYPE option         );//入队
RQTYPE  RingQueueOut  (RING_QUEUE* pQueue                                    );//出队
void    RingQueueClear(RING_QUEUE* pQueue                                    );//清空队列

/*环形队列 应用函数*/
pRQTYPE RingQueueStr   (RING_QUEUE* pQueue,pRQTYPE pStr, RQTYPE option                   );
short	RingQueueMatch (RING_QUEUE* pQueue,pRQTYPE pbuf, unsigned short len              );
void*   RingQueueStrCpy(RING_QUEUE* pQueue,pRQTYPE pDest,pRQTYPE pSrcB,     pRQTYPE pSrcE);

/*
*********************************************************************************************************
*                                        RingQueueIsEmpty()
*
* Description :  whether the RingQueue is empty.   环形队列是否为空
*
* Arguments   :  pQueue  pointer to the ring queue control block;     指向环形队列控制块的指针
*
* Return      :  TRUE    the RingQueue is empty.
*                FALSE   the RingQueue is not empty.
* Note(s)     :
*********************************************************************************************************
*/
#define RingQueueIsEmpty(pQueue) ((pQueue)->RingBufCtr == 0)

/*
*********************************************************************************************************
*                                        RingQueueIsFull()
*
* Description : whether the RingQueue is full.    环形队列是否为空
*
* Arguments   : pQueue  pointer to the ring queue control block;   指向环形队列控制块的指针
*
* Return      : TRUE    the RingQueue is full.
*               FALSE   the RingQueue is not full.
* Note(s)     :
*********************************************************************************************************
*/
#define RingQueueIsFull(pQueue)  ((pQueue)->RingBufCtr >= (pQueue)->RingBufSize)

#endif
