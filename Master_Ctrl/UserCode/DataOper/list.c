#include "list.h"
#include "malloc.h"
#include "BoxOs_str.h"

#include<stdarg.h>

/* �������� */
ListCtrl* Create_List (void)
{
    ListCtrl*	pCtrl;
    List*		pHead;
    List*		pTail;
    
    /* �ڴ���� ��������� */
    pCtrl  = mymalloc( SRAMIN,sizeof(struct ListCtrl_tag) );
    pHead  = mymalloc( SRAMIN,sizeof(struct List_tag) );
    pTail  = mymalloc( SRAMIN,sizeof(struct List_tag) );
    
    pHead->pLast = NULL;
    pHead->pNext = pTail;
    
    pTail->pLast = pHead;
    pTail->pNext = NULL;
    
    /* �����������ʼ�� */
    pCtrl->pHead = pHead;
    pCtrl->pTail = pTail;
    pCtrl->nCnt  = 0;
    
    /* ��������������ڴ��ַ */
    return pCtrl;
}

/* ���ӽڵ� ͷ�巨 */
void AddNode_List(ListCtrl* pCtrl,List* toAdd)
{
    struct List_tag* pHead = pCtrl->pHead;
    
    toAdd->pLast = pCtrl->pHead;
    toAdd->pNext = pHead->pNext;
    
    (pHead->pNext)->pLast = toAdd;
    pHead->pNext          = toAdd;
    
    pCtrl->nCnt += 1;
}

/* ���ҽڵ� */
List* FindNode_List(ListCtrl* pCtrl, char option, ... )
{
    List* pOffset;
    int   User_ID;
    char* pUser_IP;
    char* pUser_Name;
    
    /* �ҵ���һ������ڵ� */
    pOffset = pCtrl->pHead->pNext;
    
    switch(option)
    {
        case LIST_OPTION_WHEN_MATCH_USERID:
        {
            va_list v;
            va_start(v,option);
            User_ID = va_arg(v,int);
            va_end(v);
            
            while( pOffset != pCtrl->pTail )
            {
                if( pOffset->Data.User_ID == User_ID )
                {
                    return pOffset;
                }
            }
            break;
        }
        case LIST_OPTION_WHEN_MATCH_USERIP:
        {
            va_list v;
            va_start(v,option);
            pUser_IP = va_arg(v,char*);
            va_end(v);
            
            while( pOffset != pCtrl->pTail )
            {
                if( DoubleStringsIsSame(pOffset->Data.User_IP, pUser_IP) )
                {
                    return pOffset;
                }
            }
            break;
        }
        case LIST_OPTION_WHEN_MATCH_USERNAME:
        {
            va_list v;
            va_start(v,option);
            pUser_Name = va_arg(v,char*);
            va_end(v);
            
            while( pOffset != pCtrl->pTail )
            {
                if( DoubleStringsIsSame(pOffset->Data.User_Name, pUser_Name) )
                {
                    return pOffset;
                }
            }
            break;
        }
        default:
        {
            break;
        }
    }
    return NULL;
}


/* ɾ���ڵ� */
void DelNode_List(ListCtrl* pCtrl,List* toDel)
{
    List* pf = toDel->pLast;//ǰһ���ڵ�
    List* pb = toDel->pNext;//��һ���ڵ�
    
    pf->pNext = toDel->pNext;
    pb->pLast = toDel->pLast;
    
    myfree( SRAMIN,toDel ); //�ͷű�ɾ���ڵ���ռ�ڴ�
}    

