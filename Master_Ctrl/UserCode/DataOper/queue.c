/*
*********************************************************************************************************
*
*
*                                       QueueStruct
*                                         ���нṹ
* File : queue.h
* By   : 
* Date : 2019/01/03
* version : V0.1 
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
* History : 
*
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "queue.h"
#include "string.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  * @param  SPI_I2S_FLAG: specifies the SPI flag to clear. 
  *   This function clears only CRCERR flag.
  * @note
  *   - OVR (OverRun error) flag is cleared by software sequence: a read 
  *     operation to SPI_DR register (SPI_I2S_ReceiveData()) followed by a read 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - UDR (UnderRun error) flag is cleared by a read operation to 
  *     SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - MODF (Mode Fault) flag is cleared by software sequence: a read/write 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()) followed by a 
  *     write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */
static QUEUE_Error isQueueFull( Queue* pQueue )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( pQueue->valLen >= pQueue->maxSize )
    {
        errorstatus = QUEUE_ERR_FULL;
    }
    
    return errorstatus;
}
static QUEUE_Error isQueueEmpty( Queue* pQueue )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( pQueue->valLen == 0 )
    {
        errorstatus = QUEUE_ERR_EMPTY;
    }
    
    return errorstatus;
}
static void QUEUE_TailAdd( Queue* pQueue,int unit_len )
{
    if( pQueue->pTail==pQueue->pEnd )
    {
        pQueue->pTail = pQueue->pBase;
    }else
    {
        pQueue->pTail = (char*)pQueue->pTail + unit_len;
    }
}
static void QUEUE_HeadAdd( Queue* pQueue,int unit_len )
{
    if( pQueue->pHead==pQueue->pEnd )
    {
        pQueue->pHead = pQueue->pBase;
    }else
    {
        pQueue->pHead = (char*)pQueue->pHead + unit_len;
    }
}

/* ɾ����ͷ */
static void QUEUE_HeadPop( Queue* pQueue,int unit_len )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    errorstatus = isQueueEmpty(pQueue);
    if( errorstatus!=QUEUE_ERR_EMPTY )
    {
        memset( pQueue->pHead,0,unit_len );
        QUEUE_HeadAdd(pQueue,unit_len);
        pQueue->valLen--;
    }
    
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        RingQueueInit()
*
* Description : To initialize the ring queue.    ��ʼ�����ζ���
*
* Arguments   : pQueue      ָ���ζ��п��ƿ��ָ��
*               pArray      ָ���Զ���Ļ�����(ʵ�ʾ��Ǹ�����)
*               array_size  �������Ĵ�С;
*               unit_len    ��Ԫ����
*
* Return      : 
*
*Note(s):
*********************************************************************************************************
*/

QUEUE_Error Queue_Create( Queue* pQueue,void* pArray,int array_size,int unit_len )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( (pQueue==NULL) || (pArray==NULL) || (array_size==0) )
    {
        errorstatus = QUEUE_ERR;
    }else
    {
        /* ��ʼ������ */
        pQueue->sta     = QUEUE_STATUS_IDLE;
        
        pQueue->pBase   = pArray;
        pQueue->pEnd    = (char*)pArray+((array_size-1)*unit_len);
        pQueue->maxSize = array_size;
        
        pQueue->pHead   = pQueue->pBase;
        pQueue->pTail   = pQueue->pBase;
        pQueue->valLen  = 0;
    }
    
    return errorstatus;
}
QUEUE_Error Queue_Delete( Queue* pQueue )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( pQueue->sta!=QUEUE_STATUS_IDLE )
    {
        return QUEUE_ERR;
    }
    
    return errorstatus;
}
QUEUE_Error Queue_In ( Queue* pQueue,void* pUnit_In,int unit_len,QUEUE_Opt opt,void* pUnit_Out )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( pQueue->sta!=QUEUE_STATUS_IDLE )
    {
        return QUEUE_ERR;
    }
    
    errorstatus = isQueueFull(pQueue);
    if( errorstatus==QUEUE_ERR_FULL )
    {
        if( (opt==ISFULL_DEL_OLD) && (pUnit_Out!=NULL) )
        {
            if( pUnit_Out!=NULL )
            {
                errorstatus = Queue_Out( pQueue,pUnit_Out,unit_len );
                if( errorstatus!=QUEUE_OK )
                {
                    return errorstatus;
                }
            }else
            {
                QUEUE_HeadPop(pQueue,unit_len);
            }
        }else
        {
            return errorstatus;
        }
    }
    
    memset( pQueue->pTail,0,unit_len );
    memcpy( pQueue->pTail,pUnit_In,unit_len );
    QUEUE_TailAdd(pQueue,unit_len);
    pQueue->valLen++;
    
    return errorstatus;
}
QUEUE_Error Queue_Out( Queue* pQueue,void* pUnit_Out,int unit_len )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    
    if( pQueue->sta!=QUEUE_STATUS_IDLE )
    {
        return QUEUE_ERR;
    }
    
    errorstatus = isQueueEmpty(pQueue);
    if( errorstatus==QUEUE_OK )
    {
        memcpy( pUnit_Out,pQueue->pHead,unit_len );
        memset( pQueue->pHead,0,unit_len );
        QUEUE_HeadAdd(pQueue,unit_len);
        pQueue->valLen--;
    }
    
    return errorstatus;
}


Queue Q;
queue_test Q_Test[10];
QUEUE_Error Queue_Test( void )
{
    QUEUE_Error errorstatus=QUEUE_OK;
    queue_test item_in;
    queue_test item_out;
    
    if( Q.pBase!=Q_Test )
    {
        Queue_Create( &Q,Q_Test,10,sizeof(queue_test) );
    }
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=1;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=2;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=3;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=4;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=5;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=6;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=7;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=8;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=9;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=10;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    strcpy((char*)item_in.a,"hello");
    item_in.b=11;
    strcpy( (char*)item_in.c,"world!" );
    errorstatus = Queue_In( &Q,&item_in,sizeof(queue_test),ISFULL_DEL_OLD,&item_out );
    
    memset(&item_out,0,sizeof(queue_test));
    errorstatus = Queue_Out( &Q,&item_out,sizeof(queue_test) );
    
    memset(&item_out,0,sizeof(queue_test));
    errorstatus = Queue_Out( &Q,&item_out,sizeof(queue_test) );
    
    return errorstatus;
}


