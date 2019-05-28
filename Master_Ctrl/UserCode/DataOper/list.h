//文档说明
//操作链表：某个操作需要等待一定的CPU时钟，再某个中断中才能完成操作
//          而外部调用需要在某一个操作未完成时就继续操作；这样就需要
//          一个操作链表，将操作添加到链表中，设置操作完成回调函数
//          由驱动控制链表的添加和移除，使应用层调用更简单

//操作链表和互斥锁的应用区别，主要是操作的事务是否需要一直在CPU中运行；
//如果大部分时间不需要CPU干预，则选择 OperationList ； 否则选择 BoxOs_MutexLock

#ifndef  __LIST__H
#define  __LIST__H

#define LIST_OPTION_WHEN_MATCH_USERID	0u
#define LIST_OPTION_WHEN_MATCH_USERIP	1u
#define LIST_OPTION_WHEN_MATCH_USERNAME	2u

typedef struct ListData_tag
{
    int     User_ID;        //用户ID
    char    User_IP[4];     //用户IP
    short   User_Port;      //用户端口
    char    User_Name[50];  //用户名
    
    /* 在线状态 User_State0 != User_State1 */
    /* 下线状态 User_State0 == User_State1 */
    /* 操作方法：初始化为相同状态；判断到两状态不同时，修改为state0=state1；
    接收到在线通知时，state0不变，state1++ */
    int     User_State0;    //用户状态0
    int     User_State1;    //用户状态1
}ListData;

struct ListCtrl_tag;
//链表元素
typedef struct List_tag
{
    struct List_tag*        pLast;      //指向上一个元素
    struct List_tag*        pNext;      //指向下一个元素
    struct ListCtrl_tag*    pListCtrl;  //链表存在于哪个链表管理器中
    struct ListData_tag     Data;       //数据
}List;

//链表管理器
typedef struct ListCtrl_tag
{
    List* pHead;
    List* pTail;
    int   nCnt;   //nCnt作为一个参考数据 
}ListCtrl;


ListCtrl*   Create_List (void                       );  //创建链表
void        AddNode_List(ListCtrl* pCtrl,List* toAdd);  //增加节点
List*       FindNode_List(ListCtrl* pCtrl, char option, ... );//查找节点
void        DelNode_List(ListCtrl* pCtrl,List* toDel);  //删除节点

#endif
