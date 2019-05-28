/*
*********************************************************************************************************
*
*
*                                         AT24Cxx EEPROM driver
*                                         AT24Cxx EEPROM驱动程序
* File : at24cxx.h
* By   : MaxsGao
* Date : 2019/03/18
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __AT24Cxx_H
#define   __AT24Cxx_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdint.h>
#include "at24cxx_conf.h"


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

#define _WIZCHIP_IO_MODE_IIC_HARD_  0x00
#define _WIZCHIP_IO_MODE_IIC_SOFT_  0x01

#define AT24C01		127
#define AT24C02		255
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767
#define AT24C512    65535

#if     (_AT24CXX_ == AT24C01)
    #define _AT24Cxx_Toal_SIZE_     128
    #define _AT24Cxx_Page_SIZE_     8
    #define _AT24Cxx_Page_NUM_      16
    
#elif   (_AT24CXX_ == AT24C02)
    #define _AT24Cxx_Toal_SIZE_     256
    #define _AT24Cxx_Page_SIZE_     8
    #define _AT24Cxx_Page_NUM_      32
    
#elif   (_AT24CXX_ == AT24C04)
    #define _AT24Cxx_Toal_SIZE_     512
    #define _AT24Cxx_Page_SIZE_     16
    #define _AT24Cxx_Page_NUM_      32
    
#elif   (_AT24CXX_ == AT24C08)
    #define _AT24Cxx_Toal_SIZE_     1024
    #define _AT24Cxx_Page_SIZE_     16
    #define _AT24Cxx_Page_NUM_      64
    
#elif   (_AT24CXX_ == AT24C16)
    #define _AT24Cxx_Toal_SIZE_     2048
    #define _AT24Cxx_Page_SIZE_     16
    #define _AT24Cxx_Page_NUM_      128
    
#elif   (_AT24CXX_ == AT24C32)
    #define _AT24Cxx_Toal_SIZE_     4096
    #define _AT24Cxx_Page_SIZE_     32
    #define _AT24Cxx_Page_NUM_      128
    
#elif   (_AT24CXX_ == AT24C64)
    #define _AT24Cxx_Toal_SIZE_     8192
    #define _AT24Cxx_Page_SIZE_     32
    #define _AT24Cxx_Page_NUM_      256
    
#elif   (_AT24CXX_ == AT24C128)
    #define _AT24Cxx_Toal_SIZE_     16384
    #define _AT24Cxx_Page_SIZE_     64
    #define _AT24Cxx_Page_NUM_      256
    
#elif   (_AT24CXX_ == AT24C256)
    #define _AT24Cxx_Toal_SIZE_     32768
    #define _AT24Cxx_Page_SIZE_     64
    #define _AT24Cxx_Page_NUM_      512
    
#elif   (_AT24CXX_ == AT24C512)
    #define _AT24Cxx_Toal_SIZE_     65536
    #define _AT24Cxx_Page_SIZE_     128
    #define _AT24Cxx_Page_NUM_      512
#else
    #error "Unknown defined _AT24CXX_.You shoule define one of 64,128,256,512"
#endif

#define AT24CXX_BASE        ((uint16_t)0x0000)
#define AT24CXX_Page2Addr(page) (uint16_t)(AT24CXX_BASE+((page)*_AT24Cxx_Page_SIZE_))
#define AT24CXX_Addr2Page(addr) (uint16_t)(((uint16_t)addr-AT24CXX_BASE)/_AT24Cxx_Page_SIZE_)

/**
 * @brief Peer IP register address
 */
typedef enum
{
    //标准错误定义
    AT24CXX_OK                          = (0x00), /*!<  */
    AT24CXX_ERROR                       = (0x01), /*!<  */
	//特殊错误定义 
    AT24CXX_CHECK_FAIL                  = (0x02), /*!<  */
    AT24CXX_OVER_PAGE_SIZE              = (0x03),
    AT24CXX_OVER_PAGE_NUM               = (0x04),
    AT24CXX_READ_PAGE_FAIL              = (0x05),
    AT24CXX_WRITE_PAGE_FAIL             = (0x06),
    AT24CXX_WAIT_TIMEOUT                = (0x0F), /*!<  */
}AT24CXX_Error;
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
/** 
 * @brief  AT24Cxx init structure definition
 */
typedef struct __AT24Cxx
{
    uint32_t  toal_size;      ///< 
    
    union
    {
        struct
        {
            void (*_start)(void);
            void (*_stop)(void);
            void (*_writeByte)(uint8_t txd);
            uint8_t (*_readByte)(uint8_t ack);
            uint8_t (*_waitAck)(void);
            void (*_ack)(void);
            void (*_nack)(void);
        }IIC_SOFT;
        
        struct
        {
            void (*_sendBuf)(uint8_t txd);
            uint8_t (*_readBuf)(uint8_t ack);
        }IIC_HARD;
    }IF;
}_AT24Cxx;
extern _AT24Cxx at24cxx;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
void reg_at24cxx_sig_cbfunc ( void(*sig_start)(void),    void(*sig_stop)(void)                                  );
void reg_at24cxx_iic_cbfunc ( void(*iic_wb)(uint8_t txd),uint8_t(*iic_rb)(uint8_t ack)                          );
void reg_at24cxx_ack_cbfunc ( uint8_t(*ack_wait)(void),  void(*ack_send)(void),         void(*ack_nsend)(void)  );

AT24CXX_Error AT24CXX_ReadOneByte   ( uint16_t read_addr, uint8_t* read_byte);
AT24CXX_Error AT24CXX_WriteOneByte  ( uint16_t write_addr,uint8_t write_byte);

AT24CXX_Error AT24CXX_ReadBytes     ( uint16_t read_addr, uint8_t* read_bytes, uint16_t bytes_num );
AT24CXX_Error AT24CXX_WriteBytes    ( uint16_t write_addr,uint8_t* write_bytes,uint16_t bytes_num );

AT24CXX_Error AT24CXX_ReadPages     ( uint16_t read_page, uint8_t* read_bytes, uint16_t pages_num );
AT24CXX_Error AT24CXX_WritePages    ( uint16_t write_page,uint8_t* write_bytes,uint16_t pages_num );

AT24CXX_Error AT24CXX_Check(void);


#ifdef __cplusplus
}
#endif

#endif
