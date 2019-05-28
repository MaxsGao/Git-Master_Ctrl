/*
*********************************************************************************************************
*
*
*                                   HT1261 segment code LCD driver
*                                         HT1261����Һ������
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>


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
#define SYS_DIS     0x00    ///< �ر�ϵͳ������LCDƫѹ������
#define SYS_EN      0x01    ///< ��ϵͳ����
#define LCD_OFF     0x02    ///< �ر�LCDƫѹ������
#define LCD_ON      0x03    ///< ��LCDƫѹ������
#define TIMER_DIS   0x04    ///< ʱ�����ʧЧ
#define WDT_DIS     0x05    ///< WDT�����־���ʧЧ
#define TIMER_EN    0x06    ///< ʱ�����ʹ��
#define WDT_EN      0x07    ///< WDT�����־���ʹ��
#define TONE_OFF    0x08    ///< �ر��������
#define TONE_ON     0x09    ///< ���������
#define CLR_TIMER   0x0C    ///< ʱ������������
#define CLR_WDT     0x0E    ///< ���WDT״̬
#define XTAL_32K    0x14    ///< ϵͳʱ��Դ������
#define RC_256K     0x18    ///< ϵͳʱ��Դ��Ƭ��RC����
#define EXT_256K    0x1C    ///< ϵͳʱ��Դ���ⲿʱ��Դ
#define BIAS_1_2    0x28    ///< LCD1/2ƫѹѡ�4��������
#define BIAS_1_3    0x29    ///< LCD1/3ƫѹѡ�4��������
#define TONE_4K     0x40    ///< ����Ƶ�ʣ�4kHz
#define TONE_2K     0x60    ///< ����Ƶ�ʣ�2kHz
#define IRQ_DIS     0x80    ///< ʹIRQ���ʧЧ
#define IRQ_EN      0x88    ///< ʹIRQ�����Ч
#define F1          0xA0    ///< ʱ��/WDTʱ�����1Hz
#define F2          0xA1    ///< ʱ��/WDTʱ�����2Hz
#define F4          0xA2    ///< ʱ��/WDTʱ�����4Hz
#define F8          0xA3    ///< ʱ��/WDTʱ�����8Hz
#define F16         0xA4    ///< ʱ��/WDTʱ�����16Hz
#define F32         0xA5    ///< ʱ��/WDTʱ�����32Hz
#define F64         0xA6    ///< ʱ��/WDTʱ�����64Hz
#define F128        0xA7    ///< ʱ��/WDTʱ�����128Hz
#define TOPT        0xE0    ///< ����ģʽ
#define TNORMAL     0xE3    ///< ��ͨģʽ
/**
 * }
 */
  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
/**
 * @brief ����ö�ٶ���
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
*                                       FUNCTION STATEMENT  ��������
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
