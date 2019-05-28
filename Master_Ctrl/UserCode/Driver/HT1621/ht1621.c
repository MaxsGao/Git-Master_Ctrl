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


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "ht1621.h"
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/
void    ht16xx_cs_select(void)          {};
void    ht16xx_cs_deselect(void)        {};
void    ht16xx_wr_high(void)            {};
void    ht16xx_wr_low(void)             {};
void    ht16xx_rd_high(void)            {};
void    ht16xx_rd_low(void)             {};
uint8_t ht16xx_data_in(void)            {
    return 0;
};
void    ht16xx_data_out(uint8_t data)   {};
void    ht16xx_delay_ms(uint16_t num_ms){};


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
_HT1621 HT16xx={
    .ht_mode        = HT_NOR_MODE,
    .CS._select     = NULL,
    .CS._deselect   = NULL,
    .WR._high       = NULL,
    .WR._low        = NULL,
    .RD._high       = NULL,
    .RD._low        = NULL,
    .DATA._in       = NULL,
    .DATA._out      = NULL,
    .delay_ms       = NULL
};


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  从高位开始发送n个位
  * @param  data: 8位待发送数据
  * @param  num_bits: 待发送位数
  * @note   用于发送模式指令、命令指令和地址指令，如HT16xx_send_highBit(0x05<<5,3)
  * @retval None
  */
void HT16xx_send_highBit(uint8_t data,uint8_t num_bits)
{
    uint8_t send_data = data;
    for( uint8_t i=0;i<num_bits;i++ )
    {
        HT16xx.WR._low();
        HT16xx.delay_ms(2);
        
        if( send_data&0x80 )
            HT16xx.DATA._out(1);
        else
            HT16xx.DATA._out(0);
        HT16xx.delay_ms(2);
        
        HT16xx.WR._high();
        HT16xx.delay_ms(2);
        
        send_data<<=1;
    }
}

/**
  * @brief  从低位开始发送n个位
  * @param  ht_mode: 8位待发送数据
  * @param  num_bits: 待发送位数
  * @note   用于发送数据，HT16xx_send_highBit(0x07,4)
  * @retval None
  */
void HT16xx_send_lowBit(uint8_t data,uint8_t num_bits)
{
    uint8_t send_data = data;
    for( uint8_t i=0;i<num_bits;i++ )
    {
        HT16xx.WR._low();
        HT16xx.delay_ms(2);
        
        if( send_data&0x01 )
            HT16xx.DATA._out(1);
        else
            HT16xx.DATA._out(0);
        HT16xx.delay_ms(2);
        
        HT16xx.WR._high();
        HT16xx.delay_ms(2);
        
        send_data>>=1;
    }
}

/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
void HT16xx_conf_func(int num_args,...)
{
    va_list ap;
    uint8_t cmd_code;
    
    va_start(ap,num_args);
    
    HT16xx.CS._select();
    
    HT16xx_write_mode(HT_CMD_MODE);
    for( int i=0;i<num_args;i++ )
    {
        cmd_code = (uint8_t)va_arg( ap,int );
        HT16xx_write_cmd(cmd_code);
    }
    
    HT16xx.CS._deselect();
    
    va_end(ap);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
void HT16xx_test(uint8_t data)
{
    HT16xx.CS._select();
    
    HT16xx_write_mode(HT_WR_MODE);
    HT16xx_write_addr(0x05);
//    for( uint8_t i=0;i<19;i++ )
//    {
        HT16xx_write_data(data);
//    }
    
    HT16xx.CS._deselect();
}

/**
  * @brief  Register call back function of HT16xx select & deselect
  * @param  cs_sel: callback function of the HT16xx select
  * @param  cs_desel: callback function of the HT16xx deselect
  * @retval None
  * @todo Describe @ref ht16xx_cs_select and @ref ht16xx_cs_deselect function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_ht16xx_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void))
{
    if( !cs_sel || !cs_desel ) {
        HT16xx.CS._select     = ht16xx_cs_select;
        HT16xx.CS._deselect   = ht16xx_cs_deselect;
    } else {
        HT16xx.CS._select     = cs_sel;
        HT16xx.CS._deselect   = cs_desel;
    }
}

/**
  * @brief  Register call back function of HT16xx WR High & WR Low
  * @param  wr_high: callback function of the HT16xx WR High
  * @param  wr_low: callback function of the HT16xx WR Low
  * @retval None
  * @todo Describe @ref ht16xx_wr_high and @ref ht16xx_wr_low function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_ht16xx_wr_cbfunc(void(*wr_high)(void), void(*wr_low)(void))
{
    if( !wr_high || !wr_low ) {
        HT16xx.WR._high     = ht16xx_wr_high;
        HT16xx.WR._low      = ht16xx_wr_low;
    } else {
        HT16xx.WR._high     = wr_high;
        HT16xx.WR._low      = wr_low;
    }
}

/**
  * @brief  Register call back function of NRF24L01 RD High & RD Low
  * @param  rd_high: callback function of the HT16xx RD High
  * @param  rd_low: callback function of the HT16xx RD Low
  * @retval None
  * @todo Describe @ref ht16xx_rd_high and @ref ht16xx_rd_low function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_ht16xx_rd_cbfunc(void(*rd_high)(void), void(*rd_low)(void))
{
    if( !rd_high || !rd_low ) {
        HT16xx.RD._high     = ht16xx_rd_high;
        HT16xx.RD._low      = ht16xx_rd_low;
    } else {
        HT16xx.RD._high     = rd_high;
        HT16xx.RD._low      = rd_low;
    }
}

/**
  * @brief  Register call back function of NRF24L01 Data In & Data Out
  * @param  data_in: callback function of the HT16xx Data In
  * @param  data_out: callback function of the HT16xx Data Out
  * @retval None
  * @todo Describe @ref ht16xx_data_in and @ref ht16xx_data_out function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_ht16xx_data_cbfunc(uint8_t(*data_in)(void), void(*data_out)(uint8_t data))
{
    if( !data_in || !data_out ) {
        HT16xx.DATA._in     = ht16xx_data_in;
        HT16xx.DATA._out    = ht16xx_data_out;
    } else {
        HT16xx.DATA._in     = data_in;
        HT16xx.DATA._out    = data_out;
    }
}

/**
  * @brief  Register call back function of NRF24L01 delay ms
  * @param  delay_ms: callback function of the HT16xx delay ms
  * @retval None
  * @todo Describe @ref ht16xx_delay_ms function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_ht16xx_delayms_cbfunc(void(*delay_ms)(uint16_t num_ms))
{
    if( !delay_ms ) {
        HT16xx.delay_ms     = ht16xx_delay_ms;
    } else {
        HT16xx.delay_ms     = delay_ms;
    }
}

