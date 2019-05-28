/*
*********************************************************************************************************
*
*
*                                       Simulation Usart
*                                          模拟串口
* File : sl_usart.h
* By   : Bear
* Date : 2018/11/23
* version : V0.1 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __SL_USART_H
#define   __SL_USART_H


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "stm32f10x.h"
#include <stdint.h>


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
//#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full
#define RX_EXTIDisableIRQ   NVIC_DisableIRQ(EXTI0_IRQn)
enum{
    COM_START_BIT,  //停止位
    COM_D0_BIT,     //bit0
    COM_D1_BIT,	    //bit1
    COM_D2_BIT,	    //bit2
    COM_D3_BIT,	    //bit3
    COM_D4_BIT,	    //bit4
    COM_D5_BIT,	    //bit5
    COM_D6_BIT,	    //bit6
    COM_D7_BIT,	    //bit7
    COM_STOP_BIT,	//bit8
};


/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
typedef void    (*VoidFuncPtr)      (void);
typedef uint8_t (*U8FuncPtr)        (void);
typedef void    (*VoidParaFuncPtr)  (void*);

typedef struct __SL_USART
{
    VoidFuncPtr TXD_high;
    VoidFuncPtr TXD_low;
    
    uint32_t        baud;                       ///< Simulation Usart's baud
    uint32_t        EXTI_LineX;                 ///< 
    uint8_t         *dataReceived;
    
    
    U8FuncPtr       RXD;
    TIM_TypeDef*    TIMx_Send;                  ///< 模拟串口发送:用于产生1bit的延时
    TIM_TypeDef*    TIMx_Recv;                  ///< 模拟串口接收:用于产生1bit的中断，定时去读取RXD上的电平
    TIM_TypeDef*    TIMx_RecvOver;              ///< 模拟串口接收:接收到一个完整的字节数据之后，开启,它的作用是判断是否接收到了全部的数据
}_SL_USART;
extern _SL_USART SL_USART;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
/* 注册函数 */
void reg_sl_usart_txd_cbfunc            ( VoidFuncPtr    txd_high, VoidFuncPtr txd_low  );
void reg_sl_usart_rxd_cbfunc            ( U8FuncPtr      rxd                            );

/* 中断回调函数 */
void EXTIx_IRQHandler_cbfunc            (void);
void TIMx_Recv_IRQHandler_cbfunc        (void);
void TIMx_Recv_Over_IRQHandler_cbfunc   (void);

/* 串口收发函数 */
void        SL_USART_SendOneByte( uint8_t datatoSend );
void        SL_USART_SendBytes  ( uint8_t *send_datas,uint32_t send_len );
uint32_t    SL_USART_RecvBytes  ( uint8_t *readBuffer,uint32_t Buf_len );

#endif
