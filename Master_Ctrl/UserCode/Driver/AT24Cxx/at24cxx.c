/*
*********************************************************************************************************
*
*
*                                         AT24Cxx EEPROM driver
*                                         AT24Cxx EEPROM驱动程序
* File : at24cxx.c
* By   : MaxsGao
* Date : 2019/03/18
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
#include "at24cxx.h"
#include "bsp_delay.h"
#include "string.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/
void    at24cxx_iic_soft_start(void)        {};
void    at24cxx_iic_soft_stop(void)         {};
void    at24cxx_iic_soft_wd(uint8_t txd)    {};
uint8_t at24cxx_iic_soft_rd(uint8_t ack)    {return 0;};
uint8_t at24cxx_iic_soft_waitAck(void)      {return 0;};
void    at24cxx_iic_soft_ack(void)          {};
void    at24cxx_iic_soft_nack(void)         {};


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/

///** @defgroup SPI_Private_Defines
//  * @{
//  */

///* SPI SPE mask */
//#define CR1_SPE_Set          ((uint16_t)0x0040)
//#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

///* I2S I2SE mask */
//#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
//#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

///**
//  * @}
//  */


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
_AT24Cxx at24cxx;

//SD_ReadDisk/SD_WriteDisk函数专用buf,当这两个函数的数据缓存区地址不是4字节对齐的时候,
//需要用到该数组,确保数据缓存区地址是4字节对齐的.
uint8_t PAGE_DATA_BUFFER[_AT24Cxx_Page_NUM_];


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */


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
void reg_at24cxx_sig_cbfunc (
    void(*sig_start)(void),
    void(*sig_stop)(void)
)
{
    if( !sig_start || !sig_start ) {
        at24cxx.IF.IIC_SOFT._start = at24cxx_iic_soft_start;
        at24cxx.IF.IIC_SOFT._stop  = at24cxx_iic_soft_stop;
    } else {
        at24cxx.IF.IIC_SOFT._start = sig_start;
        at24cxx.IF.IIC_SOFT._stop  = sig_stop;
    }
}

void reg_at24cxx_iic_cbfunc (
    void(*iic_wb)(uint8_t txd),
    uint8_t(*iic_rb)(uint8_t ack)
)
{
    if( !iic_wb || !iic_rb ) {
        at24cxx.IF.IIC_SOFT._writeByte = at24cxx_iic_soft_wd;
        at24cxx.IF.IIC_SOFT._readByte  = at24cxx_iic_soft_rd;
    } else {
        at24cxx.IF.IIC_SOFT._writeByte = iic_wb;
        at24cxx.IF.IIC_SOFT._readByte  = iic_rb;
    }
}

void reg_at24cxx_ack_cbfunc (
    uint8_t(*ack_wait)(void),
    void(*ack_send)(void),
    void(*ack_nsend)(void)
)
{
    if( !ack_wait || !ack_send || !ack_nsend ) {
        at24cxx.IF.IIC_SOFT._waitAck = at24cxx_iic_soft_waitAck;
        at24cxx.IF.IIC_SOFT._ack     = at24cxx_iic_soft_ack;
        at24cxx.IF.IIC_SOFT._nack    = at24cxx_iic_soft_nack;
    } else {
        at24cxx.IF.IIC_SOFT._waitAck = ack_wait;
        at24cxx.IF.IIC_SOFT._ack     = ack_send;
        at24cxx.IF.IIC_SOFT._nack    = ack_nsend;
    }
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
AT24CXX_Error AT24CXX_ReadOneByte (
    uint16_t read_addr,
    uint8_t* read_byte
)
{
    uint8_t Rsp;
	
    at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行写数据
    if ( _AT24CXX_ > AT24C16 ) {
        at24cxx.IF.IIC_SOFT._writeByte(0xA0);               ///> 发送写命令
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if ( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        at24cxx.IF.IIC_SOFT._writeByte( read_addr>>8 );     ///> 发送写地址高八位
    }
    else {
        at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((read_addr/256)<<1) );    ///> 发送器件地址0xA0,写数据
    }
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if ( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    at24cxx.IF.IIC_SOFT._writeByte( read_addr%256 );        ///> 发送写地址低八位
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if ( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行读数据
    at24cxx.IF.IIC_SOFT._writeByte( 0xA1 );                 ///> 发送读命令
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if ( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    *read_byte = at24cxx.IF.IIC_SOFT._readByte(0);
    at24cxx.IF.IIC_SOFT._stop();
    
    return AT24CXX_OK;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
AT24CXX_Error AT24CXX_WriteOneByte (
    uint16_t write_addr,
    uint8_t write_byte
)
{
    uint8_t Rsp;
    
    at24cxx.IF.IIC_SOFT._start();
    if ( _AT24CXX_ > AT24C16 ) {
        at24cxx.IF.IIC_SOFT._writeByte( 0xA0 );
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        at24cxx.IF.IIC_SOFT._writeByte( write_addr>>8 );
    }
    else {
       at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((write_addr/256)<<1) ); 
    }
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    at24cxx.IF.IIC_SOFT._writeByte( write_addr%256 );
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    at24cxx.IF.IIC_SOFT._writeByte( write_byte );
    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
    if( Rsp!=0 )
        return AT24CXX_WAIT_TIMEOUT;
    
    at24cxx.IF.IIC_SOFT._stop();
	delay_ms(10);
    return AT24CXX_OK;
}

AT24CXX_Error AT24CXX_ReadBytes (
    uint16_t read_addr,
    uint8_t* read_bytes,
    uint16_t bytes_num
)
{
    uint8_t  Rsp;
    uint16_t i,n_read;
    uint16_t curt_addr,next_addr;
    uint16_t curt_page,next_page;
    uint16_t bytes_cnt=0;
    
    if ( (read_addr+bytes_num)>_AT24CXX_ )
        return AT24CXX_OVER_PAGE_SIZE;
    
    curt_addr = read_addr;
    curt_page = AT24CXX_Addr2Page(curt_addr);
    
    next_page = curt_page+1;
    next_addr = AT24CXX_Page2Addr(next_page);
    
    if ( (next_addr-curt_addr)<(bytes_num-bytes_cnt) )
        n_read = next_addr-curt_addr;
    else
        n_read = bytes_num-bytes_cnt;
    
    while ( n_read ) {
        /****************************************/
        at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行写数据
        if ( _AT24CXX_ > AT24C16 ) {
            at24cxx.IF.IIC_SOFT._writeByte(0xA0);               ///> 发送写命令
            Rsp = at24cxx.IF.IIC_SOFT._waitAck();
            if ( Rsp!=0 )
                return AT24CXX_WAIT_TIMEOUT;
            
            at24cxx.IF.IIC_SOFT._writeByte( curt_addr>>8 );     ///> 发送写地址高八位
        }
        else {
            at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((curt_addr/256)<<1) );    ///> 发送器件地址0xA0,写数据
        }
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if ( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        at24cxx.IF.IIC_SOFT._writeByte( curt_addr%256 );        ///> 发送写地址低八位
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if ( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        
        at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行读数据
        at24cxx.IF.IIC_SOFT._writeByte( 0xA1 );                 ///> 发送读命令
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if ( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        for ( i=0;i<n_read-1;i++ ) {
            read_bytes[bytes_cnt++] = at24cxx.IF.IIC_SOFT._readByte(1);
        }
        read_bytes[bytes_cnt++] = at24cxx.IF.IIC_SOFT._readByte(0);
        at24cxx.IF.IIC_SOFT._stop();
        /****************************************/
        
        curt_addr = next_addr;
        curt_page = next_page;
        
        next_page = curt_page+1;
        next_addr = AT24CXX_Page2Addr(next_page);
        
        if ( (next_addr-curt_addr)<(bytes_num-bytes_cnt) )
            n_read = next_addr-curt_addr;
        else
            n_read = bytes_num-bytes_cnt;
    }
    
    return AT24CXX_OK;
}

AT24CXX_Error AT24CXX_WriteBytes (
    uint16_t write_addr,
    uint8_t* write_bytes,
    uint16_t bytes_num
)
{
    uint8_t  Rsp;
    uint16_t i,n_write;
    uint16_t curt_addr,next_addr;
    uint16_t curt_page,next_page;
    uint16_t bytes_cnt=0;
    
    if ( (write_addr+bytes_num)>_AT24CXX_ )
        return AT24CXX_OVER_PAGE_SIZE;
    
    curt_addr = write_addr;
    curt_page = AT24CXX_Addr2Page(curt_addr);
    
    next_page = curt_page+1;
    next_addr = AT24CXX_Page2Addr(next_page);
    
    if ( (next_addr-curt_addr)<(bytes_num-bytes_cnt) )
        n_write = next_addr-curt_addr;
    else
        n_write = bytes_num-bytes_cnt;
    
    while ( n_write ) {
        /**************************************/
        at24cxx.IF.IIC_SOFT._start();
        if ( _AT24CXX_ > AT24C16 ) {
            at24cxx.IF.IIC_SOFT._writeByte( 0xA0 );
            Rsp = at24cxx.IF.IIC_SOFT._waitAck();
            if( Rsp!=0 )
                return AT24CXX_WAIT_TIMEOUT;
            
            at24cxx.IF.IIC_SOFT._writeByte( write_addr>>8 );
        }
        else {
           at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((write_addr/256)<<1) ); 
        }
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        at24cxx.IF.IIC_SOFT._writeByte( write_addr%256 );
        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
        if( Rsp!=0 )
            return AT24CXX_WAIT_TIMEOUT;
        
        for ( i=0;i<n_write;i++ ) {
            at24cxx.IF.IIC_SOFT._writeByte( write_bytes[bytes_cnt++] );
            Rsp = at24cxx.IF.IIC_SOFT._waitAck();
            if( Rsp!=0 )
                return AT24CXX_WAIT_TIMEOUT;
        }
        
        at24cxx.IF.IIC_SOFT._stop();
        delay_ms(10);
        /**************************************/
        
        curt_addr = next_addr;
        curt_page = next_page;
        
        next_page = curt_page+1;
        next_addr = AT24CXX_Page2Addr(next_page);
        
        if ( (next_addr-curt_addr)<(bytes_num-bytes_cnt) )
            n_write = next_addr-curt_addr;
        else
            n_write = bytes_num-bytes_cnt;
    }
    
    return AT24CXX_OK;
}

AT24CXX_Error AT24CXX_ReadPages(
    uint16_t read_page,
    uint8_t* read_bytes,
    uint16_t pages_num
)
{
    AT24CXX_Error res = AT24CXX_OK;
    
    if ( (read_page+pages_num)>_AT24Cxx_Page_NUM_ )
        return AT24CXX_OVER_PAGE_NUM;
    
    res = AT24CXX_ReadBytes( AT24CXX_Page2Addr(read_page),read_bytes,pages_num*_AT24Cxx_Page_SIZE_ );
    
    return res;
}

AT24CXX_Error AT24CXX_WritePages(
    uint16_t write_page,
    uint8_t* write_bytes,
    uint16_t pages_num
)
{
    AT24CXX_Error res = AT24CXX_OK;
    
    if ( (write_page+pages_num)>_AT24Cxx_Page_NUM_ )
        return AT24CXX_OVER_PAGE_NUM;
    
    res = AT24CXX_WriteBytes( AT24CXX_Page2Addr(write_page),write_bytes,pages_num*_AT24Cxx_Page_SIZE_ );
    
    return res;
}
///**
//  * @brief  
//  * @param  
//  * @note   
//  * @retval 
//  */
//AT24CXX_Error AT24CXX_ReadOnePage (
//    uint16_t page_num,
//    uint8_t* read_bytes,
//    uint16_t bytes_num
//)
//{
//    uint8_t     Rsp;
//    uint16_t    i,n_read;
//    uint16_t    read_addr; 
//    
//    if ( page_num>_AT24Cxx_Page_NUM_ )
//        return AT24CXX_OVER_PAGE_NUM;
//    read_addr = AT24CXX_Page2Addr(page_num);
//    
//    if ( bytes_num>_AT24Cxx_Page_SIZE_ )
//        return AT24CXX_OVER_PAGE_SIZE;
//    n_read = bytes_num;
//    
//    
//    at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行写数据
//    if ( _AT24CXX_ > AT24C16 ) {
//        at24cxx.IF.IIC_SOFT._writeByte(0xA0);               ///> 发送写命令
//        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//        if ( Rsp!=0 )
//            return AT24CXX_WAIT_TIMEOUT;
//        
//        at24cxx.IF.IIC_SOFT._writeByte( read_addr>>8 );     ///> 发送写地址高八位
//    }
//    else {
//        at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((read_addr/256)<<1) );    ///> 发送器件地址0xA0,写数据
//    }
//    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//    if ( Rsp!=0 )
//        return AT24CXX_WAIT_TIMEOUT;
//    
//    at24cxx.IF.IIC_SOFT._writeByte( read_addr%256 );        ///> 发送写地址低八位
//    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//    if ( Rsp!=0 )
//        return AT24CXX_WAIT_TIMEOUT;
//    
//    
//    at24cxx.IF.IIC_SOFT._start();                           ///> 发送起始信号，接下来将进行读数据
//    at24cxx.IF.IIC_SOFT._writeByte( 0xA1 );                 ///> 发送读命令
//    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//    if ( Rsp!=0 )
//        return AT24CXX_WAIT_TIMEOUT;
//    
//    for ( i=0;i<n_read-1;i++ ) {
//        read_bytes[i] = at24cxx.IF.IIC_SOFT._readByte(1);
//    }
//    read_bytes[i] = at24cxx.IF.IIC_SOFT._readByte(0);
//    at24cxx.IF.IIC_SOFT._stop();
//    
//    return AT24CXX_OK;
//}

///**
//  * @brief  
//  * @param  
//  * @note   
//  * @retval 
//  */
//AT24CXX_Error AT24CXX_WriteOnePage (
//    uint16_t page_num,
//    uint8_t* write_bytes,
//    uint16_t bytes_num
//)
//{
//    uint8_t     Rsp;
//    uint16_t    n_write;
//    uint16_t    write_addr;
//    
//    if ( page_num>_AT24Cxx_Page_NUM_ )
//        return AT24CXX_OVER_PAGE_NUM;
//    write_addr = AT24CXX_Page2Addr(page_num);
//    
//    if ( bytes_num>_AT24Cxx_Page_SIZE_ )
//        return AT24CXX_OVER_PAGE_SIZE;
//    n_write = bytes_num;
//    
//    at24cxx.IF.IIC_SOFT._start();
//    if ( _AT24CXX_ > AT24C16 ) {
//        at24cxx.IF.IIC_SOFT._writeByte( 0xA0 );
//        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//        if( Rsp!=0 )
//            return AT24CXX_WAIT_TIMEOUT;
//        
//        at24cxx.IF.IIC_SOFT._writeByte( write_addr>>8 );
//    }
//    else {
//       at24cxx.IF.IIC_SOFT._writeByte( 0xA0+((write_addr/256)<<1) ); 
//    }
//    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//    if( Rsp!=0 )
//        return AT24CXX_WAIT_TIMEOUT;
//    
//    at24cxx.IF.IIC_SOFT._writeByte( write_addr%256 );
//    Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//    if( Rsp!=0 )
//        return AT24CXX_WAIT_TIMEOUT;
//    
//    for ( uint16_t i=0;i<n_write;i++ ) {
//        at24cxx.IF.IIC_SOFT._writeByte( write_bytes[i] );
//        Rsp = at24cxx.IF.IIC_SOFT._waitAck();
//        if( Rsp!=0 )
//            return AT24CXX_WAIT_TIMEOUT;
//    }
//    
//    at24cxx.IF.IIC_SOFT._stop();
//	delay_ms(10);
//    
//    return AT24CXX_OK;
//}


AT24CXX_Error AT24CXX_Check(void)
{
    AT24CXX_Error res;
    uint8_t m_byte,temp0,temp1;
    uint8_t write_str[20] = "hello world!";
    uint8_t read_str[20];
    
    res = AT24CXX_ReadOneByte( _AT24CXX_,&m_byte );     ///> 读取最后一个字节的数据并记录，用于恢复
    if ( res!=AT24CXX_OK )
        return res;
    
    
    temp0 = 0xFF-m_byte;
    res = AT24CXX_WriteOneByte( _AT24CXX_,temp0 );      ///> 写数据
    if ( res!=AT24CXX_OK )
        return res;
    
    res = AT24CXX_ReadOneByte( _AT24CXX_,&temp1 );      ///> 读数据
    if ( res!=AT24CXX_OK )
        return res;
    
    if ( temp0!=temp1 )                                 ///> 比较读写数据是否一致
//        return AT24CXX_CHECK_FAIL;
    
    
    res = AT24CXX_WriteOneByte( _AT24CXX_,m_byte );     ///> 恢复原数据
    if ( res!=AT24CXX_OK )
        return res;
    
    /* TEST CODE BEGIN 1 */
    res = AT24CXX_WriteBytes( _AT24CXX_-15,write_str,15 );
    if ( res!=AT24CXX_OK )
        return res;
    
    res = AT24CXX_ReadBytes( _AT24CXX_-15,read_str,15 );
    if ( res!=AT24CXX_OK )
        return res;
    
    if ( strcmp((char*)write_str,(char*)read_str) )
        return AT24CXX_CHECK_FAIL;
    /* TEST CODE END 1 */
    
    return AT24CXX_OK;
}

