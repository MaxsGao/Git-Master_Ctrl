/*
*********************************************************************************************************
*
*
*                                       Communicate with function Node
*                                         �빦���ӽڵ�ͨ��
* File : CommNode.c
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : �˹�������2.4G����ͨ�ŵĻ�������ʵ���빦���ӽڵ��ͨ�ţ��������ļ���Ҫ���������ݷ��ͣ����ݽ��պ���
*   �ݴ����API�ӿ�
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
#include "CommNode.h"
#include "MsgManage.h"
#include "nrf24l01.h"


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
/**
  * @brief  ��ʼ���빦���ӽڵ�ͨ����ص�����
  * @param  None
  * @note   �������ڵ���������ӽڵ���ͨ��2.4G����ͨ��ģ��NRF24L01ʵ�ֵ�
  * @retval None
  */
int CommNodeInit(void)
{
    /* ����2.4G����ͨ��ģ�� */
    
    /* ��ʼ���빦�ܽڵ�ͨ����صı������ʶ�� */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNodeSendData(
    uint8_t     nodeID,
    uint8_t*    pBuff,
    uint16_t    len
)
{
    /* ����nodeID�������Ӧ��ͨ���ŵ� */
    /* ����2.4Gģ������ݷ��ͺ����������ݷ��͵�ָ���ýڵ� */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNodeDealData()
{
    return 0;
}

