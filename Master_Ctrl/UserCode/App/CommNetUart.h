/*
*********************************************************************************************************
*
*
*                                   Communicate with net uart
*                                        �����紮��ͨ��
* File : CommNetUart.h
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : �˹���ģ����ͨ��ESP8266ģ�飬��ʵ�ִ���ת����ͨ��
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __COMMNETUART_H
#define   __COMMNETUART_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif

/**
 * @brief ����������صĺ궨��
 * @{
 */
#define UartConfigToNetUart USART3_Config
#define UartIDToNetUart     USART3
#define UartPortToNetUart   UART_PORT_3
/**
  * @}
  */

/**
 * @brief ����������صĺ궨��
 * @{
 */
typedef enum{
    Projector   = 0,        ///< ͶӰ�ǿ��ƽڵ�
    Monitor     = 1,        ///< ������ʾ�����ƽڵ�
    Video       = 2,        ///< ������ƽڵ�
    Camera      = 3,        ///< ���龵ͷ���ƽڵ�
    LampSwitch  = 4,        ///< �ƹ⿪�ؿ��ƽڵ�
    LampAdjust  = 5,        ///< �ƹ���ڿ��ƽڵ�
    Curtain     = 6,        ///< �������ƽڵ�
    Screen      = 7,        ///< Ļ�����ƽڵ�
    Sensor      = 8,        ///< ���������ƽڵ�
}NODETYPE;
/**
  * @}
  */

/**
 * @brief ö�����Ͷ���˵��
 * @{
 */
typedef enum{
    enum0   							= 0,        ///< ö�ٳ���0
    enum1								= 1,        ///< ö�ٳ���1
    enum2								= 2,        ///< ö�ٳ���2
}ENUM;
/**
  * @}
  */
  
  
/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
#define DECL_NETUART_CMD_FUNC(name) void net_uart_cmd_##name(void* cmd)
#define ADDS_NETUART_CMD_FUNC(name) #name,net_uart_cmd_##name

typedef void (*deal_func_net_uart_cmd) (void* cmd);
typedef struct{
    const char* cmd_str;
    deal_func_net_uart_cmd pfun;
}net_uart_cmd_ctrl;


/** 
 * @brief  CAN init structure definition
 */
//typedef struct
//{
//  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum. 
//                                 It ranges from 1 to 1024. */
//  
//  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
//                                 This parameter can be a value of 
//                                @ref CAN_operating_mode */
//} CAN_InitTypeDef;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
int CommNetUart_Init(void);


#ifdef __cplusplus
}
#endif

#endif
