//�ĵ�˵��
//��������ĳ��������Ҫ�ȴ�һ����CPUʱ�ӣ���ĳ���ж��в�����ɲ���
//          ���ⲿ������Ҫ��ĳһ������δ���ʱ�ͼ�����������������Ҫ
//          һ������������������ӵ������У����ò�����ɻص�����
//          �����������������Ӻ��Ƴ���ʹӦ�ò���ø���

//��������ͻ�������Ӧ��������Ҫ�ǲ����������Ƿ���Ҫһֱ��CPU�����У�
//����󲿷�ʱ�䲻��ҪCPU��Ԥ����ѡ�� OperationList �� ����ѡ�� BoxOs_MutexLock

#ifndef  __QUEUE__H
#define  __QUEUE__H


#include "includes.h"
#include <stdio.h>  
#include <stdlib.h>  

#define  DEF_FALSE                                         0u
#define  DEF_TRUE                                          1u

#define  DEF_NO                                            0u
#define  DEF_YES                                           1u

#define  DEF_DISABLED                                      0u
#define  DEF_ENABLED                                       1u

#define  DEF_INACTIVE                                      0u
#define  DEF_ACTIVE                                        1u

#define  DEF_INVALID                                       0u
#define  DEF_VALID                                         1u

#define  DEF_OFF                                           0u
#define  DEF_ON                                            1u

#define  DEF_CLR                                           0u
#define  DEF_SET                                           1u

#define  DEF_FAIL                                          0u
#define  DEF_OK                                            1u


typedef struct Queue_t   
{  
    uint8_t *p_base;   /*�������ݵĵ�ַ*/
    uint16_t front;   /*ָ����е�һ��Ԫ��*/
    uint16_t rear;    /*ָ��������һ��Ԫ�ص���һ��Ԫ��*/
    uint16_t maxsize; /*ѭ�����е����洢�ռ�*/
}QUEUE,*PQUEUE;  
  
uint8_t Queue_create(PQUEUE Q, uint8_t *pbuf, uint16_t maxsize);
uint8_t Queue_is_full(PQUEUE Q);
uint8_t Queue_is_empty(PQUEUE Q);
uint16_t Queue_len(PQUEUE Q);
uint8_t Queue_enqueue(PQUEUE Q, uint8_t val);
uint8_t Queue_dequeue(PQUEUE Q, uint8_t *val);

#endif
