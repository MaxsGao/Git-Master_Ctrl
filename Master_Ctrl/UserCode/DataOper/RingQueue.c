/*
*********************************************************************************************************
*
*
*                                      RingQueueStruct
*                                        环形队列结构
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
*                                   CONSTANTS  常量
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
* Description : To initialize the ring queue.    初始化环形队列
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
*               pbuf     pointer to the buffer(an array);             指向自定义的缓冲区(实际就是个数组)
*               bufSize  the Size of the buffer;                      缓冲区的大小;
*
* Return      : the pointer to the ring queue control block;        返回指向环形队列控制块的指针
*               0x00 if any error;                                  如果出错了则返回NULL
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

	pQueue->RingBufCtr      = 0;                //环形队列中的数据量
	pQueue->RingBufSize     = bufSize;          //环形队列缓冲区大小
    pQueue->RingBufSta      = RQ_STATUS_IDLE;   //环形队列的工作状态：空闲/忙
	pQueue->RingBuf         = pbuf;             //环形队列缓冲区首地址
	pQueue->RingBufInPtr    = pbuf;             //入队的位置
	pQueue->RingBufOutPtr   = pbuf;             //出队的位置
	pQueue->RingBufEnd      = pbuf + bufSize;   //环形队列缓冲区尾地址

	return RQ_ERR_NONE;
}

/*
*********************************************************************************************************
*                                        RingQueueIn()
*
* Description : Enqueue an element.    入队一个元素
*
* Arguments   : pQueue   pointer to the ring queue control block;    指向环形队列控制块的指针
*               data     the data to enqueue;                        要入队的数据
*               option   option when queue is full ,you can choose:  当队列满的时候的选项,你可以选择:
*                            RQ_OPTION_WHEN_FULL_DISCARD_FIRST          抛弃队头的元素来填进去新的元素
*                            RQ_OPTION_WHEN_FULL_DONT_IN                不入队新给的元素
*
* Return       : the Elements Count after enqueue the element
*                    调用函数后队列中的元素个数
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
* Description : Dequeue an element.       出队一个元素
*
* Arguments   : pQueue   pointer to the ring queue control block;     指向环形队列控制块的指针
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
* Description:  Clear the RingQueue.        清空环形队列
*
* Arguments  :  pQueue    pointer to the ring queue control block;     指向环形队列控制块的指针
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
* Description : Match the given string in RingQueue                 在环形队列中匹配给定字符串
*
* Arguments   : pQueue   pointer to the ring queue control block;   指向环形队列控制块的指针
*               pStr     pointer to the chars need to match;
* Return      :  NULL    Don't match            没有匹配到
*                !NULL   match                  匹配到了
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

	/* 环形队列数据长度与匹配字符串的比较 */
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
* Description : Match the given buffer in RingQueue                 在环形队列中匹配给定缓冲区
*
* Arguments   : pQueue   pointer to the ring queue control block;   指向环形队列控制块的指针
*               pbuf     pointer to the chars need to match;
*               len      the length of the chars
* Return      :  -1       Don't match            -1    则没有匹配到
*                >= 0     match                  >= 0  则匹配到了
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
* Description : 拷贝环形队列中一段内容
*
* Arguments   : pQueue   指向环形队列控制块的指针
*               pOut     将内容拷贝到pOut所指向的地址空间
*               pInB     被拷贝内容的开始位置 Begin
*               pInE     被拷贝内容的结束位置 End
* Return      : void*    
*
*Note(s):
*********************************************************************************************************
*/
void* RingQueueStrCpy(RING_QUEUE* pQueue,pRQTYPE pDest,pRQTYPE pSrcB,pRQTYPE pSrcE)
{
	pRQTYPE pOffset;    //偏移指针
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

