/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission
*                                         无线数传
* File : nrf24l01.h
* By   : 
* Date : 2018/12/6
* version : V0.1
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __NRF24L01_H
#define   __NRF24L01_H


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"


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

/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */
/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#define _Si24R1  

/**
 * @brief NRF24L01 Command Operation
 */
#define R_REGISTER              0x00        ///< 读配置寄存器,低5位为读操作的寄存器地址
#define W_REGISTER              0x20        ///< 写配置寄存器,低5位为寄存器地址
#define R_RX_PAYLOAD            0x61        ///< 读RX有效数据,1~32字节
#define W_TX_PAYLOAD            0xA0        ///< 写TX有效数据,1~32字节
#define FLUSH_TX                0xE1        ///< 清除TX FIFO寄存器,发射模式下用
#define FLUSH_RX                0xE2        ///< 清除RX FIFO寄存器,接收模式下用
#define REUSE_TX_PL             0xE3        ///< 重新使用上一包数据,CE为高,数据包被不断发送
#ifdef _Si24R1  ///< {
    #define R_RX_PL_WID         0x60        ///< 读取收到的数据字节数
    #define W_ACK_PAYLOAD       0xA8        ///< 适用于接收方，通过PIPE PPP将数据通过ACK的形式发出去，最多允许三帧数据存于FIFO中
    #define W_TX_PAYLOAD_NOACK  0xB0        ///< 适用于发射模式，使用这个命令同时需要将AUTOACK位置1  
#endif  ///< }
#define NOP                     0xFF        ///< 空操作,可以用来读状态寄存器	

//--------------------------------------Registers ADDR----------------------------------------------------//
/**
 * @brief 配置寄存器
 */
#define CONFIG          0x00 
typedef enum
{
    CONF_MASK_RX_DR     = (1<<6),   ///< 接收中断屏蔽控制
    CONF_MASK_TX_DS     = (1<<5),   ///< 发射中断屏蔽控制
    CONF_MASK_MAX_RT    = (1<<4),   ///< 最大重发计数中断屏蔽控制
    CONF_EN_CRC         = (1<<3),   ///< 使能CRC，如果EN_AA不全为零时，EN_CRC必须为1
    CONF_CRCO           = (1<<2),   ///< CRC长度配置，0：1byte，1：2byte
    CONF_PWR_UP         = (1<<1),   ///< 关断/开机模式配置
    CONF_PRIM_RX        = (1<<0),   ///< 发射/接收配置，只能在Shutdown和Standby下更改，0：接收模式，1：发射模式
}config_type;

/**
 * @brief 使能自动确认
 */
#define EN_AA           0x01
typedef enum
{
    ENAA_P5             = (1<<5),   ///< 使能数据管道5自动确认
    ENAA_P4             = (1<<4),   ///< 使能数据管道4自动确认
    ENAA_P3             = (1<<3),   ///< 使能数据管道3自动确认
    ENAA_P2             = (1<<2),   ///< 使能数据管道2自动确认
    ENAA_P1             = (1<<1),   ///< 使能数据管道1自动确认
    ENAA_P0             = (1<<0),   ///< 使能数据管道0自动确认
}en_aa_type;

/**
 * @brief 使能接收数据管道地址
 */
#define EN_RXADDR       0x02  //接收地址允许,bit0~5,对应通道0~5
typedef enum
{
    ERX_P5              = (1<<5),   ///< 使能数据管道5
    ERX_P4              = (1<<4),   ///< 使能数据管道4
    ERX_P3              = (1<<3),   ///< 使能数据管道3
    ERX_P2              = (1<<2),   ///< 使能数据管道2
    ERX_P1              = (1<<1),   ///< 使能数据管道1
    ERX_P0              = (1<<0),   ///< 使能数据管道0
}en_rxaddr_type;

/**
 * @brief 地址宽度配置(所有数据通道)
 */
#define SETUP_AW        0x03
typedef enum
{
    AW                  = (11<<0),   ///< 发射方/接收方地址宽度，00：错误值，01：3bytes，10：4bytes，11:5bytes
}setup_aw_type;

/**
 * @brief 自动重发配置
 */
#define SETUP_RETR      0x04
typedef enum
{  
    RETR_ARD            = (1111<<4),   ///< 自动重发延时配置
    RETR_ARC            = (1111<<0),   ///< 最大自动重发次数
}setup_retr_type;

/**
 * @brief 射频信道，设置芯片工作时的信道，分别对应1~125个信道，信道间隔为1MHz，默认为02即2402MHz
 */
#define RF_CH           0x05

/**
 * @brief 射频配置
 */
#define RF_SETUP        0x06
typedef enum
{  
    RF_CONT_WAVE        = (1<<7),   ///< 为‘1’时，使能恒载波发射模式
    RF_DR_LOW           = (1<<5),   ///< 设置射频数据率，与RF_DR_HIGH共同控制
    RF_PLL_LOCK         = (1<<4),   ///< 保留字，必须为0
    RF_DR_HIGH          = (1<<3),   ///< 设置射频数据率，[LOW，HIGH]：00-1Mbps，01-2Mbps，10-250kbps，11-保留
    RF_PWR              = (110<<0), ///< 设置TX发射功率，111:7dBm，110:4dBm，101:3dBm，100:1dBm，011:0dBm，010:-4dBm，001:-6dBm，000:-12dBm，
}rf_setup_type;

/**
 * @brief 状态寄存器（SPI操作开始，状态寄存器值通过MISO串行输出）
 */
#define STATUS          0x07
typedef enum
{  
    STA_RX_DR           = (1<<6),   ///< 接收数据中断
    STA_TX_DS           = (1<<5),   ///< 数据发送完成中断
    STA_MAX_RT          = (1<<4),   ///< 达到最多次重发
    STA_RX_P_NO         = (111<<3), ///< 接收数据通道号(最大:6)
    STA_TX_FULL         = (1<<0),   ///< TX FIFO满标志
}status_type;

/**
 * @brief 发射结果统计
 */
#define OBSERVE_TX      0x08
typedef enum
{
    TX_PLOS_CNT         = (1111<<4),    ///< 丢包计数，最大计数为15，改变RF_CH后PLOS_CNT从0开始计数
    TX_ARC_CNT          = (1111<<0),    ///< 重发计数，发射一个新包时，ARC_CNT从0开始计数
}observe_tx_type;

/**
 * @brief 载波检测，接收信号强度检测
 */
#ifdef _Si24R1  ///< {
    #define RSSI
    typedef enum
    {
        RF_RSSI         = (1<<0),   ///< 接收信号强度，0：接收信号小于-60dBm
    }rssi_type;
#else
    #define CD              0x09
    typedef enum
    {  
        RF_CD           = (1<<0),   ///< 载波检测
    }cd_type;
#endif  ///< }

/**
 * @brief 数据管道0的接收地址，最大宽度为5bytes（LSByte最先写入，通过SETUP_AW配置地址宽度）
 */
#define RX_ADDR_P0      0x0A

/**
 * @brief 数据管道1的接收地址，最大宽度为5bytes（LSByte最先写入，通过SETUP_AW配置地址宽度）
 */
#define RX_ADDR_P1      0x0B

/**
 * @brief 数据管道2的接收地址的最低字节，接收地址高字节与RX_ADDR_P1[39:8]相同
 */
#define RX_ADDR_P2      0x0C

/**
 * @brief 数据管道3的接收地址的最低字节，接收地址高字节与RX_ADDR_P1[39:8]相同
 */
#define RX_ADDR_P3      0x0D

/**
 * @brief 数据管道4的接收地址的最低字节，接收地址高字节与RX_ADDR_P1[39:8]相同
 */
#define RX_ADDR_P4      0x0E

/**
 * @brief 数据管道5的接收地址的最低字节，接收地址高字节与RX_ADDR_P1[39:8]相同
 */
#define RX_ADDR_P5      0x0F

/**
 * @brief 发射方的发射地址（LSByte最先写入），如果发射方需要收ACK确认信号，则需要配置RX_ADDR_P0的值等于TX_ADDR，并使能ARQ
 */
#define TX_ADDR         0x10

/**
 * @brief 接收数据管道0数据字节数（1-32Bytes）
 */
#define RX_PW_P0        0x11

/**
 * @brief 接收数据管道1数据字节数（1-32Bytes）
 */
#define RX_PW_P1        0x12

/**
 * @brief 接收数据管道2数据字节数（1-32Bytes）
 */
#define RX_PW_P2        0x13

/**
 * @brief 接收数据管道3数据字节数（1-32Bytes）
 */
#define RX_PW_P3        0x14

/**
 * @brief 接收数据管道4数据字节数（1-32Bytes）
 */
#define RX_PW_P4        0x15

/**
 * @brief 接收数据管道5数据字节数（1-32Bytes）
 */
#define RX_PW_P5        0x16

/**
 * @brief FIFO状态
 */
#define FIFO_STATUS     0x17
typedef enum
{  
    FIFO_TX_REUSE       = (1<<6),   ///< 只用于发射端，FIFO数据重新利用，当用RESUSE_TX_PL命令后，发射上次已成功发射的数据，通过W_TX_PAYLOAD或FLUSH_TX命令关闭该功能
    FIFO_TX_FULL        = (1<<5),   ///< TX FIFO满标志，1：TX FIFO满，0：TX FIFO可写
    FIFO_TX_EMPTY       = (1<<4),   ///< TX FIFO空标志，1：TX FIFO为空，0：TX FIFO有数据
    FIFO_RX_FULL        = (1<<1),   ///< RX FIFO满标志，1：RX FIFO满，0：RX FIFO可写
    FIFO_RX_EMPTY       = (1<<0),   ///< RX FIFO空标志，1：RX FIFO为空，0：RX FIFO有数据
}fifo_type;

#ifdef _Si24R1  ///< {
    /**
     * @brief 使能动态负载长度
     */
        #define DYNPD       0x1C
    typedef enum
    {  
        DPL_P5              = (1<<5),   ///< 使能接收管道5动态负载长度（需EN_DPL及ENAA_P5）
        DPL_P4              = (1<<4),   ///< 使能接收管道4动态负载长度（需EN_DPL及ENAA_P4）
        DPL_P3              = (1<<3),   ///< 使能接收管道3动态负载长度（需EN_DPL及ENAA_P3）
        DPL_P2              = (1<<2),   ///< 使能接收管道2动态负载长度（需EN_DPL及ENAA_P2）
        DPL_P1              = (1<<1),   ///< 使能接收管道1动态负载长度（需EN_DPL及ENAA_P1）
        DPL_P0              = (1<<0),   ///< 使能接收管道0动态负载长度（需EN_DPL及ENAA_P0）
    }dynpd_type;

    /**
     * @brief 特征寄存器
     */
        #define FEATURE     0x1D
        typedef enum
        {  
            EN_DPL          = (1<<6),   ///< 使能动态负载长度
            EN_ACK_PAYD     = (1<<5),   ///< 使能ACK负载（带负载数据的ACK包）
            EN_DYN_ACK      = (1<<4),   ///< 使能命令W_TX_PAYLOAD_NOACK
        }feature_type;
#endif  ///< }

//--------------------------------------NRF24L01 Error----------------------------------------------------//
/**
 * @brief 各种枚举定义
 */
typedef enum
{
    NRF_NONRF           = 0x00,     ///< 未检测到模块时的----模块不存在模式
    NRF_CHECKED         = 0x01,     ///< 上电检查模块通信正常后的----检查完毕模式
    NRF_SHUTDOWN        = 0x02,     ///< 检查通信正常后的----关断模式
    NRF_STANDBY         = 0x03,     ///< 初始化完成后的----待机模式
    NRF_IDLETX          = 0x04,     ///< 发射空闲模式
    NRF_TX              = 0x05,     ///< 发射模式
    NRF_RX              = 0x06,     ///< 接收模式
}NRFMode;
#define IS_NRFMODE(MODE) (((MODE) == NRF_NONRF) || \
                          ((MODE) == NRF_CHECKED) || \
                          ((MODE) == NRF_SHUTDOWN) || \
                          ((MODE) == NRF_STANDBY) || \
                          ((MODE) == NRF_IDLETX) || \
                          ((MODE) == NRF_TX) || \
                          ((MODE) == NRF_RX))

typedef enum
{
    NOACK   = 0x00,
    ACK     = 0x01,
}AckMode;
#define IS_ACKMODE(MODE) ( ((MODE) == ACK) || ((MODE) == NOACK) )

typedef enum {STA_DISABLE = 0, STA_ENABLE = !STA_DISABLE} StartState;
#define IS_START_STATE(STATE) (((STATE) == STA_DISABLE) || ((STATE) == STA_ENABLE))
        
typedef enum
{
    //标准错误定义
    NRF24L01_OK                         = (0x00), /*!<  */
    NRF24L01_ERROR                      = (0x01), /*!<  */
	//特殊错误定义 
    NRF24L01_CHECK_FAIL                 = (0x02), /*!<  */
    NRF24L01_CHECK_NOTALLOW             = (0x03), /*!<  */
    NRF24L01_TX_MAX                     = (0x04), /*!<  */
    NRF24L01_RX_OK                      = (0x05), /*!<  */
    NRF24L01_WAIT_TX_DS                 = (0x06), /*!<  */
    NRF24L01_ADDR_WIDTH_ERROR           = (0x07), /*!<  */
}NRF24L01_Error;

//24L01 宽度定义
#define ADDR_WIDTH      5       ///< 5字节的地址宽度
#define PLOAD_WIDTH     32      ///< 32字节的用户数据宽度


/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
typedef void (*VoidFuncPtr)     (void);
//typedef void (*VoidParaFuncPtr) (void*);
/************************************************* 
*类型名:		UartRecvHandle
*描述：   串口接收数据后的处理函数指针，每个字节的处理函数
*       涉及到的函数，通过uart_config注册到串口接收中断处理操作中
*       涉及到的函数，执行时间不能未知，且尽量短暂
*************************************************/
typedef void (*NrfRxHandle) (uint8_t* pRx_buf,uint8_t rx_len);

/** 
 * @brief   NRF24L01 发射队列单元的结构体定义
 */
#define UNIT_BUF_MAX_SIZE     64
typedef struct __NRF_TX_QUEUE_UNIT
{
    AckMode ack_mode;                   ///< 响应模式
    uint8_t addr[5];                    ///< 远端地址
    uint8_t buf[UNIT_BUF_MAX_SIZE];     ///< 发射缓冲区
    uint8_t iTx;                        ///< 待发射位置
    uint8_t nLast;                      ///< 剩余数量
}_NRF_TX_QUEUE_UNIT;

/** 
 * @brief   NRF24L01 init structure definition
 */
typedef struct __NRF24L01
{
    NRFMode     mode;                               ///< NRF24L01's Operating mode
    uint8_t     rx_addr[5];                         ///< Local Address
    
    /**
    * The set of chip enable callback func.
    */
    struct
    {
        void    (*_en)              (void);         ///< chip enable
        void    (*_dis)             (void);         ///< chip disable
    }CE;
    /**
    * The set of critical section callback func.
    */
    struct
    {
        void    (*_enter)           (void);         ///< critical section enter 
        void    (*_exit)            (void);         ///< critical section exit  
    }CRIS;
    /**
    * The set of chip select callback func.
    */
    struct
    {
        void    (*_select)          (void);         ///< chip select
        void    (*_deselect)        (void);         ///< chip deselect
    }CS;
    struct
    {
        void    (*_set_speed)       (void);
        uint8_t (*_read_byte)       (void);
        void    (*_write_byte)      (uint8_t wb);
    }SPI;
    struct
    {
        VoidFuncPtr _trigger;                       ///< trigger interrupt
    }IRQ;        
    
    /**
    * 与发送相关的变量
    */
    Queue               tx_queue;                       ///< TX Queue
    _NRF_TX_QUEUE_UNIT  tx_unit;
    uint8_t             tx_addr[5];                     ///< Trans Address
    AckMode             ackmode;                        ///< Ack Mode
    StartState          sta_state;
    
    /**
    * 与接收相关的变量
    */
    uint8_t         rx_len;
    uint8_t         rx_buf[PLOAD_WIDTH];            ///< RX Buffer
    NrfRxHandle     _rx_handle;                     ///< 接收中断函数
}_NRF24L01;
extern _NRF24L01 NRF24L01;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
void reg_nrf_ce_cbfunc      ( VoidFuncPtr ce_en,    VoidFuncPtr ce_dis );
void reg_nrf_cris_cbfunc    ( VoidFuncPtr cris_en,  VoidFuncPtr cris_ex );
void reg_nrf_cs_cbfunc      ( VoidFuncPtr cs_sel,   VoidFuncPtr cs_desel );
void reg_nrf_spi_cbfunc     ( VoidFuncPtr spi_ss,   uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb) );
void reg_nrf_irq_cbfunc     ( VoidFuncPtr irq_trigger );
void reg_nrf_rx_handle_cbfunc( NrfRxHandle rx_handle );

/* 接收相关 */
NRF24L01_Error NRF24L01_RX          ( __IN__ const AckMode ack_mode );
NRF24L01_Error NRF24L01_SET_RxAddr  ( __IN__ const uint8_t *pRXAddr,__IN__ const uint8_t addr_width );

/* 发射相关 */
NRF24L01_Error NRF24L01_TX_Init     ( __OUT_ _NRF_TX_QUEUE_UNIT* pTXUnitArray,__IN__ const uint16_t array_size );
NRF24L01_Error NRF24L01_TX_Buffer   ( __IN__ const AckMode ack_mode,__IN__ const uint8_t* pTXAddr,__OUT_ uint8_t* pTXBuf,__OUT_ uint8_t tx_len );



NRF24L01_Error NRF24L01_IRQ_oper(void);

#endif
