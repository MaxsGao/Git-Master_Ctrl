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
#ifndef   _E2PIO_DEFINED
#define   _E2PIO_DEFINED

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdio.h>
#include <stdint.h>
#include "integer.h"
     
#include "efconf.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
/**
 * @brief Peer IP register address
 */
#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif

#define CTRL_SYNC           0
#define GET_TOAL_SIZE       1
#define GET_PAGE_NUM        2
#define GET_PAGE_SIZE       3
#define CTRL_TRIM           4

#define E2P_ROM0    0   //EEPROM0,���Ϊ0
#define E2P_ROM1    1   //EEPROM1,���Ϊ1


/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
/* Status of Disk Functions */
typedef BYTE	ESTATUS;
#define ESTA_OK         0x00
#define ESTA_NOREADY    0x01
#define ESTA_NOE2P      0x02
#define ESTA_PROTECT    0x04


/* Results of E2P Functions */
typedef enum {
	ERES_OK = 0,		/* 0: Successful */
	ERES_ERROR,		/* 1: R/W Error */
	ERES_WRPRT,		/* 2: Write Protected */
	ERES_NOTRDY,		/* 3: Not Ready */
	ERES_PARERR		/* 4: Invalid Parameter */
} ERESULT;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
/* Prototypes for e2p control functions */
ESTATUS e2p_initialize  ( uint8_t pDrv );
ESTATUS e2p_status      ( uint8_t pDrv );
ERESULT e2p_read        ( uint8_t pDrv,uint8_t* buff,uint16_t addr,uint32_t count );
ERESULT e2p_write       ( uint8_t pDrv,uint8_t* buff,uint16_t addr,uint32_t count );
ERESULT e2p_ioctl       ( uint8_t pDrv,uint8_t cmd,void* buff );


#ifdef __cplusplus
}
#endif

#endif
