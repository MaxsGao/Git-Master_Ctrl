/*
*********************************************************************************************************
*
*
*                                   Communicate with net uart
*                                        与网络串口通信
* File : CommNetUart.h
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : 此功能模块是通过ESP8266模块，以实现串口转网络通信
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //参数作为输入
#endif
#ifndef  __OUT_
#define  __OUT_      //参数作为输出
#endif

/**
 * @brief 串口配置相关的宏定义
 * @{
 */
#define UartConfigToNetUart USART3_Config
#define UartIDToNetUart     USART3
#define UartPortToNetUart   UART_PORT_3
/**
  * @}
  */

/**
 * @brief 串口配置相关的宏定义
 * @{
 */
typedef enum{
    Projector   = 0,        ///< 投影仪控制节点
    Monitor     = 1,        ///< 大屏显示器控制节点
    Video       = 2,        ///< 矩阵控制节点
    Camera      = 3,        ///< 会议镜头控制节点
    LampSwitch  = 4,        ///< 灯光开关控制节点
    LampAdjust  = 5,        ///< 灯光调节控制节点
    Curtain     = 6,        ///< 窗帘控制节点
    Screen      = 7,        ///< 幕布控制节点
    Sensor      = 8,        ///< 传感器控制节点
}NODETYPE;
/**
  * @}
  */

/**
 * @brief 枚举类型定义说明
 * @{
 */
typedef enum{
    enum0   							= 0,        ///< 枚举常量0
    enum1								= 1,        ///< 枚举常量1
    enum2								= 2,        ///< 枚举常量2
}ENUM;
/**
  * @}
  */
  
  
/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
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
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
int CommNetUart_Init(void);


#ifdef __cplusplus
}
#endif

#endif
