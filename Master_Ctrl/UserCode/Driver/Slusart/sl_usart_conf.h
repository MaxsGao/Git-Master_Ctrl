/*
*********************************************************************************************************
*
*
*                                       Simulation Usart
*                                          ģ�⴮��
* File : sl_usart_conf.h
* By   : Bear
* Date : 2018/11/23
* version : V0.1 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __SL_USART_CONF_H
#define   __SL_USART_CONF_H


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include <stdint.h>


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
//#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full


/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
void        SlUsart_Config      ( u32 baud,uint8_t *recv_buffer );
void        SlUsart_SendOneByte ( uint8_t datatoSend);
uint32_t    SlUsart_RecvBytes   ( uint8_t *recvBuffer,uint32_t Buf_len );

#endif