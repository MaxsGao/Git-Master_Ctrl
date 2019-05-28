//文档说明
//操作链表：某个操作需要等待一定的CPU时钟，再某个中断中才能完成操作
//          而外部调用需要在某一个操作未完成时就继续操作；这样就需要
//          一个操作链表，将操作添加到链表中，设置操作完成回调函数
//          由驱动控制链表的添加和移除，使应用层调用更简单

//操作链表和互斥锁的应用区别，主要是操作的事务是否需要一直在CPU中运行；
//如果大部分时间不需要CPU干预，则选择 OperationList ； 否则选择 BoxOs_MutexLock

#ifndef  __OPERATION_LIST__H
#define  __OPERATION_LIST__H

#ifdef __cplusplus
extern "C" {
#endif

#include<string.h>
#include<stdlib.h>
    

typedef void (*VoidParaFuncPtr) (void*);
    
typedef enum
{
    LIST_ADD_ITEM_TO_TAIL = 0,  //添加元素到尾部
    LIST_ADD_ITEM_TO_HEAD    ,  //添加元素到尾部
}ListAddType;

typedef enum
{
    LIST_DEL_ITEM_RT_SUCCESS = 0,  //
    LIST_DEL_ITEM_RT_NO_ITEM    ,  //没有找到被删除元素
    LIST_DEL_ITEM_RT_CONFUSION  ,  //链表混乱，属于系统异常
}ListDeleteResult;

enum
{
    OPER_QUEUE_STATE_NULL = 0,
    OPER_QUEUE_STATE_IN_LIST ,  //处于队列中
    OPER_QUEUE_STATE_OPERING ,  //正在被处理中
    OPER_QUEUE_STATE_OPER_END,  //处理结束 ， 会立即切换到 OPER_QUEUE_STATE_NULL 状态


    OPER_QUEUE_STATE_OK  = 0,
    OPER_QUEUE_STATE_ERR1    ,
    OPER_QUEUE_STATE_ERR2    ,
    OPER_QUEUE_STATE_ERR3    ,
    OPER_QUEUE_STATE_ERR4    ,
    OPER_QUEUE_STATE_ERR5    ,
};

struct ListCtrl_tag;

typedef struct List_tag
{
    struct List_tag     *pNext;  //链表下一个
    struct ListCtrl_tag *pListCtrl;  //链表存在于哪个链表管理器中
    //BX_UINT           Data[4];  //数据
}List;

//在嵌入式系统中，为了增加CPU执行速度；链表都是单向链表，管理链表状态需要一个开始指针和结束指针
typedef struct ListCtrl_tag
{
    List* pHead;
    List* pTail;
    int   nCnt;   //nCnt作为一个参考数据 
}ListCtrl;

void  ListAddItem(ListCtrl* pCtrl,List* toAdd);
void  ListAddItemEx(ListCtrl* pCtrl,List* toAdd,ListAddType addType);
int   ListDelItem(ListCtrl* pCtrl,List* toDel);  //删除一个元素，成功返回0 其他方法错误标志
List* ListPickItem(ListCtrl* pCtrl);   //从链表中提取一个元素，提取完成后从链表中删除

//操作链表(OperationList)，队列(Queue)
//在需要增加操作时将处理添加到链表中，处理操作过程在中断中完成，
//一个操作完成后，调用操作处理完成的回调函数，并且继续执行下一个操作
typedef struct OperationQueue_tag
{
    List                list;
    VoidParaFuncPtr     pFinishCb; //操作完成时的回调函数;会在软件中断中被调用
    void*               pFinishCbPara ; //完成回调函数参数
    int                 queueState ;    //队列状态； 处于 无，队列中，处理中，完成，无  状态循环中某个状态
    BX_UINT             sOperState ;    //操作结束状态 成功或者失败
}OperationQueue;

//向操作队列中添加一个元素到结尾
static __inline void  OperationQueueAdd(ListCtrl* pCtrl,OperationQueue* toAdd)
{
    toAdd->queueState = OPER_QUEUE_STATE_IN_LIST;
    ListAddItemEx(pCtrl, (List*)(toAdd) , LIST_ADD_ITEM_TO_TAIL );
}
//向操作队列中添加一个元素到结尾 或者 开始处添加（LIST_ADD_ITEM_TO_HEAD）
static __inline void  OperationQueueAddEx(ListCtrl* pCtrl,OperationQueue* toAdd,ListAddType addType)
{
    toAdd->queueState = OPER_QUEUE_STATE_IN_LIST;
    ListAddItemEx(pCtrl, (List*)(toAdd) , addType );
}
//从操作队列中提取一个元素
static __inline OperationQueue*  OperationQueuePick(ListCtrl* pCtrl)
{
    OperationQueue* pPick = (OperationQueue*)(ListPickItem(pCtrl));
    if ( pPick )
    {
        pPick->queueState = OPER_QUEUE_STATE_OPERING;
    }
    return pPick ;
}
//从操作队列中删除一个元素
static __inline int  OperationQueueDel(ListCtrl* pCtrl,OperationQueue* toDel)
{
    int rt = ListDelItem(pCtrl, (List*)(toDel) ) ;
    toDel->queueState = OPER_QUEUE_STATE_NULL;
    return rt;
}

#endif
