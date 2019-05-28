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
#ifndef   __QUEUE_H
#define   __QUEUE_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
/**
 * @brief ���ִ���ö�ٶ���
 */
typedef enum
{
    //��׼������
    QUEUE_OK            = (0x00), /*!<  */
    QUEUE_ERR           = (0x01),
	//��������� 
    QUEUE_ERR_EMPTY     = (0xBF), /*!< ����Ϊ�� */
    QUEUE_ERR_FULL      = (0xA0), /*!< ����Ϊ�� */
	QUEUE_CREATE_FAIL   = (0xA1), /*!< ���д���ʧ�� */
	QUEUE_DELETE_FAIL   = (0xA2), /*!< ����ɾ��ʧ�� */
}QUEUE_Error;

typedef enum
{
    QUEUE_STATUS_ERROR  = (0x00), /*!< ����״̬ */
    QUEUE_STATUS_IDLE   = (0x01), /*!< ����״̬ */
    QUEUE_STATUS_BUSY   = (0x02), /*!< ��æ״̬ */
}QUEUE_Status;

typedef enum
{
    ISFULL_NOT_IN       = (0x00),
    ISFULL_DEL_OLD      = (0x01),
}QUEUE_Opt;
  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/

/** 
 * @brief  CAN init structure definition
 */
typedef struct circlequeue{
    QUEUE_Status sta;   ///< ����״̬
    
    void*   pBase;      ///< ����������ַ
    void*   pEnd;       ///< ������ĩβ��ַ
    int     maxSize;    ///< ����������С
    
    void*   pHead;      ///< ��������Ч���ݶ�ͷ��ַ
    void*   pTail;      ///< ��������Ч���ݶ�β��ַ
    int     valLen;     ///< ��������Ч���ݳ���
}Queue;

typedef struct
{
    uint8_t a[5];
    uint16_t b;
    uint8_t c[128];
}queue_test;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
/*���ζ��� ��������*/
QUEUE_Error Queue_Create( Queue* pQueue,void* pArray,int maxsize,int unit_len );
QUEUE_Error Queue_Delete( Queue* pQueue );
QUEUE_Error Queue_In    ( Queue* pQueue,void* pUnit_In,int unit_len,QUEUE_Opt opt,void* pUnit_Out );
QUEUE_Error Queue_Out   ( Queue* pQueue,void* pUnit_Out,int unit_len );

QUEUE_Error Queue_Test( void );


#ifdef __cplusplus
}
#endif

#endif
