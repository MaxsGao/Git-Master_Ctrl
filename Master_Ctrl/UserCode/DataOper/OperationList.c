#include "OperationList.h"


void  ListAddItem(ListCtrl* pCtrl,List* toAdd)
{
    ListAddItemEx( pCtrl , toAdd , LIST_ADD_ITEM_TO_TAIL );
}
void  ListAddItemEx(ListCtrl* pCtrl,List* toAdd,ListAddType addType)
{
    OS_CPU_SR  cpu_sr = 0;
    OS_ENTER_CRITICAL();

    if ( toAdd->pListCtrl == pCtrl )  //����Ѿ�������������
    {
        OS_EXIT_CRITICAL();
        return;
    }

    if ( (toAdd->pListCtrl) && (toAdd->pNext) )
    {
        ListDelItem( toAdd->pListCtrl , toAdd );
    }

    if ( (NULL==pCtrl->pTail) || (NULL==pCtrl->pHead) || (0==pCtrl->nCnt) )
    {
        pCtrl->pTail   = toAdd;
        pCtrl->pHead   = toAdd;
        toAdd->pNext   = NULL ;
        pCtrl->nCnt    = 0;
    }
    else if ( LIST_ADD_ITEM_TO_TAIL == addType )  //��ӵ�β��
    {
        pCtrl->pTail->pNext = toAdd;
        pCtrl->pTail        = toAdd;
        toAdd->pNext        = NULL ;
    } 
    else
    {
        toAdd->pNext   = pCtrl->pHead ;
        pCtrl->pHead   = toAdd;
    }
    toAdd->pListCtrl = pCtrl;
    pCtrl->nCnt++;
    OS_EXIT_CRITICAL();
}

//ɾ��һ��Ԫ�أ��ɹ�����0 �������������־
u32   ListDelItem(ListCtrl* pCtrl,List* toDel)
{
    OS_CPU_SR  cpu_sr = 0;
    List* pBl     ;
    List* pBlPrev ;
    OS_ENTER_CRITICAL();

    if ( pCtrl != toDel->pListCtrl )
    {
        OS_EXIT_CRITICAL();
        return LIST_DEL_ITEM_RT_NO_ITEM;
    }

    pBl     = pCtrl->pHead;
    pBlPrev = pCtrl->pHead;
    while ( pBl )
    {
        if ( pBl == toDel )
        {
            if ( pBlPrev == pBl )  //����pHeadλ��
            {
                pCtrl->pHead = pCtrl->pHead->pNext ;
                if ( pCtrl->pHead == NULL )
                {
                    pCtrl->pTail = NULL ;
                    pCtrl->nCnt  = 1;
                }
            }
            else
            {
                pBlPrev->pNext = pBl->pNext ;
            }
            toDel->pListCtrl = NULL;
            toDel->pNext     = NULL;
            pCtrl->nCnt--;

            OS_EXIT_CRITICAL();
            return LIST_DEL_ITEM_RT_SUCCESS;
        }
        pBlPrev = pBl ;
        pBl = pBl->pNext ;
    }

    toDel->pListCtrl = NULL;
    toDel->pNext     = NULL;
    OS_EXIT_CRITICAL();
    BOX_ERROR_HANDLER(BOXOS_ERR_LIST_CONFUSION); //�������ݻ���
    return LIST_DEL_ITEM_RT_CONFUSION;
}

//����������ȡһ��Ԫ�أ���ȡ��ɺ��������ɾ��
List* ListPickItem(ListCtrl* pCtrl)
{
    OS_CPU_SR  cpu_sr = 0;
    List* pListRt = NULL;
    OS_ENTER_CRITICAL();
    
    if ( pCtrl->pHead )
    {
        pListRt      = pCtrl->pHead;
        pCtrl->pHead = pCtrl->pHead->pNext ;
        if ( pCtrl->pHead == NULL )
        {
            pCtrl->pTail = NULL ;
            pCtrl->nCnt  = 1;
        }
        pListRt->pListCtrl = NULL;
        pListRt->pNext     = NULL;
        pCtrl->nCnt--;
    }

    OS_EXIT_CRITICAL();
    return pListRt;
}


