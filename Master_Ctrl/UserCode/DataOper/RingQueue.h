/*
*********************************************************************************************************
*
*
*                                       RingQueueStruct
*                                         ���ζ��нṹ
* File : RingQueue.h
* By   : Lin Shijun(http://blog.csdn.net/lin_strong)
* Date : 2017/10/16
* version : V1.1 
* NOTE(s) : ��γ���������һ�������Ļ���������ģ�⻷�ζ��еĹ���
*                   ���������Զ����仺�����ռ�,�û���Ҫ�Լ��������ռ�,����Ҫ��֤��ֱ�ӷ��ʻ�����
*                   // ��ĳ�������ڴ�ռ�
*                   RQTYPE buffer[BUFFER_SIZE];
*                   RING_QUEUE que,*ptr_que;
*                   unsigned char err;
*                   // ��ʼ��
*                   ptr_que = RingQueueInit(&que,buffer,BUFFER_SIZE,&err);
*                   if(err == RQ_ERR_NONE  ){
*                     // ��ʼ���ɹ�,ʹ����������
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
*                                       CONSTANTS     ����
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

#define   RQ_STATUS_IDLE                     0u         //����״̬
#define   RQ_STATUS_BUSY                     1u         //æ״̬

#undef    NULL
#define   NULL 0                   /* see <stddef.h> */


/*
*********************************************************************************************************
*                                    DATA TYPE    ��������
*********************************************************************************************************
*/
// define the data type that stores in the RingQueue.       ������ڻ��λ������ڵ����ݵ�����
typedef unsigned char RQTYPE;
typedef RQTYPE* pRQTYPE;
typedef struct {
	unsigned short  RingBufCtr;              /* Number of characters in the ring buffer */
	unsigned short  RingBufSize;             /* Ring buffer Size */
    RQTYPE  RingBufSta;                      /* ���ζ��е�״̬ */
	pRQTYPE RingBufInPtr;                    /* Pointer to where next character will be inserted        */  
	pRQTYPE RingBufOutPtr;                   /* Pointer from where next character will be extracted     */  
	pRQTYPE RingBuf;                         /* Ring buffer array */  
	pRQTYPE RingBufEnd;                      /* Point to the end of the buffer */
} RING_QUEUE;


/*
*********************************************************************************************************
*                                  FUNCTION PROTOTYPES ����ԭ��
*********************************************************************************************************
*/
/*���ζ��� ��������*/
RQTYPE  RingQueueInit (RING_QUEUE* pQueue,pRQTYPE pbuf,unsigned short bufSize);//��ʼ��
RQTYPE  RingQueueIn   (RING_QUEUE* pQueue,RQTYPE  data,RQTYPE option         );//���
RQTYPE  RingQueueOut  (RING_QUEUE* pQueue                                    );//����
void    RingQueueClear(RING_QUEUE* pQueue                                    );//��ն���

/*���ζ��� Ӧ�ú���*/
pRQTYPE RingQueueStr   (RING_QUEUE* pQueue,pRQTYPE pStr, RQTYPE option                   );
short	RingQueueMatch (RING_QUEUE* pQueue,pRQTYPE pbuf, unsigned short len              );
void*   RingQueueStrCpy(RING_QUEUE* pQueue,pRQTYPE pDest,pRQTYPE pSrcB,     pRQTYPE pSrcE);

/*
*********************************************************************************************************
*                                        RingQueueIsEmpty()
*
* Description :  whether the RingQueue is empty.   ���ζ����Ƿ�Ϊ��
*
* Arguments   :  pQueue  pointer to the ring queue control block;     ָ���ζ��п��ƿ��ָ��
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
* Description : whether the RingQueue is full.    ���ζ����Ƿ�Ϊ��
*
* Arguments   : pQueue  pointer to the ring queue control block;   ָ���ζ��п��ƿ��ָ��
*
* Return      : TRUE    the RingQueue is full.
*               FALSE   the RingQueue is not full.
* Note(s)     :
*********************************************************************************************************
*/
#define RingQueueIsFull(pQueue)  ((pQueue)->RingBufCtr >= (pQueue)->RingBufSize)

#endif
