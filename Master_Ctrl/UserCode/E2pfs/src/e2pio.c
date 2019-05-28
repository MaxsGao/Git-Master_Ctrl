/*
*********************************************************************************************************
*
*
*                                     EEPROM dedicated file system
*                                         EEPROMר���ļ�ϵͳ
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "e2pio.h"
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
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
*                                       VARIABLES           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
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
*                                       API FUNCTIONS       API�ӿں���
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
    /* Ӳ����ʼ�� */
    /* checkһ�¶�д�Ƿ����� */
    /* ���س�ʼ����� */
    if ( res ) {
        return ESTA_NOREADY;
    }
    else {
        /* �޸�EEPROMӲ����ʼ��״̬ */
        return ESTA_OK;
    }
}

/* ��ȡ������EEPROMӲ����ʼ��״̬ */
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
    /* Ӳ����ʼ�� */
    /* checkһ�¶�д�Ƿ����� */
    /* ���س�ʼ����� */
    if ( res ) {
        return ESTA_NOREADY;
    }
    else {
        /* �޸�EEPROMӲ����ʼ��״̬ */
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
/* �����ٽ��� */
void ef_enter(E2PFS* fs)
{
#if SYSTEM_SUPPORT_OS
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
}
/* �˳��ٽ��� */
void ef_exit(E2PFS* fs)
{
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}

