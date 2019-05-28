//文档说明
//操作链表：某个操作需要等待一定的CPU时钟，再某个中断中才能完成操作
//          而外部调用需要在某一个操作未完成时就继续操作；这样就需要
//          一个操作链表，将操作添加到链表中，设置操作完成回调函数
//          由驱动控制链表的添加和移除，使应用层调用更简单

//操作链表和互斥锁的应用区别，主要是操作的事务是否需要一直在CPU中运行；
//如果大部分时间不需要CPU干预，则选择 OperationList ； 否则选择 BoxOs_MutexLock

#ifndef  __QUEUE__H
#define  __QUEUE__H


#include "includes.h"
#include <stdio.h>  
#include <stdlib.h>  

#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  DEF_NO                                            0u
#define  DEF_YES                                           1u

#define  DEF_DISABLED                                      0u
#define  DEF_ENABLED                                       1u

#define  DEF_INACTIVE                                      0u
#define  DEF_ACTIVE                                        1u

#define  DEF_INVALID                                       0u
#define  DEF_VALID                                         1u

#define  DEF_OFF                                           0u
#define  DEF_ON                                            1u

#define  DEF_CLR                                           0u
#define  DEF_SET                                           1u

#define  DEF_FAIL                                          0u
#define  DEF_OK                                            1u


typedef struct Queue_t   
{  
    uint8_t *p_base;   /*队列数据的地址*/
    uint16_t front;   /*指向队列第一个元素*/
    uint16_t rear;    /*指向队列最后一个元素的下一个元素*/
    uint16_t maxsize; /*循环队列的最大存储空间*/
}QUEUE,*PQUEUE;  
  
uint8_t Queue_create(PQUEUE Q, uint8_t *pbuf, uint16_t maxsize);
uint8_t Queue_is_full(PQUEUE Q);
uint8_t Queue_is_empty(PQUEUE Q);
uint16_t Queue_len(PQUEUE Q);
uint8_t Queue_enqueue(PQUEUE Q, uint8_t val);
uint8_t Queue_dequeue(PQUEUE Q, uint8_t *val);

#endif
