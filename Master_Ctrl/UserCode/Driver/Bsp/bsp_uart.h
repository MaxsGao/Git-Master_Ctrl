#ifndef  __BSP_UART_H
#define  __BSP_UART_H
#include "bsp_sys.h"

#define BAUD_RATE_2400                  2400
#define BAUD_RATE_4800                  4800
#define BAUD_RATE_9600                  9600
#define BAUD_RATE_19200                 19200
#define BAUD_RATE_38400                 38400
#define BAUD_RATE_57600                 57600
#define BAUD_RATE_115200                115200
#define BAUD_RATE_230400                230400
#define BAUD_RATE_460800                460800
#define BAUD_RATE_921600                921600
#define BAUD_RATE_2000000               2000000
#define BAUD_RATE_3000000               3000000

//串口通信方式
typedef enum
{
	UART_TTL_EN = 0,	//TTL
	UART_485_EN = 1,	//485
    UART_422_EN = 2     //422
}UartCommType;

//模式控制，使用时需根据电路原理修改使能引脚
#define RS485_TX_EN     PDout(0)    //RS485 发送使能引脚控制

typedef void (*VoidFuncPtr)     (void);
typedef void (*VoidParaFuncPtr) (void*);
/************************************************* 
*类型名:		UartRecvHandle
*描述：   串口接收数据后的处理函数指针，每个字节的处理函数
*       涉及到的函数，通过uart_config注册到串口接收中断处理操作中
*       涉及到的函数，执行时间不能未知，且尽量短暂
*************************************************/
typedef void (*UartRecvHandle) (u8 recv_data);

typedef struct
{
    //下来两个变量在初始化后不能被修改
    USART_TypeDef*  pRegs;              //寄存器开始地址
    u8              u8_NVIC_IRQChannel;
    
    UartRecvHandle  recv_handle;        //接收中断函数
    
    UartCommType    CommType;           //通信方式，TTL、485、422...
    
    //与发送相关的变量
    u8 *            p_tx_buf;               
    VoidParaFuncPtr send_finish_cb;
    u16             tx_buf_len;         //发送缓冲区大小
    __IO u16        i_to_send;          //填充新数据的位置
    __IO u16        i_yet_send;         //已经发送的 位置
}UartDriverCtrl;
extern UartDriverCtrl DriverUart1;
extern UartDriverCtrl DriverUart2;
extern UartDriverCtrl DriverUart3;
extern UartDriverCtrl DriverUart4;
extern UartDriverCtrl DriverUart5;

//为了和之前的串口驱动保持一致，增加下面的宏定义
#define UART_PORT_1    &DriverUart1
#define UART_PORT_2    &DriverUart2
#define UART_PORT_3    &DriverUart3
#define UART_PORT_4    &DriverUart4
#define UART_PORT_5    &DriverUart5

void USART_Config(USART_TypeDef* USARTx,u32 baud);

//串口默认的IO初始化函数
void USART1_Config(void);
void USART2_Config(void);
void USART3_Config(void);
void USART4_Config(void);
void USART5_Config(void);
/************************************************* 
*Function:		Uart_Config
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  配置串口参数，主要是波特率；注册串口接收字节的回调函数
*************************************************/
void Uart_Config(
    VoidFuncPtr     uart_io_init_fun,   //串口IO口初始化函数,可以使用 USART?_Config 函数
    UartDriverCtrl* uart,               //UartDriverCtrl* uart，使用 DriverUart?
    UartCommType    commtype,           //串口通信方式，TTL、485、422
    u32             baud,               //u32 baud  串口被配置的波特率
    u8*             pSendBuf,           //发送缓冲区 地址 发送缓冲区由外部提供 必须提供，否则会调用出先问题
    u32             nSendBufLen,        //发送缓冲区 的 大小
    UartRecvHandle  recv_handle         //UartRecvHandle recv_handle  串口接收一个字节的中断中对接收数据的处理函数
    );

/************************************************* 
*Function:		uart_set_send_finish_handle
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               VoidParaFuncPtr send_ok_handle 发送完成的回调函数，这里发送完成，指将最后一个字节写入发送缓冲区中
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  配置串口发送完成的回调函数，这里的回调函数只会执行一次，机所有的数据发送完毕 然后会自动清除
*           若有需要，函数执行过程再次自动设置
*************************************************/
void uart_set_send_finish_handle(UartDriverCtrl* uart,VoidParaFuncPtr send_ok_handle);

/************************************************* 
*Function:		uart_send_byte
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               u8 tosend     需要发送的数据
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  发送一个字节数据
*************************************************/
u32 uart_send_byte(UartDriverCtrl* uart,u8 tosend);

/************************************************* 
*Function:		uart_send_bytes
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               const u8* sdata     需要发送的数据的开始地址
*               u32 send_len  需要发送的数据长度
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  发送固定长度的串口数据
*************************************************/
u32 uart_send_bytes(UartDriverCtrl* uart,const u8* sdata,u32 send_len);

/************************************************* 
*Function:		uart_get_need_send_cnt
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  获取当前发送缓冲区中 但是还没有发送出去的数据量
*************************************************/
u32 uart_get_need_send_cnt(UartDriverCtrl* uart);

//获取可用的缓冲区大小
u32 uart_get_send_buf_useful_cnt(UartDriverCtrl* uart);

/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               const char* str   需要发送的字符串数据
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  通过串口发送一个字符串
*************************************************/
u32 uart_send_string(UartDriverCtrl* uart,const char* str);

//清空需要发送的数据
void uart_clear_sending_data(UartDriverCtrl* uart);

/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               const char* str   需要发送的字符串数据
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  通过串口发送一个字符串
*************************************************/
void uart_send_format(UartDriverCtrl* uart,const char *Data,...);

#endif

