/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission
*                                         无线数传
* File : nrf24l01.c
* By   :
* Date : 2018/12/6
* version : V0.1
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
#include "nrf24l01.h"
#include "bsp_delay.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/
uint8_t NRF24L01_Read_Byte  (__IN__ const uint8_t reg);
void    NRF24L01_Write_Byte (__IN__ const uint8_t reg,__IN__ const uint8_t value);
void    NRF24L01_Read_Buf   (__IN__ const uint8_t reg,__OUT_       uint8_t *pBuf,__IN__ const uint8_t len);
void    NRF24L01_Write_Buf  (__IN__ const uint8_t reg,__IN__ const uint8_t *pBuf,__IN__ const uint8_t len);
NRF24L01_Error NRF24L01_Check(void);
NRF24L01_Error NRF24L01_Shutdown(void);
NRF24L01_Error NRF24L01_Standby(void);
NRF24L01_Error NRF24L01_IdleTX(AckMode tx_mode);
void    nrf_ce_enable(void)             {};
void    nrf_ce_disable(void)            {};
void    nrf_cris_enter(void)            {};
void    nrf_cris_exit(void)             {};
void    nrf_cs_select(void)             {};
void    nrf_cs_deselect(void)           {};
void    nrf_spi_setspeed(void)          {};
uint8_t nrf_spi_readbyte(void)          {
    return 0;
};
void    nrf_spi_writebyte(uint8_t wb)   {};
void    nrf_irq_trigger(void)           {};
void    nrf_rx_handle(uint8_t* pRx_buf,uint8_t rx_len) {};
uint8_t nrf_irq(void)                   {
    return 0;
};


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/

//--------------------------------------SPI Command Oper----------------------------------------------------//
/**
 * @brief 从FIFO中读收到的数据，1-32字节，读出后FIFO数据被删除，适用于接收模式
 */
#define readRX_PAYLOAD(rxbuf,rxlen) \
    NRF24L01_Read_Buf(R_RX_PAYLOAD,rxbuf,rxlen);

/**
 * @brief 写发射负载数据，大小为1-32字节，适用于发射模式
 */
#define writeTX_PAYLOAD(txbuf,txlen) \
    NRF24L01_Write_Buf(W_TX_PAYLOAD,txbuf,txlen);

/**
 * @brief 清空TX FIFO，适用于发射模式
 */
#define writeFLUSH_TX() \
    NRF24L01_Write_Byte(FLUSH_TX,0xFF);

/**
 * @brief 清空RX FIFO，适用于接收模式，如果需要回ACK，则不能在回ACK操作完成前进行清空FIFO，否则视为通信失败
 */
#define writeFLUSH_RX() \
    NRF24L01_Write_Byte(FLUSH_RX,0xFF);

/**
 * @brief 适用于发送方，清空TX FIFO或对FIFO写入新的数据后不能使用该命令
 */
#define writeREUSE_TX_PL() \
    NRF24L01_Write_Byte(REUSE_TX_PL,0xFF);

#ifdef _Si24R1  ///< {
/**
 * @brief 读取收到的字节数
 */
#define readRX_PL_WID() \
        NRF24L01_Read_Byte(R_RX_PL_WID);

/**
 * @brief 适用于接收方，通过PIPE PPP将数据通过ACK的形式发出去，最多允许三帧数据存于FIFO中
 */
#define writeACK_PAYLOAD(pipe) \
        NRF24L01_Write_Byte(W_ACK_PAYLOAD+pipe);

/**
 * @brief 适用于发射模式，使用这个命令同时需要将AUTOACK位置1
 */
#define writeTX_PAYLOAD_NOACK(txbuf,txlen) \
        NRF24L01_Write_Buf(W_TX_PAYLOAD_NOACK,txbuf,txlen);
#endif  ///< }

//--------------------------------------NRF24L01 Register Oper----------------------------------------------------//
/**
 * @brief Set Config Register
 * @param (uint8_t)conf The value to be set.
 * @sa getCONFIG()
 */
#define setCONFIG(conf) \
    NRF24L01_Write_Byte(W_REGISTER+CONFIG,conf);

/**
 * @brief Get Config Register
 * @return uint8_t. The value of Config register.
 * @sa setCONFIG()
 */
#define getCONFIG() \
    NRF24L01_Read_Byte(R_REGISTER+CONFIG)

/**
 * @brief Set EN_AA Register
 * @param (uint8_t)enaa The value to be set.
 * @sa getEN_AA()
 */
#define setEN_AA(enaa) \
    NRF24L01_Write_Byte(W_REGISTER+EN_AA,enaa);

/**
 * @brief Get EN_AA Register
 * @return uint8_t. The value of EN_AA register.
 * @sa setEN_AA()
 */
#define getEN_AA() \
    NRF24L01_Read_Byte(R_REGISTER+EN_AA)

/**
 * @brief Set EN_RXADDR Register
 * @param (uint8_t)rxaddr The value to be set.
 * @sa getEN_RXADDR()
 */
#define setEN_RXADDR(rxaddr) \
    NRF24L01_Write_Byte(W_REGISTER+EN_RXADDR,rxaddr);

/**
 * @brief Get EN_RXADDR Register
 * @return uint8_t. The value of EN_RXADDR register.
 * @sa setEN_RXADDR()
 */
#define getEN_RXADDR() \
    NRF24L01_Read_Byte(R_REGISTER+EN_RXADDR)

/**
 * @brief Set SETUP_AW Register
 * @param (uint8_t)aw The value to be set.
 * @sa getSETUP_AW()
 */
#define setSETUP_AW(aw) \
    NRF24L01_Write_Byte(W_REGISTER+SETUP_AW,aw);

/**
 * @brief Get SETUP_AW Register
 * @return uint8_t. The value of SETUP_AW register.
 * @sa setSETUP_AW()
 */
#define getSETUP_AW() \
    NRF24L01_Read_Byte(R_REGISTER+SETUP_AW)

/**
 * @brief Set SETUP_RETR Register
 * @param (uint8_t)retr The value to be set.
 * @sa getSETUP_RETR()
 */
#define setSETUP_RETR(retr) \
    NRF24L01_Write_Byte(W_REGISTER+SETUP_RETR,retr);

/**
 * @brief Get SETUP_RETR Register
 * @return uint8_t. The value of SETUP_RETR register.
 * @sa setSETUP_RETR()
 */
#define getSETUP_RETR() \
    NRF24L01_Read_Byte(R_REGISTER+SETUP_RETR)

/**
 * @brief Set RF_CH Register
 * @param (uint8_t)ch The value to be set.
 * @sa getRF_CH()
 */
#define setRF_CH(ch) \
    NRF24L01_Write_Byte(W_REGISTER+RF_CH,ch);

/**
 * @brief Get RF_CH Register
 * @return uint8_t. The value of RF_CH register.
 * @sa setRF_CH()
 */
#define getRF_CH() \
    NRF24L01_Read_Byte(R_REGISTER+RF_CH)

/**
 * @brief Set RF_SETUP Register
 * @param (uint8_t)setup The value to be set.
 * @sa getRF_SETUP()
 */
#define setRF_SETUP(setup) \
    NRF24L01_Write_Byte(W_REGISTER+RF_SETUP,setup);

/**
 * @brief Get RF_SETUP Register
 * @return uint8_t. The value of RF_SETUP register.
 * @sa setRF_SETUP()
 */
#define getRF_SETUP() \
    NRF24L01_Read_Byte(R_REGISTER+RF_SETUP)

/**
 * @brief Set STATUS Register
 * @param (uint8_t)sta The value to be set.
 * @sa getSTATUS()
 */
#define setSTATUS(sta) \
    NRF24L01_Write_Byte(W_REGISTER+STATUS,sta);

/**
 * @brief Get STATUS Register
 * @return uint8_t. The value of STATUS register.
 * @sa setSTATUS()
 */
#define getSTATUS() \
    NRF24L01_Read_Byte(R_REGISTER+STATUS)

/**
 * @brief Get OBSERVE_TX Register
 * @return uint8_t. The value of OBSERVE_TX register.
 */
#define getOBSERVE_TX() \
    NRF24L01_Read_Byte(R_REGISTER+OBSERVE_TX)

#ifdef _Si24R1  ///< {
/**
 * @brief Get RSSI Register
 * @return uint8_t. The value of RSSI register.
 */
#define getRSSI() \
        NRF24L01_Read_Byte(R_REGISTER+RSSI)
#else
/**
 * @brief Get CD Register
 * @return uint8_t. The value of RSSI register.
 */
#define getCD() \
        NRF24L01_Read_Byte(R_REGISTER+CD)
#endif  ///< }

/**
 * @brief Set RX_ADDR_P0 Register
 * @param (uint8_t)addr The values to be set.
 * @param (uint8_t)5    The values len.
 * @sa getRX_ADDR_P0()
 */
#define setRX_ADDR_P0(addr) \
    NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P0,addr,ADDR_WIDTH);

/**
 * @brief Get RX_ADDR_P0 Register
 * @param (uint8_t)addr The values of RX_ADDR_P0 register.
 * @param (uint8_t)5    The values len.
 * @sa setRX_ADDR_P0()
 */
#define getRX_ADDR_P0(addr) \
    NRF24L01_Read_Buf(R_REGISTER+RX_ADDR_P0,addr,ADDR_WIDTH)

/**
 * @brief Set RX_ADDR_P1 Register
 * @param (uint8_t)addr The values to be set.
 * @param (uint8_t)5    The values len.
 * @sa getRX_ADDR_P1()
 */
#define setRX_ADDR_P1(addr) \
    NRF24L01_Write_Buf(W_REGISTER+RX_ADDR_P1,addr,ADDR_WIDTH);

/**
 * @brief Get RX_ADDR_P1 Register
 * @param (uint8_t)addr The values of RX_ADDR_P1 register.
 * @param (uint8_t)5    The values len.
 * @sa setRX_ADDR_P1()
 */
#define getRX_ADDR_P1(addr) \
    NRF24L01_Read_Buf(R_REGISTER+RX_ADDR_P1,addr,ADDR_WIDTH)

/**
 * @brief Set RX_ADDR_P2 Register
 * @param (uint8_t)addr_lsbyte The value to be set.
 * @sa getRX_ADDR_P2()
 */
#define setRX_ADDR_P2(addr_lsbyte) \
    NRF24L01_Write_Byte(W_REGISTER+RX_ADDR_P2,addr_lsbyte);

/**
 * @brief Get RX_ADDR_P2 Register
 * @return uint8_t. The value of RX_ADDR_P2 register.
 * @sa setRX_ADDR_P2()
 */
#define getRX_ADDR_P2() \
    NRF24L01_Read_Byte(R_REGISTER+RX_ADDR_P2)

/**
 * @brief Set RX_ADDR_P3 Register
 * @param (uint8_t)addr_lsbyte The value to be set.
 * @sa getRX_ADDR_P3()
 */
#define setRX_ADDR_P3(addr_lsbyte) \
    NRF24L01_Write_Byte(W_REGISTER+RX_ADDR_P3,addr_lsbyte);

/**
 * @brief Get RX_ADDR_P3 Register
 * @return uint8_t. The value of RX_ADDR_P3 register.
 * @sa setRX_ADDR_P3()
 */
#define getRX_ADDR_P3() \
    NRF24L01_Read_Byte(R_REGISTER+RX_ADDR_P3)

/**
 * @brief Set RX_ADDR_P4 Register
 * @param (uint8_t)addr_lsbyte The value to be set.
 * @sa getRX_ADDR_P4()
 */
#define setRX_ADDR_P4(addr_lsbyte) \
    NRF24L01_Write_Byte(W_REGISTER+RX_ADDR_P4,addr_lsbyte);

/**
 * @brief Get RX_ADDR_P4 Register
 * @return uint8_t. The value of RX_ADDR_P4 register.
 * @sa setRX_ADDR_P4()
 */
#define getRX_ADDR_P4() \
    NRF24L01_Read_Byte(R_REGISTER+RX_ADDR_P4)

/**
 * @brief Set RX_ADDR_P5 Register
 * @param (uint8_t)addr_lsbyte The value to be set.
 * @sa getRX_ADDR_P5()
 */
#define setRX_ADDR_P5(addr_lsbyte) \
    NRF24L01_Write_Byte(W_REGISTER+RX_ADDR_P5,addr_lsbyte);

/**
 * @brief Get RX_ADDR_P5 Register
 * @return uint8_t. The value of RX_ADDR_P5 register.
 * @sa setRX_ADDR_P5()
 */
#define getRX_ADDR_P5() \
    NRF24L01_Read_Byte(R_REGISTER+RX_ADDR_P5)

/**
 * @brief Set TX_ADDR Register
 * @param (uint8_t)addr The value to be set.
 * @sa getTX_ADDR()
 */
#define setTX_ADDR(addr) \
    NRF24L01_Write_Buf(W_REGISTER+TX_ADDR,addr,ADDR_WIDTH);

/**
 * @brief Get TX_ADDR Register
 * @return uint8_t. The value of TX_ADDR register.
 * @sa setTX_ADDR()
 */
#define getTX_ADDR(addr) \
    NRF24L01_Read_Buf(R_REGISTER+TX_ADDR,addr,ADDR_WIDTH);

/**
 * @brief Set RX_PW_P0 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P0()
 */
#define setRX_PW_P0(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P0,pw);

/**
 * @brief Get RX_PW_P0 Register
 * @return uint8_t. The value of RX_PW_P0 register.
 * @sa setRX_PW_P0()
 */
#define getRX_PW_P0() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P0)

/**
 * @brief Set RX_PW_P1 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P1()
 */
#define setRX_PW_P1(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P1,pw);

/**
 * @brief Get RX_PW_P1 Register
 * @return uint8_t. The value of RX_PW_P1 register.
 * @sa setRX_PW_P1()
 */
#define getRX_PW_P1() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P1)

/**
 * @brief Set RX_PW_P2 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P2()
 */
#define setRX_PW_P2(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P2,pw);

/**
 * @brief Get RX_PW_P2 Register
 * @return uint8_t. The value of RX_PW_P2 register.
 * @sa setRX_PW_P2()
 */
#define getRX_PW_P2() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P2)

/**
 * @brief Set RX_PW_P3 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P3()
 */
#define setRX_PW_P3(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P3,pw);

/**
 * @brief Get RX_PW_P3 Register
 * @return uint8_t. The value of RX_PW_P3 register.
 * @sa setRX_PW_P3()
 */
#define getRX_PW_P3() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P3)

/**
 * @brief Set RX_PW_P4 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P4()
 */
#define setRX_PW_P4(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P4,pw);

/**
 * @brief Get RX_PW_P4 Register
 * @return uint8_t. The value of RX_PW_P4 register.
 * @sa setRX_PW_P4()
 */
#define getRX_PW_P4() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P4)

/**
 * @brief Set RX_PW_P5 Register
 * @param (uint8_t)pw The value to be set.
 * @sa getRX_PW_P5()
 */
#define setRX_PW_P5(pw) \
    NRF24L01_Write_Byte(W_REGISTER+RX_PW_P5,pw);

/**
 * @brief Get RX_PW_P5 Register
 * @return uint8_t. The value of RX_PW_P5 register.
 * @sa setRX_PW_P5()
 */
#define getRX_PW_P5() \
    NRF24L01_Read_Byte(R_REGISTER+RX_PW_P5)

/**
 * @brief Get FIFO_STATUS Register
 * @return uint8_t. The value of FIFO_STATUS register.
 */
#define getFIFO_STATUS() \
    NRF24L01_Read_Byte(R_REGISTER+FIFO_STATUS)

/**
 * @brief Set DYNPD Register
 * @param (uint8_t)dpl The value to be set.
 * @sa getDYNPD()
 */
#define setDYNPD(dpl) \
    NRF24L01_Write_Byte(W_REGISTER+DYNPD,dpl);

/**
 * @brief Get DYNPD Register
 * @return uint8_t. The value of DYNPD register.
 * @sa setDYNPD()
 */
#define getDYNPD() \
    NRF24L01_Read_Byte(R_REGISTER+DYNPD)

/**
 * @brief Set FEATURE Register
 * @param (uint8_t)fea The value to be set.
 * @sa getFEATURE()
 */
#define setFEATURE(fea) \
    NRF24L01_Write_Byte(W_REGISTER+FEATURE,fea);

/**
 * @brief Get FEATURE Register
 * @return uint8_t. The value of FEATURE register.
 * @sa setFEATURE()
 */
#define getFEATURE() \
    NRF24L01_Read_Byte(R_REGISTER+FEATURE)


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
_NRF24L01 NRF24L01=
{
    .mode               = NRF_NONRF,
    .rx_addr            = { 0x34,0x43,0x10,0x10,0x01 },
    .CE._en             = nrf_ce_enable,
    .CE._dis            = nrf_ce_disable,
    .CRIS._enter        = nrf_cris_enter,
    .CRIS._exit         = nrf_cris_exit,
    .CS._select         = nrf_cs_select,
    .CS._deselect       = nrf_cs_deselect,
    .SPI._set_speed     = nrf_spi_setspeed,
    .SPI._read_byte     = nrf_spi_readbyte,
    .SPI._write_byte    = nrf_spi_writebyte,
    .IRQ._trigger       = nrf_irq_trigger,
    .tx_queue           = { QUEUE_STATUS_ERROR,0,0,0,0,0,0 },
    .tx_unit            = { NOACK,0,0,0,0 },
    .tx_addr            = { 0x00,0x00,0x00,0x00,0x00 },
    .ackmode            = NOACK,
    .sta_state          = STA_DISABLE,
    .rx_len             = 0,
    .rx_buf             = {0},
    ._rx_handle         = nrf_rx_handle
};


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  Read one byte from a specific register
  * @param  reg: The specific register
  * @retval Register value
  * @note   
  */
uint8_t NRF24L01_Read_Byte(
    __IN__ const uint8_t reg
    )
{
    uint8_t reg_val;

    NRF24L01.CRIS._enter();                 ///< 进入临界区
    NRF24L01.SPI._set_speed();              ///< 设置SPI接口速率
    NRF24L01.CS._select();                  ///< 片选NRF24L01

    NRF24L01.SPI._write_byte(reg);          ///< 发送寄存器号
    reg_val = NRF24L01.SPI._read_byte();    ///< 读取寄存器内容

    NRF24L01.CS._deselect();                ///< 取消片选NRF24L01
    NRF24L01.CRIS._exit();                  ///< 退出临界区

    return (reg_val);                       ///< 返回状态值
}

/**
 * @brief   Write one byte to a specific register
 * @param   reg: The specific register
 * @param   value: Register value
 * @retval  None
 * @note   
 */
void NRF24L01_Write_Byte(
    __IN__ const uint8_t reg,
    __IN__ const uint8_t value
    )
{
    NRF24L01.CRIS._enter();                 ///< 进入临界区
    NRF24L01.SPI._set_speed();              ///< 设置SPI接口速率
    NRF24L01.CS._select();                  ///< 片选NRF24L01

    NRF24L01.SPI._write_byte(reg);          ///< 发送寄存器号
    NRF24L01.SPI._write_byte(value);        ///< 写入寄存器的值

    NRF24L01.CS._deselect();                ///< 取消片选NRF24L01
    NRF24L01.CRIS._exit();                  ///< 退出临界区
}

/**
  * @brief  Read multiple bytes from a specific register
  * @param  reg: The specific register
  * @param  pBuf: The buffer of the data to be read
  * @param  len: The length of the data to be read
  * @retval None
  * @note   
  */
void NRF24L01_Read_Buf(
    __IN__ const uint8_t reg,
    __OUT_       uint8_t *pBuf,
    __IN__ const uint8_t len
    )
{
    uint8_t i;

    NRF24L01.CRIS._enter();                 ///< 进入临界区
    NRF24L01.SPI._set_speed();              ///< 设置SPI接口速率
    NRF24L01.CS._select();                  ///< 片选NRF24L01
    NRF24L01.SPI._write_byte(reg);          ///< 发送寄存器号
    for( i=0; i<len; i++ ) {
        pBuf[i]=NRF24L01.SPI._read_byte();  ///<读出数据
    }

    NRF24L01.CS._deselect();                ///< 取消片选NRF24L01
    NRF24L01.CRIS._exit();                  ///< 退出临界区
}

/**
  * @brief  Write multiple bytes to a specific register
  * @param  reg: The specific register
  * @param  pBuf: The buffer of the data to be write
  * @param  len: The length of the data to be write
  * @retval None
  * @note   
  */
void NRF24L01_Write_Buf(
    __IN__ const uint8_t reg, 
    __IN__ const uint8_t *pBuf, 
    __IN__ const uint8_t len
    )
{
    uint8_t u8_ctr;

    NRF24L01.CRIS._enter();                 ///< 进入临界区
    NRF24L01.SPI._set_speed();              ///< 设置SPI接口速率
    NRF24L01.CS._select();                  ///< 片选NRF24L01

    NRF24L01.SPI._write_byte(reg);          ///< 发送寄存器号
    for(u8_ctr=0; u8_ctr<len; u8_ctr++) {
        NRF24L01.SPI._write_byte(*pBuf++);  ///< 写入寄存器的值
    }

    NRF24L01.CS._deselect();                ///< 取消片选NRF24L01
    NRF24L01.CRIS._exit();                  ///< 退出临界区
}

/**
  * @brief  Check the NRF24L01 moudle is normal or not
  * @param  None
  * @retval NRF24L01_OK             : Module is normal
  *         NRF24L01_CHECK_NOTALLOW : Fail because of busy \ref idle tx mode \ref tx mode \ref rx mode
  *         NRF24L01_CHECK_FAIL     : Fail because of the module is abnormal
  * @note   
  */
NRF24L01_Error NRF24L01_Check(void)
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    uint8_t buf[5];
    uint8_t check_buf[5]= {0XA5,0XB6,0XC7,0XD8,0XE9};
    uint8_t i;

    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    
    if( (NRF24L01.mode==NRF_IDLETX) || (NRF24L01.mode==NRF_TX) || (NRF24L01.mode==NRF_RX) )
    {
        errorstatus = NRF24L01_CHECK_NOTALLOW;
        return errorstatus;
    }
    
    NRF24L01_Write_Buf( W_REGISTER+TX_ADDR,check_buf,5 );   ///< 写入5个字节的地址
    NRF24L01_Read_Buf ( R_REGISTER+TX_ADDR,buf,5 );         ///< 读出写入的地址

    for(i=0; i<5; i++) {
        if(buf[i]!=check_buf[i]) {
            NRF24L01.mode = NRF_NONRF;                      ///< If check failed,change the working mode to NRF_NONRF mode
            errorstatus = NRF24L01_CHECK_FAIL;
            return errorstatus;
        }
    }
    
    if( NRF24L01.mode == NRF_NONRF )
    {
        NRF24L01.mode = NRF_CHECKED;
    }
    
    return errorstatus;
}

/**
  * @brief  Switch to shutdown mode
  * @param  None
  * @retval NRF24L01_OK : Switch success
  *         Other       : Switch fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_Shutdown(void)
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    
    if( NRF24L01.mode == NRF_NONRF )
    {
        errorstatus = NRF24L01_Check();
        if( errorstatus != NRF24L01_OK )
            return errorstatus;
    }else if( NRF24L01.mode == NRF_IDLETX )
    {
        errorstatus = NRF24L01_Standby();
        if( errorstatus != NRF24L01_OK )
            return errorstatus;
    }
    
    /* 写PWR_UP=0 并切换到关断模式 */
    setCONFIG(0x08);
    NRF24L01.mode = NRF_SHUTDOWN;
    
    return errorstatus;
}

/**
  * @brief  Switch standby mode
  * @param  None
  * @retval NRF24L01_OK: Switch success
  *         Other       : Switch fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_Standby(void)
{
    uint8_t Rsp;
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    
    if( (NRF24L01.mode==NRF_NONRF) || (NRF24L01.mode==NRF_CHECKED) || (NRF24L01.mode==NRF_SHUTDOWN) )
    {
        errorstatus = NRF24L01_Shutdown();
        if( errorstatus != NRF24L01_OK )
            return errorstatus;
        
        /* 写PWR_UP=1，延时1.5~2ms后进行软件复位和通用配置，并切换到待机模式 */
        setCONFIG( 0x0A );
        
    }else if( (NRF24L01.mode==NRF_IDLETX) || (NRF24L01.mode==NRF_RX) )
    {
        /* CE=0 */
        NRF24L01.CE._dis();
    }else if( NRF24L01.mode==NRF_TX )
    {
        errorstatus = NRF24L01_WAIT_TX_DS;
        return errorstatus;
    }
    
    /* 进行软件复位 */
    writeFLUSH_TX();
    writeFLUSH_RX();
    Rsp = getSTATUS();
    setSTATUS(Rsp);
    
    /* 通用配置 */
    setSETUP_AW( 0x03 );            ///< 设置地址宽度为5bytes
    setRF_CH( 40 );                 ///< 选择射频通道为0x40
    setRF_SETUP( 0x0f );            ///< 数据传输速率为2Mbps
    
    NRF24L01.mode = NRF_STANDBY;
    
    return errorstatus;
}

/**
  * @brief  Switch idle tx mode
  * @param  ack_mode: Automatic confirmation or not @ref AckMode
  * @retval NRF24L01_OK : Switch success
  *         Other       : Switch fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_IdleTX(AckMode ack_mode)
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    assert_param( IS_ACKMODE(ack_mode) );
    
    if( (NRF24L01.mode==NRF_NONRF) || (NRF24L01.mode==NRF_CHECKED) || (NRF24L01.mode==NRF_SHUTDOWN) || (NRF24L01.mode==NRF_RX) )
    {
        errorstatus = NRF24L01_Standby();
        if( errorstatus!=NRF24L01_OK )
            return errorstatus;
    }else if( NRF24L01.mode==NRF_TX )
    {
        errorstatus = NRF24L01_WAIT_TX_DS;
        return errorstatus;
    }
    
    /* PRIM_RX=0,TX FIFO无数据,CE=1 */
    NRF24L01.CE._dis();
    setTX_ADDR( NRF24L01.tx_addr );
    
    if( ack_mode==ACK )
    {
        setRX_ADDR_P0(NRF24L01.tx_addr);    ///< 设置本机地址为RX节点接收数据通道0的地址

        setEN_RXADDR( 0x01 );               ///< 使能接收通道0的接收地址
        setEN_AA(0x01);                     ///< 使能数据管道0的自动确认
    #ifdef _Si24R1  ///< {
        setFEATURE( 0x04 );                 ///< 使能动态负载长度
        setDYNPD( 0x01 );                   ///< 使能接收管道0动态负载长度
    #endif  ///< }
    }else
    {
    #ifdef _Si24R1  ///< {
        setFEATURE( 0x01 );                 ///< 使能命令 W_TX_PAYLOAD_NOACK
    #endif  ///< }
    }
    
    setSETUP_RETR( 0x15 );                  ///< 自动重发配置
    setCONFIG( 0x0E );                      ///< 切换工作模式为发送模式PRIM_RX=0
    writeFLUSH_TX();                        ///< Clear TX FIFO
    NRF24L01.CE._en();
    
    NRF24L01.mode = NRF_IDLETX;
    
    return errorstatus;
}

/**
  * @brief  Filling TX FIFO and Switch to tx mode
  * @param  ack_mode: Automatic confirmation or not @ref AckMode
  * @param  pTXAddr: Pointing to TX address array
  * @param  pTXBuf: Pointing to TX buffer
  * @param  tx_len: The length of the tx buffer to send
  * @retval NRF24L01_OK : Switch success
  *         Other       : Switch fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_TX(AckMode ack_mode,uint8_t* pTXAddr,uint8_t* pTXBuf,uint8_t tx_len)
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    assert_param( IS_ACKMODE(ack_mode) );
    
    if( (NRF24L01.mode!=NRF_IDLETX) || \
        (memcmp(NRF24L01.tx_addr,pTXAddr,ADDR_WIDTH)!=0) || \
        (NRF24L01.ackmode!=ack_mode) )
    {
        memcpy( NRF24L01.tx_addr,pTXAddr,ADDR_WIDTH );
        NRF24L01.ackmode=ack_mode;
        
        errorstatus = NRF24L01_IdleTX(ack_mode);
        if( errorstatus!=NRF24L01_OK )
            return errorstatus;
    }
    
    NRF24L01.CE._dis();
    writeTX_PAYLOAD( pTXBuf,tx_len );       ///< 写TX FIFO
    NRF24L01.CE._en();
    
    return errorstatus;
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  Switch to rx mode
  * @param  ack_mode: Automatic confirmation or not @ref AckMode
  * @retval NRF24L01_OK : Switch success
  *         Other       : Switch fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_RX(AckMode ack_mode)
{
    uint8_t Rsp;
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_NRFMODE(NRF24L01.mode) );
    assert_param( IS_ACKMODE(ack_mode) );
    
    if( NRF24L01.mode!=NRF_STANDBY )
    {
        errorstatus = NRF24L01_Standby();
        if( errorstatus!=NRF24L01_OK )
            return errorstatus;
    }
    
    Rsp = getSTATUS();                      ///< 读取状态寄存器的值
    setSTATUS(Rsp);                         ///< 清除TX_DS或MAX_RT中断标志
    
    NRF24L01.CE._dis();
    setRX_ADDR_P0( NRF24L01.rx_addr );
    setEN_RXADDR( 0x01 );                   ///< 使能接收通道0的接收地址
    
    if( ack_mode==ACK )
    {
        setEN_AA(0x01);                     ///< 使能数据管道0的自动确认
    #ifdef _Si24R1  ///< {
        setFEATURE( 0x04 );                 ///< 使能动态负载长度
        setDYNPD( 0x01 );                   ///< 使能接收管道0动态负载长度
    #endif  ///< }
    }
    
    setRX_PW_P0( PLOAD_WIDTH );             ///< 选择通道0的有效数据宽度
    setCONFIG( 0x0F );                      ///< 切换工作模式为发送模式PRIM_RX=0
    writeFLUSH_RX();                        ///< Clear RX FIFO
    NRF24L01.CE._en();
    
    return errorstatus;
}

/**
  * @brief  Set RX address
  * @param  pRXAddr: Pointing to RX address array
  * @param  addr_width: The lenght of RX address array
  * @retval NRF24L01_OK : Set success
  *         Other       : Set fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_SET_RxAddr(
    __IN__ const uint8_t *pRXAddr,
    __IN__ const uint8_t addr_width
    )
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    if( ADDR_WIDTH!=addr_width ) {
        errorstatus=NRF24L01_ADDR_WIDTH_ERROR;
    } else {
        memcpy( NRF24L01.rx_addr,pRXAddr,ADDR_WIDTH );
        setRX_ADDR_P0( NRF24L01.rx_addr );
    }
    
    return errorstatus;
}

/**
  * @brief  Init TX related content
  * @param  pTXUnitArray: Pointing to TX Unit Array
  * @param  array_size: The size of TX Unit Array
  * @retval NRF24L01_OK : Init success
  *         Other       : Init fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_TX_Init(
    __OUT_ _NRF_TX_QUEUE_UNIT* pTXUnitArray,
    __IN__ const uint16_t array_size
    )
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    if( (pTXUnitArray==0) || (array_size==0) )
    {
        errorstatus=NRF24L01_ERROR;
    }
    
    if( QUEUE_OK != Queue_Create( &(NRF24L01.tx_queue),pTXUnitArray,array_size,sizeof(_NRF_TX_QUEUE_UNIT) ) )
    {
        errorstatus=NRF24L01_ERROR;
    }
    
    return errorstatus;
}

/**
  * @brief  Add ACK mode,TX Address,and TX Buffer
  * @param  ack_mode: Automatic confirmation or not @ref AckMode
  * @param  pTXAddr: Pointing to TX Address array
  * @param  pTXBuf: Pointing to TX Buffer array
  * @param  tx_len: The length of the TX Buffer
  * @retval NRF24L01_OK : Switch success
  *         Other       : Init fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_TX_Buffer( 
    __IN__ const AckMode ack_mode,
    __IN__ const uint8_t* pTXAddr,
    __OUT_ uint8_t* pTXBuf,
    __OUT_ uint8_t tx_len
    )
{
    _NRF_TX_QUEUE_UNIT tx_unit;
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    /* Check the parameters */
    assert_param( IS_ACKMODE(ack_mode) );
    
    if( (pTXAddr==NULL) || (pTXBuf==NULL) || (tx_len==0) )
    {
        errorstatus = NRF24L01_ERROR;
    }else
    {
        while( tx_len>0 )
        {
            if( tx_len>UNIT_BUF_MAX_SIZE )
            {
                memset( &tx_unit,0,sizeof(_NRF_TX_QUEUE_UNIT) );
                
                tx_unit.ack_mode = ack_mode;
                memcpy( tx_unit.addr,pTXAddr,ADDR_WIDTH );
                memcpy( tx_unit.buf,pTXBuf,UNIT_BUF_MAX_SIZE );
                tx_unit.nLast = UNIT_BUF_MAX_SIZE;
                if( QUEUE_OK != Queue_In( &NRF24L01.tx_queue,&tx_unit,sizeof(_NRF_TX_QUEUE_UNIT),ISFULL_DEL_OLD,NULL ) )
                {
                    errorstatus=NRF24L01_ERROR;
                    break;
                }else
                {
                    tx_len -= UNIT_BUF_MAX_SIZE;
                    pTXBuf += UNIT_BUF_MAX_SIZE;
                    NRF24L01.sta_state = STA_ENABLE;
                    NRF24L01.IRQ._trigger();
                }
            }else
            {
                memset( &tx_unit,0,sizeof(_NRF_TX_QUEUE_UNIT) );
                
                tx_unit.ack_mode = ack_mode;
                memcpy( tx_unit.addr,pTXAddr,ADDR_WIDTH );
                memcpy( tx_unit.buf,pTXBuf,tx_len );
                tx_unit.nLast = tx_len;
                if( QUEUE_OK != Queue_In( &NRF24L01.tx_queue,&tx_unit,sizeof(_NRF_TX_QUEUE_UNIT),ISFULL_DEL_OLD,NULL ) )
                {
                    errorstatus=NRF24L01_ERROR;
                    break;
                }else
                {
                    tx_len = 0;
                    NRF24L01.sta_state = STA_ENABLE;
                    NRF24L01.IRQ._trigger();
                }
            }
        }
    }
    
    return errorstatus;
}

/**
  * @brief  Send a TX unit
  * @param  None
  * @retval NRF24L01_OK     : Send success
  *         NRF24L01_ERROR  : Send fail
  * @note   
  */
static __inline NRF24L01_Error NRF24L01_TX_Unit()
{
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    if( (NRF24L01.tx_unit.nLast>0) && (NRF24L01.tx_unit.nLast<=PLOAD_WIDTH) )
    {
        if( NRF24L01_OK == NRF24L01_TX(NRF24L01.tx_unit.ack_mode,NRF24L01.tx_unit.addr,NRF24L01.tx_unit.buf+NRF24L01.tx_unit.iTx,NRF24L01.tx_unit.nLast) )
        {
            NRF24L01.tx_unit.nLast=0;
        }
    }else if( NRF24L01.tx_unit.nLast>PLOAD_WIDTH )
    {
        if( NRF24L01_OK == NRF24L01_TX(NRF24L01.tx_unit.ack_mode,NRF24L01.tx_unit.addr,NRF24L01.tx_unit.buf+NRF24L01.tx_unit.iTx,PLOAD_WIDTH) )
        {
            NRF24L01.tx_unit.nLast  -= PLOAD_WIDTH;
            NRF24L01.tx_unit.iTx    += PLOAD_WIDTH;
        }
    }else
    {
        errorstatus = NRF24L01_ERROR;
    }
    
    return errorstatus;
}

/**
  * @brief  Interrupt processing function
  * @param  None
  * @retval NRF24L01_OK : Interrupt processing completed
  *         Other       : Interrupt processing fail,specific reasons reference @ref NRF24L01_Error
  * @note   
  */
NRF24L01_Error NRF24L01_IRQ_oper(void)
{
    uint8_t Rsp;
    NRF24L01_Error errorstatus=NRF24L01_OK;
    
    Rsp = getSTATUS();                              ///< 读取状态寄存器的值
    setSTATUS(Rsp);                                 ///< 清除TX_DS或MAX_RT中断标志
    /* 接收端接收完成中断位 */
    if( Rsp&STA_RX_DR )
    {
        memset( NRF24L01.rx_buf,0,sizeof(NRF24L01.rx_buf) );
        
        NRF24L01.rx_len = readRX_PL_WID();
        readRX_PAYLOAD( NRF24L01.rx_buf,NRF24L01.rx_len );
        
        NRF24L01._rx_handle( NRF24L01.rx_buf,NRF24L01.rx_len );
        
        writeFLUSH_RX();
    }
    
    /* 发射端发射完成中断位 */
    if( Rsp&STA_TX_DS )
    {
        NRF24L01.mode = NRF_IDLETX;         ///< 切换到发射空闲模式
        
        /* 判断发射单元待发射数据的剩余长度 */
        if( NRF24L01.tx_unit.nLast<=0 )
        {
            if( QUEUE_ERR_EMPTY == Queue_Out(&NRF24L01.tx_queue,&NRF24L01.tx_unit,sizeof(_NRF_TX_QUEUE_UNIT)) )
            {
                errorstatus = NRF24L01_RX(ACK);
            }else
            {
                errorstatus = NRF24L01_TX_Unit();
                NRF24L01.mode = NRF_TX;
            }
        }else
        {
            errorstatus = NRF24L01_TX_Unit();
            NRF24L01.mode = NRF_TX;
        }
    }
    
    /* 达到最大重发次数中断位 */
    if( Rsp&STA_MAX_RT )
    {
        NRF24L01.mode = NRF_IDLETX;         ///< 切换到发射空闲模式
        
        memset( &NRF24L01.tx_unit,0,sizeof(_NRF_TX_QUEUE_UNIT) );
        if( QUEUE_ERR_EMPTY == Queue_Out(&NRF24L01.tx_queue,&NRF24L01.tx_unit,sizeof(_NRF_TX_QUEUE_UNIT)) )
        {
            errorstatus = NRF24L01_RX(ACK);
        }else
        {
            errorstatus = NRF24L01_TX_Unit();
            NRF24L01.mode = NRF_TX;
        }
    }
    
    if( NRF24L01.sta_state==STA_ENABLE )
    {
        NRF24L01.sta_state = STA_DISABLE;
        
        if( NRF24L01.mode!=NRF_TX )
        {
            if( QUEUE_ERR_EMPTY == Queue_Out(&NRF24L01.tx_queue,&NRF24L01.tx_unit,sizeof(_NRF_TX_QUEUE_UNIT)) )
            {
                errorstatus = NRF24L01_RX(ACK);
            }else
            {
                errorstatus = NRF24L01_TX_Unit();
                NRF24L01.mode = NRF_TX;
            }
        }
    }
    
    return errorstatus;
}

/**
  * @brief  Registered call back function for Chip Enable Activates RX or TX mode
  * @param  ce_en: callback function for chip enable activates RX or TX mode
  * @param  ce_dis: callback function for chip disable activates RX or TX mode
  * @retval None
  * @todo Describe @ref nrf_ce_enable and @ref nrf_ce_disable function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_ce_cbfunc(void(*ce_en)(void), void(*ce_dis)(void))
{
    if( !ce_en || !ce_dis ) {
        NRF24L01.CE._en     = nrf_ce_enable;
        NRF24L01.CE._dis    = nrf_ce_disable;
    } else {
        NRF24L01.CE._en     = ce_en;
        NRF24L01.CE._dis    = ce_dis;
    }
}

/**
  * @brief  Registered call back function for NRF24L01 critical section operation
  * @param  cris_en: callback function for NRF24L01 enter critical section operation
  * @param  cris_en: callback function for NRF24L01 exit critical section operation
  * @retval None
  * @todo Describe @ref nrf_cris_enter and @ref nrf_cris_exit function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_cris_cbfunc(void(*cris_en)(void), void(*cris_ex)(void))
{
    if( !cris_en || !cris_ex ) {
        NRF24L01.CRIS._enter  = nrf_cris_enter;
        NRF24L01.CRIS._exit   = nrf_cris_exit;
    } else {
        NRF24L01.CRIS._enter  = cris_en;
        NRF24L01.CRIS._exit   = cris_ex;
    }
}

/**
  * @brief  Registered call back function for NRF24L01 select & deselect
  * @param  cs_sel: callback function for NRF24L01 select
  * @param  cs_desel: callback function for NRF24L01 deselect
  * @retval None
  * @todo Describe @ref nrf_cs_select and @ref nrf_cs_deselect function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_cs_cbfunc(void(*cs_sel)(void), void(*cs_desel)(void))
{
    if( !cs_sel || !cs_desel ) {
        NRF24L01.CS._select     = nrf_cs_select;
        NRF24L01.CS._deselect   = nrf_cs_deselect;
    } else {
        NRF24L01.CS._select     = cs_sel;
        NRF24L01.CS._deselect   = cs_desel;
    }
}

/**
  * @brief  Registered call back function for SPI interface
  * @param  spi_ss: callback function to set SPI speed
  * @param  spi_rb: callback function to read byte usig SPI
  * @param  spi_wb: callback function to write byte usig SPI
  * @retval None
  * @todo Describe @ref nrf_spi_setspeed @ref nrf_spi_readbyte and @ref nrf_spi_writebyte function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_spi_cbfunc(void(*spi_ss)(void), uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb))
{
    if( !spi_ss || !spi_rb || !spi_wb ) {
        NRF24L01.SPI._set_speed     = nrf_spi_setspeed;
        NRF24L01.SPI._read_byte     = nrf_spi_readbyte;
        NRF24L01.SPI._write_byte    = nrf_spi_writebyte;
    } else {
        NRF24L01.SPI._set_speed     = spi_ss;
        NRF24L01.SPI._read_byte     = spi_rb;
        NRF24L01.SPI._write_byte    = spi_wb;
    }
}

/**
  * @brief  Registered call back function for interrupt trigger
  * @param  irq_trigger: callback function to trigger interrupt
  * @retval None
  * @todo Describe @ref nrf_irq_trigger function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_irq_cbfunc(void(*irq_trigger)(void))
{
    if( !irq_trigger ){
        NRF24L01.IRQ._trigger = nrf_irq_trigger;
    }else {
        NRF24L01.IRQ._trigger = irq_trigger;
    }
}

/**
  * @brief  Registered call back function for rx handle
  * @param  rx_handle: callback function for NRF24L01 rx handle
  * @retval None
  * @todo Describe @ref rx_handle function
  * or register your functions.
  * @note   If you do not describe or register, null function is called.
  */
void reg_nrf_rx_handle_cbfunc( NrfRxHandle rx_handle )
{
    if( !rx_handle ){
        NRF24L01._rx_handle = nrf_rx_handle;
    } else {
        NRF24L01._rx_handle = rx_handle;
    }
}

