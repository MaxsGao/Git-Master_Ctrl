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

//����ͨ�ŷ�ʽ
typedef enum
{
	UART_TTL_EN = 0,	//TTL
	UART_485_EN = 1,	//485
    UART_422_EN = 2     //422
}UartCommType;

//ģʽ���ƣ�ʹ��ʱ����ݵ�·ԭ���޸�ʹ������
#define RS485_TX_EN     PDout(0)    //RS485 ����ʹ�����ſ���

typedef void (*VoidFuncPtr)     (void);
typedef void (*VoidParaFuncPtr) (void*);
/************************************************* 
*������:		UartRecvHandle
*������   ���ڽ������ݺ�Ĵ�����ָ�룬ÿ���ֽڵĴ�����
*       �漰���ĺ�����ͨ��uart_configע�ᵽ���ڽ����жϴ��������
*       �漰���ĺ�����ִ��ʱ�䲻��δ֪���Ҿ�������
*************************************************/
typedef void (*UartRecvHandle) (u8 recv_data);

typedef struct
{
    //�������������ڳ�ʼ�����ܱ��޸�
    USART_TypeDef*  pRegs;              //�Ĵ�����ʼ��ַ
    u8              u8_NVIC_IRQChannel;
    
    UartRecvHandle  recv_handle;        //�����жϺ���
    
    UartCommType    CommType;           //ͨ�ŷ�ʽ��TTL��485��422...
    
    //�뷢����صı���
    u8 *            p_tx_buf;               
    VoidParaFuncPtr send_finish_cb;
    u16             tx_buf_len;         //���ͻ�������С
    __IO u16        i_to_send;          //��������ݵ�λ��
    __IO u16        i_yet_send;         //�Ѿ����͵� λ��
}UartDriverCtrl;
extern UartDriverCtrl DriverUart1;
extern UartDriverCtrl DriverUart2;
extern UartDriverCtrl DriverUart3;
extern UartDriverCtrl DriverUart4;
extern UartDriverCtrl DriverUart5;

//Ϊ�˺�֮ǰ�Ĵ�����������һ�£���������ĺ궨��
#define UART_PORT_1    &DriverUart1
#define UART_PORT_2    &DriverUart2
#define UART_PORT_3    &DriverUart3
#define UART_PORT_4    &DriverUart4
#define UART_PORT_5    &DriverUart5

void USART_Config(USART_TypeDef* USARTx,u32 baud);

//����Ĭ�ϵ�IO��ʼ������
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
*DESCRIPTION:  ���ô��ڲ�������Ҫ�ǲ����ʣ�ע�ᴮ�ڽ����ֽڵĻص�����
*************************************************/
void Uart_Config(
    VoidFuncPtr     uart_io_init_fun,   //����IO�ڳ�ʼ������,����ʹ�� USART?_Config ����
    UartDriverCtrl* uart,               //UartDriverCtrl* uart��ʹ�� DriverUart?
    UartCommType    commtype,           //����ͨ�ŷ�ʽ��TTL��485��422
    u32             baud,               //u32 baud  ���ڱ����õĲ�����
    u8*             pSendBuf,           //���ͻ����� ��ַ ���ͻ��������ⲿ�ṩ �����ṩ���������ó�������
    u32             nSendBufLen,        //���ͻ����� �� ��С
    UartRecvHandle  recv_handle         //UartRecvHandle recv_handle  ���ڽ���һ���ֽڵ��ж��жԽ������ݵĴ�����
    );

/************************************************* 
*Function:		uart_set_send_finish_handle
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               VoidParaFuncPtr send_ok_handle ������ɵĻص����������﷢����ɣ�ָ�����һ���ֽ�д�뷢�ͻ�������
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ���ô��ڷ�����ɵĻص�����������Ļص�����ֻ��ִ��һ�Σ������е����ݷ������ Ȼ����Զ����
*           ������Ҫ������ִ�й����ٴ��Զ�����
*************************************************/
void uart_set_send_finish_handle(UartDriverCtrl* uart,VoidParaFuncPtr send_ok_handle);

/************************************************* 
*Function:		uart_send_byte
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               u8 tosend     ��Ҫ���͵�����
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ����һ���ֽ�����
*************************************************/
u32 uart_send_byte(UartDriverCtrl* uart,u8 tosend);

/************************************************* 
*Function:		uart_send_bytes
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               const u8* sdata     ��Ҫ���͵����ݵĿ�ʼ��ַ
*               u32 send_len  ��Ҫ���͵����ݳ���
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ���͹̶����ȵĴ�������
*************************************************/
u32 uart_send_bytes(UartDriverCtrl* uart,const u8* sdata,u32 send_len);

/************************************************* 
*Function:		uart_get_need_send_cnt
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ��ȡ��ǰ���ͻ������� ���ǻ�û�з��ͳ�ȥ��������
*************************************************/
u32 uart_get_need_send_cnt(UartDriverCtrl* uart);

//��ȡ���õĻ�������С
u32 uart_get_send_buf_useful_cnt(UartDriverCtrl* uart);

/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               const char* str   ��Ҫ���͵��ַ�������
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ͨ�����ڷ���һ���ַ���
*************************************************/
u32 uart_send_string(UartDriverCtrl* uart,const char* str);

//�����Ҫ���͵�����
void uart_clear_sending_data(UartDriverCtrl* uart);

/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               const char* str   ��Ҫ���͵��ַ�������
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ͨ�����ڷ���һ���ַ���
*************************************************/
void uart_send_format(UartDriverCtrl* uart,const char *Data,...);

#endif

