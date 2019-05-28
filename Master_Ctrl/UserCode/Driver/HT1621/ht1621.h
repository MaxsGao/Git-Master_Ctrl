/*
*********************************************************************************************************
*
*
*                                   HT1261 segment code LCD driver
*                                         HT1261段码液晶驱动
* File : ht1261.h
* By   : MaxsGao
* Date : 2019/03/12
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __HT1621_H
#define   __HT1621_H

#ifdef __cplusplus
 extern "C" {
#endif


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


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

/**
 * @brief HT16xx Command Code {
 */
#define SYS_DIS     0x00    ///< 关闭系统振荡器和LCD偏压发生器
#define SYS_EN      0x01    ///< 打开系统振荡器
#define LCD_OFF     0x02    ///< 关闭LCD偏压发生器
#define LCD_ON      0x03    ///< 打开LCD偏压发生器
#define TIMER_DIS   0x04    ///< 时基输出失效
#define WDT_DIS     0x05    ///< WDT溢出标志输出失效
#define TIMER_EN    0x06    ///< 时基输出使能
#define WDT_EN      0x07    ///< WDT溢出标志输出使能
#define TONE_OFF    0x08    ///< 关闭声音输出
#define TONE_ON     0x09    ///< 打开声音输出
#define CLR_TIMER   0x0C    ///< 时基发生器清零
#define CLR_WDT     0x0E    ///< 清除WDT状态
#define XTAL_32K    0x14    ///< 系统时钟源，晶振
#define RC_256K     0x18    ///< 系统时钟源，片内RC振荡器
#define EXT_256K    0x1C    ///< 系统时钟源，外部时钟源
#define BIAS_1_2    0x28    ///< LCD1/2偏压选项，4个公共口
#define BIAS_1_3    0x29    ///< LCD1/3偏压选项，4个公共口
#define TONE_4K     0x40    ///< 声音频率，4kHz
#define TONE_2K     0x60    ///< 声音频率，2kHz
#define IRQ_DIS     0x80    ///< 使IRQ输出失效
#define IRQ_EN      0x88    ///< 使IRQ输出有效
#define F1          0xA0    ///< 时基/WDT时钟输出1Hz
#define F2          0xA1    ///< 时基/WDT时钟输出2Hz
#define F4          0xA2    ///< 时基/WDT时钟输出4Hz
#define F8          0xA3    ///< 时基/WDT时钟输出8Hz
#define F16         0xA4    ///< 时基/WDT时钟输出16Hz
#define F32         0xA5    ///< 时基/WDT时钟输出32Hz
#define F64         0xA6    ///< 时基/WDT时钟输出64Hz
#define F128        0xA7    ///< 时基/WDT时钟输出128Hz
#define TOPT        0xE0    ///< 测试模式
#define TNORMAL     0xE3    ///< 普通模式
/**
 * }
 */
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
/**
 * @brief 各种枚举定义
 */
typedef enum
{
    HT_NOR_MODE     = 0x00,     ///< 
    HT_CMD_MODE     = 0x04,     ///< Command Mode
    HT_WR_MODE      = 0x05,     ///< Write Mode
    HT_WR_RD_MODE   = 0x05,     ///< Read-Modify-Write Mode
    HT_RD_MODE      = 0x06,     ///< Read Mode
}HTMode_TypeDef;
#define IS_HTMODE(MODE) (((MODE) == HT_CMD_MODE)    || \
                          ((MODE) == HT_WR_MODE)    || \
                          ((MODE) == HT_WR_RD_MODE) || \
                          ((MODE) == HT_RD_MODE)

typedef void (*HT16xxDelayHandle) (uint16_t num_ms);

/**
 * @brief   HT16xx init structure definition
 */
typedef struct __HT1621
{
    HTMode_TypeDef  ht_mode;                    ///< HT16xx's Operating mode
    
    /**
    * The set of chip select callback func.
    */
    struct
    {
        void    (*_select)      (void);         ///< chip select
        void    (*_deselect)    (void);         ///< chip deselect
    }CS;
    
    /**
    * The set of chip select callback func.
    */
    struct
    {
        void    (*_high)        (void);
        void    (*_low)         (void);
    }WR;
    
    /**
    * The set of chip select callback func.
    */
    struct
    {
        void    (*_high)        (void);
        void    (*_low)         (void);
    }RD;
    
    /**
    * The set of chip select callback func.
    */
    struct
    {
        uint8_t (*_in)         (void);
        void    (*_out)        (uint8_t data);
    }DATA;
    
    HT16xxDelayHandle           delay_ms;
}_HT1621;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
/* Registration of callback function */
void reg_ht16xx_cs_cbfunc       ( void(*cs_sel)(void), void(*cs_desel)(void));
void reg_ht16xx_wr_cbfunc       ( void(*wr_high)(void), void(*wr_low)(void));
void reg_ht16xx_rd_cbfunc       ( void(*rd_high)(void), void(*rd_low)(void));
void reg_ht16xx_data_cbfunc     ( uint8_t(*data_in)(void), void(*data_out)(uint8_t data));
void reg_ht16xx_delayms_cbfunc  ( void(*delay_ms)(uint16_t num_ms));

/* Function macro definition */
#define HT16xx_write_mode(ht_mode)  {HT16xx_send_highBit( ht_mode<<5,3 );}      ///< issue mode command code
#define HT16xx_write_cmd(cmd)       {HT16xx_send_highBit( cmd,9 );}             ///< issue command code
#define HT16xx_write_addr(addr)     {HT16xx_send_highBit( addr<<2,6 );}         ///< issue data write address
#define HT16xx_write_data(data)     {HT16xx_send_lowBit( data,4 );}             ///< issue data

/* HT1621 Functional configuration */
void HT16xx_conf_func           ( int num_args,... );

void HT16xx_test                (uint8_t data);

#ifdef __cplusplus
}
#endif

#endif
