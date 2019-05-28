/*
*********************************************************************************************************
*
*
*                                     EEPROM dedicated file system
*                                         EEPROM专用文件系统
* File : ee_fat.h
* By   : MaxsGao
* Date : 2019/03/22
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
#include "e2pio.h"
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/


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


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  * @param  SPI_I2S_FLAG: specifies the SPI flag to clear. 
  *   This function clears only CRCERR flag.
  * @note
  *   - OVR (OverRun error) flag is cleared by software sequence: a read 
  *     operation to SPI_DR register (SPI_I2S_ReceiveData()) followed by a read 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - UDR (UnderRun error) flag is cleared by a read operation to 
  *     SPI_SR register (SPI_I2S_GetFlagStatus()).
  *   - MODF (Mode Fault) flag is cleared by software sequence: a read/write 
  *     operation to SPI_SR register (SPI_I2S_GetFlagStatus()) followed by a 
  *     write operation to SPI_CR1 register (SPI_Cmd() to enable the SPI).
  * @retval None
  */


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
ESTATUS e2p_initialize( uint8_t pDrv )
{
    uint8_t res;
    
    switch( pDrv )
    {
        case E2P_ROM0:
            res = AT24CXX_Init();
            break;
        case E2P_ROM1:
            break;
        default:
            res = 1;
            break;
    }
    /* 硬件初始化 */
    /* check一下读写是否正常 */
    /* 返回初始化结果 */
    if ( res ) {
        return ESTA_NOREADY;
    }
    else {
        /* 修改EEPROM硬件初始化状态 */
        return ESTA_OK;
    }
}

/* 读取并返回EEPROM硬件初始化状态 */
ESTATUS e2p_status( uint8_t pDrv )
{
    uint8_t res;
    
    switch( pDrv )
    {
        case E2P_ROM0:
            res = AT24CXX_Check();
            break;
        case E2P_ROM1:
            break;
        default:
            res = 1;
            break;
    }
    /* 硬件初始化 */
    /* check一下读写是否正常 */
    /* 返回初始化结果 */
    if ( res ) {
        return ESTA_NOREADY;
    }
    else {
        /* 修改EEPROM硬件初始化状态 */
        return ESTA_OK;
    }
}
ERESULT e2p_read( uint8_t pDrv,uint8_t* buff,uint16_t page,uint32_t count )
{
    uint8_t res;
    
    if (!count) return ERES_PARERR;
    
    switch( pDrv )
    {
        case E2P_ROM0:
            res = AT24CXX_ReadPages( page,buff,count );
            break;
        case E2P_ROM1:
            break;
        default:
            res = 1;
            break;
    }
    
    if ( res ) {
        return ERES_ERROR;
    }
    else 
        return ERES_OK;
}
ERESULT e2p_write( uint8_t pDrv,uint8_t* buff,uint16_t page,uint32_t count )
{
    uint8_t res;
    
    switch( pDrv )
    {
        case E2P_ROM0:
            res = AT24CXX_WritePages( page,buff,count );
            break;
        case E2P_ROM1:
            break;
        default:
            res = 1;
            break;
    }
    
    if ( res ) {
        return ERES_ERROR;
    }
    else 
        return ERES_OK;
}
ERESULT e2p_ioctl( uint8_t pDrv,uint8_t cmd,void* buff )
{
    ERESULT res;
    if ( pDrv==E2P_ROM0 ) {
        switch(cmd)
        {
            case CTRL_SYNC:
                res = ERES_OK;
                break;
            case GET_TOAL_SIZE:
                *(uint16_t*)buff = _E2P_TOAL_SIZE_;
                break;
            case GET_PAGE_NUM:
                *(uint16_t*)buff = _E2P_PAGE_NUM_;
                break;
            case GET_PAGE_SIZE:
                *(uint8_t*)buff = _E2P_PAGE_SIZE_;
                break;
            case CTRL_TRIM:
                res = ERES_OK;
                break;
            default:
                break;
        }
    }
    else if ( pDrv==E2P_ROM1 ){
        res = ERES_ERROR;
    }
    else {
        res = ERES_ERROR;
    }
    
    return res;
}

static OS_CPU_SR cpu_sr=0;
/* 进入临界区 */
void ef_enter(E2PFS* fs)
{
#if SYSTEM_SUPPORT_OS
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
}
/* 退出临界区 */
void ef_exit(E2PFS* fs)
{
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
#else
    INTX_ENABLE();
#endif
}

