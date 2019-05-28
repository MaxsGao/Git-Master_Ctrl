/*
*********************************************************************************************************
*
*
*                                      RingQueueStruct
*                                        ���ζ��нṹ
*
* File : RingQueue.c
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2017/10/16
* version: V1.1 
* NOTE(s): 
*
* History : 2017/04/25   the original version of RingQueueStruct.
*           2017/10/16   put functions used frequently,RingQueueIn and RingQueueOut, in non-banked address;
*                        modify single line function RingQueueIsEmpty and RingQueueIsFull to marco function;
*                        to get better efficiency.  
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     INCLUDES
*********************************************************************************************************
*/
#include "RingQueue.h"
#include "string.h"


/*
*********************************************************************************************************
*                                   CONSTANTS  ����
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL FUNCTION 
*********************************************************************************************************
*/
static void forwardPointer(RING_QUEUE *pQueue,pRQTYPE* pPointer)
{
	if(++*pPointer == pQueue->RingBufEnd)
		*pPointer   = pQueue->RingBuf;        /* Wrap OUT pointer                          */  
}

/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : To initialize the ring queue.    ��ʼ�����ζ���
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               pbuf     pointer to the buffer(an array);             ָ���Զ���Ļ�����(ʵ�ʾ��Ǹ�����)
*               bufSize  the Size of the buffer;                      �������Ĵ�С;
*
* Return      : the pointer to the ring queue control block;        ����ָ���ζ��п��ƿ��ָ��
*               0x00 if any error;                                  ����������򷵻�NULL
*
*Note(s):
*********************************************************************************************************
*/
RQTYPE RingQueueInit(RING_QUEUE* pQueue,pRQTYPE pbuf,unsigned short bufSize)
{
	if(pQueue == 0x00 || pbuf == 0x00)
	{
		return RQ_ERR_POINTER_NULL;      
	}

	if(bufSize == 0)
	{
		return RQ_ERR_SIZE_ZERO;      
	}

	pQueue->RingBufCtr      = 0;                //���ζ����е�������
	pQueue->RingBufSize     = bufSize;          //���ζ��л�������С
    pQueue->RingBufSta      = RQ_STATUS_IDLE;   //���ζ��еĹ���״̬������/æ
	pQueue->RingBuf         = pbuf;             //���ζ��л������׵�ַ
	pQueue->RingBufInPtr    = pbuf;             //��ӵ�λ��
	pQueue->RingBufOutPtr   = pbuf;             //���ӵ�λ��
	pQueue->RingBufEnd      = pbuf + bufSize;   //���ζ��л�����β��ַ

	return RQ_ERR_NONE;
}

/*
*********************************************************************************************************
*                                        RingQueueIn()
*
* Description : Enqueue an element.    ���һ��Ԫ��
*
* Arguments   : pQueue   pointer to the ring queue control block;    ָ���ζ��п��ƿ��ָ��
*               data     the data to enqueue;                        Ҫ��ӵ�����
*               option   option when queue is full ,you can choose:  ����������ʱ���ѡ��,�����ѡ��:
*                            RQ_OPTION_WHEN_FULL_DISCARD_FIRST          ������ͷ��Ԫ�������ȥ�µ�Ԫ��
*                            RQ_OPTION_WHEN_FULL_DONT_IN                ������¸���Ԫ��
*
* Return       : the Elements Count after enqueue the element
*                    ���ú���������е�Ԫ�ظ���
*Note(s)       :
*********************************************************************************************************
*/
RQTYPE RingQueueIn(RING_QUEUE* pQueue,RQTYPE data,RQTYPE option)
{
	if(pQueue == 0x00)
	{
		return RQ_ERR_POINTER_NULL; 
	}

	if(pQueue->RingBufCtr >= pQueue->RingBufSize)
	{
		if(option == RQ_OPTION_WHEN_FULL_DISCARD_FIRST)
		{
			forwardPointer(pQueue,&pQueue->RingBufOutPtr);  /* Wrap OUT pointer                         */
            *pQueue->RingBufInPtr = data;                   /* Put character into buffer                */
            forwardPointer(pQueue,&pQueue->RingBufInPtr);   /* Wrap IN pointer                          */
			return RQ_ERR_BUFFER_FULL;
		}
		else
		{                                                   // option == RQ_OPTION_WHEN_FULL_DONT_IN
			return RQ_ERR_BUFFER_FULL;
		}
	}
	else
	{
		pQueue->RingBufCtr++;                               /* No, increment character count            */      
	}
	*pQueue->RingBufInPtr = data;                           /* Put character into buffer                */  
	forwardPointer(pQueue,&pQueue->RingBufInPtr);           /* Wrap IN pointer                          */  
	return RQ_ERR_NONE;
}

/*
*********************************************************************************************************
*                                        RingQueueOut()
*
* Description : Dequeue an element.       ����һ��Ԫ��
*
* Arguments   : pQueue   pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*               perr     a pointer to a variable containing an error message which will be set by this
*                          function to either:
*
*                              RQ_ERR_NONE                            if no err happen
*                              RQ_ERR_POINTER_NULL                    if pointer is 0x00
*                              RQ_ERR_BUFFER_EMPTY                    if buffer is empty
*
* Return      : the data 
*               0                 if any error;
*Note(s):
*********************************************************************************************************
*/
RQTYPE RingQueueOut(RING_QUEUE* pQueue)
{
	RQTYPE data;

	if(pQueue == 0x00)
	{
		return 0;
	}

	if(pQueue->RingBufCtr == 0)
	{       
		return 0;
	}

	pQueue->RingBufCtr--;                                   /*  decrement character count           */  
	data = *pQueue->RingBufOutPtr;                          /* Get character from buffer                */  
	forwardPointer(pQueue,&pQueue->RingBufOutPtr);          /* Wrap OUT pointer                          */  

	return data;
}

/*
*********************************************************************************************************
*                                        RingQueueClear()
*
* Description:  Clear the RingQueue.        ��ջ��ζ���
*
* Arguments  :  pQueue    pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
*
* Return:             
*
* Note(s):
*********************************************************************************************************
*/

void RingQueueClear(RING_QUEUE *pQueue)
{
	pQueue->RingBufCtr     = 0;
	pQueue->RingBufInPtr   = pQueue -> RingBufOutPtr;
}

/*
*********************************************************************************************************
*                                        RingQueueStr()
*
* Description : Match the given string in RingQueue                 �ڻ��ζ�����ƥ������ַ���
*
* Arguments   : pQueue   pointer to the ring queue control block;   ָ���ζ��п��ƿ��ָ��
*               pStr     pointer to the chars need to match;
* Return      :  NULL    Don't match            û��ƥ�䵽
*                !NULL   match                  ƥ�䵽��
*
*Note(s):
*********************************************************************************************************
*/
pRQTYPE RingQueueStr(RING_QUEUE* pQueue,pRQTYPE pStr,RQTYPE option)
{
	unsigned short rq_len,str_len;
	pRQTYPE pPosQ,pCurQ;
	pRQTYPE pEndS,pCurS;

	str_len = strlen((char*)pStr);

	/* ���ζ������ݳ�����ƥ���ַ����ıȽ� */
	if( str_len > pQueue->RingBufCtr )
		return NULL;

	rq_len  = pQueue->RingBufCtr;
	pPosQ = pQueue->RingBufOutPtr;
	pEndS = pStr + str_len;

	while( rq_len >= str_len )
	{
		pCurQ = pPosQ;
		pCurS = pStr;

		while( (pCurS!=pEndS) && (*pCurQ==*pCurS) )
		{
			forwardPointer( pQueue,&pCurQ );
			pCurS++;
		}
		if( pCurS == pEndS )
		{
			if( option == RQ_OPTION_WHEN_RETURN_CAPITAL )
			{
				pQueue->RingBufCtr      = rq_len;
				pQueue->RingBufOutPtr   = pPosQ;
				return pPosQ;
			}
			else
				return pCurQ;
		}

		forwardPointer( pQueue,&pPosQ );
		rq_len--;
	}
	return NULL;
}

/*
*********************************************************************************************************
*                                        RingQueueMatch()
*
* Description : Match the given buffer in RingQueue                 �ڻ��ζ�����ƥ�����������
*
* Arguments   : pQueue   pointer to the ring queue control block;   ָ���ζ��п��ƿ��ָ��
*               pbuf     pointer to the chars need to match;
*               len      the length of the chars
* Return      :  -1       Don't match            -1    ��û��ƥ�䵽
*                >= 0     match                  >= 0  ��ƥ�䵽��
*
*Note(s):
*********************************************************************************************************
*/
short RingQueueMatch(RING_QUEUE *pQueue,pRQTYPE pbuf,unsigned short len)
{
	pRQTYPE pPosQ,pCurQ,pCurB,pEndB;
	unsigned short rLen,Cnt;

	if(len > pQueue->RingBufCtr)
		return -1;

	pPosQ = pQueue->RingBufOutPtr;
	pEndB = pbuf + len;
	Cnt = 0;
	rLen = pQueue ->RingBufCtr;

	while(rLen-- >= len){                                    // if remian length of queue bigger than buffer. continue
		pCurQ = pPosQ;
		pCurB = pbuf;
		while(pCurB != pEndB && *pCurQ == *pCurB) {    // compare one by one,until match all(pCurB==pEndB) or some one don't match
			forwardPointer(pQueue,&pCurQ);
			pCurB++;
		}
		if(pCurB == pEndB)                                                 // if match all
			return Cnt;
		Cnt++;
		forwardPointer(pQueue,&pPosQ);
	}
	return -1;
}

/*
*********************************************************************************************************
*                                        RingQueueStrCpy()
*
* Description : �������ζ�����һ������
*
* Arguments   : pQueue   ָ���ζ��п��ƿ��ָ��
*               pOut     �����ݿ�����pOut��ָ��ĵ�ַ�ռ�
*               pInB     ���������ݵĿ�ʼλ�� Begin
*               pInE     ���������ݵĽ���λ�� End
* Return      : void*    
*
*Note(s):
*********************************************************************************************************
*/
void* RingQueueStrCpy(RING_QUEUE* pQueue,pRQTYPE pDest,pRQTYPE pSrcB,pRQTYPE pSrcE)
{
	pRQTYPE pOffset;    //ƫ��ָ��
	pOffset = pDest;

	if( pQueue->RingBufOutPtr != pSrcB )
	{
		return NULL;
	}

	while( pQueue->RingBufOutPtr != pSrcE )
	{
		*pOffset = RingQueueOut(pQueue);
		pOffset++;
	}
	*pOffset = 0;

	return pDest;
}

