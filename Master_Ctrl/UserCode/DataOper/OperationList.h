//�ĵ�˵��
//��������ĳ��������Ҫ�ȴ�һ����CPUʱ�ӣ���ĳ���ж��в�����ɲ���
//          ���ⲿ������Ҫ��ĳһ������δ���ʱ�ͼ�����������������Ҫ
//          һ������������������ӵ������У����ò�����ɻص�����
//          �����������������Ӻ��Ƴ���ʹӦ�ò���ø���

//��������ͻ�������Ӧ��������Ҫ�ǲ����������Ƿ���Ҫһֱ��CPU�����У�
//����󲿷�ʱ�䲻��ҪCPU��Ԥ����ѡ�� OperationList �� ����ѡ�� BoxOs_MutexLock

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
    LIST_ADD_ITEM_TO_TAIL = 0,  //���Ԫ�ص�β��
    LIST_ADD_ITEM_TO_HEAD    ,  //���Ԫ�ص�β��
}ListAddType;

typedef enum
{
    LIST_DEL_ITEM_RT_SUCCESS = 0,  //
    LIST_DEL_ITEM_RT_NO_ITEM    ,  //û���ҵ���ɾ��Ԫ��
    LIST_DEL_ITEM_RT_CONFUSION  ,  //������ң�����ϵͳ�쳣
}ListDeleteResult;

enum
{
    OPER_QUEUE_STATE_NULL = 0,
    OPER_QUEUE_STATE_IN_LIST ,  //���ڶ�����
    OPER_QUEUE_STATE_OPERING ,  //���ڱ�������
    OPER_QUEUE_STATE_OPER_END,  //������� �� �������л��� OPER_QUEUE_STATE_NULL ״̬


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
    struct List_tag     *pNext;  //������һ��
    struct ListCtrl_tag *pListCtrl;  //����������ĸ������������
    //BX_UINT           Data[4];  //����
}List;

//��Ƕ��ʽϵͳ�У�Ϊ������CPUִ���ٶȣ������ǵ���������������״̬��Ҫһ����ʼָ��ͽ���ָ��
typedef struct ListCtrl_tag
{
    List* pHead;
    List* pTail;
    int   nCnt;   //nCnt��Ϊһ���ο����� 
}ListCtrl;

void  ListAddItem(ListCtrl* pCtrl,List* toAdd);
void  ListAddItemEx(ListCtrl* pCtrl,List* toAdd,ListAddType addType);
int   ListDelItem(ListCtrl* pCtrl,List* toDel);  //ɾ��һ��Ԫ�أ��ɹ�����0 �������������־
List* ListPickItem(ListCtrl* pCtrl);   //����������ȡһ��Ԫ�أ���ȡ��ɺ��������ɾ��

//��������(OperationList)������(Queue)
//����Ҫ���Ӳ���ʱ��������ӵ������У���������������ж�����ɣ�
//һ��������ɺ󣬵��ò���������ɵĻص����������Ҽ���ִ����һ������
typedef struct OperationQueue_tag
{
    List                list;
    VoidParaFuncPtr     pFinishCb; //�������ʱ�Ļص�����;��������ж��б�����
    void*               pFinishCbPara ; //��ɻص���������
    int                 queueState ;    //����״̬�� ���� �ޣ������У������У���ɣ���  ״̬ѭ����ĳ��״̬
    BX_UINT             sOperState ;    //��������״̬ �ɹ�����ʧ��
}OperationQueue;

//��������������һ��Ԫ�ص���β
static __inline void  OperationQueueAdd(ListCtrl* pCtrl,OperationQueue* toAdd)
{
    toAdd->queueState = OPER_QUEUE_STATE_IN_LIST;
    ListAddItemEx(pCtrl, (List*)(toAdd) , LIST_ADD_ITEM_TO_TAIL );
}
//��������������һ��Ԫ�ص���β ���� ��ʼ����ӣ�LIST_ADD_ITEM_TO_HEAD��
static __inline void  OperationQueueAddEx(ListCtrl* pCtrl,OperationQueue* toAdd,ListAddType addType)
{
    toAdd->queueState = OPER_QUEUE_STATE_IN_LIST;
    ListAddItemEx(pCtrl, (List*)(toAdd) , addType );
}
//�Ӳ�����������ȡһ��Ԫ��
static __inline OperationQueue*  OperationQueuePick(ListCtrl* pCtrl)
{
    OperationQueue* pPick = (OperationQueue*)(ListPickItem(pCtrl));
    if ( pPick )
    {
        pPick->queueState = OPER_QUEUE_STATE_OPERING;
    }
    return pPick ;
}
//�Ӳ���������ɾ��һ��Ԫ��
static __inline int  OperationQueueDel(ListCtrl* pCtrl,OperationQueue* toDel)
{
    int rt = ListDelItem(pCtrl, (List*)(toDel) ) ;
    toDel->queueState = OPER_QUEUE_STATE_NULL;
    return rt;
}

#endif
