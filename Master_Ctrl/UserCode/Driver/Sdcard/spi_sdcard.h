/*
*********************************************************************************************************
*
*
*                                               SPI SD
*                                         ����SPI�����ϵ�SD��
* File : SPI_SD.h
* By   : Bear
* Date : 2018/11/15
* version : V0.1
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __SPI_SDCARD_H
#define   __SPI_SDCARD_H


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdint.h>


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
// SD�����Ͷ���  
#define SD_TYPE_ERR     0X00
#define SD_TYPE_MMC     0X01
#define SD_TYPE_V1      0X02
#define SD_TYPE_V2      0X04
#define SD_TYPE_V2HC    0X06
// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD8    8       //����8 ��SEND_IF_COND
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define CMD23   23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define CMD41   41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00
//SD�� ���ִ���ö�ٶ���
typedef enum
{	 
	//��������� 
	SD_CMD_CRC_FAIL                    = (1), /*!< Command response received (but CRC check failed) */
	SD_DATA_CRC_FAIL                   = (2), /*!< Data bock sent/received (CRC check Failed) */
	SPI_SD_CMD_RSP_TIMEOUT              = (3), /*!< Command response timeout */
	SD_DATA_TIMEOUT                    = (4), /*!< Data time out */
	SD_TX_UNDERRUN                     = (5), /*!< Transmit FIFO under-run */
	SD_RX_OVERRUN                      = (6), /*!< Receive FIFO over-run */
	SD_START_BIT_ERR                   = (7), /*!< Start bit not detected on all data signals in widE bus mode */
	SD_CMD_OUT_OF_RANGE                = (8), /*!< CMD's argument was out of range.*/
	SD_ADDR_MISALIGNED                 = (9), /*!< Misaligned address */
	SD_BLOCK_LEN_ERR                   = (10), /*!< Transferred block length is not allowed for the card or the number of transferred bytes does not match the block length */
	SD_ERASE_SEQ_ERR                   = (11), /*!< An error in the sequence of erase command occurs.*/
	SD_BAD_ERASE_PARAM                 = (12), /*!< An Invalid selection for erase groups */
	SD_WRITE_PROT_VIOLATION            = (13), /*!< Attempt to program a write protect block */
	SD_LOCK_UNLOCK_FAILED              = (14), /*!< Sequence or password error has been detected in unlock command or if there was an attempt to access a locked card */
	SD_COM_CRC_FAILED                  = (15), /*!< CRC check of the previous command failed */
	SD_ILLEGAL_CMD                     = (16), /*!< Command is not legal for the card state */
	SD_CARD_ECC_FAILED                 = (17), /*!< Card internal ECC was applied but failed to correct the data */
	SD_CC_ERROR                        = (18), /*!< Internal card controller error */
	SD_GENERAL_UNKNOWN_ERROR           = (19), /*!< General or Unknown error */
	SD_STREAM_READ_UNDERRUN            = (20), /*!< The card could not sustain data transfer in stream read operation. */
	SD_STREAM_WRITE_OVERRUN            = (21), /*!< The card could not sustain data programming in stream mode */
	SD_CID_CSD_OVERWRITE               = (22), /*!< CID/CSD overwrite error */
	SD_WP_ERASE_SKIP                   = (23), /*!< only partial address space was erased */
	SD_CARD_ECC_DISABLED               = (24), /*!< Command has been executed without using internal ECC */
	SD_ERASE_RESET                     = (25), /*!< Erase sequence was cleared before executing because an out of erase sequence command was received */
	SD_AKE_SEQ_ERROR                   = (26), /*!< Error in sequence of authentication. */
	SD_INVALID_VOLTRANGE               = (27),
	SD_ADDR_OUT_OF_RANGE               = (28),
	SD_SWITCH_ERROR                    = (29),
	SD_SDIO_DISABLED                   = (30),
	SD_SDIO_FUNCTION_BUSY              = (31),
	SD_SDIO_FUNCTION_FAILED            = (32),
	SD_SDIO_UNKNOWN_FUNCTION           = (33),
    SPI_SD_WAIT_READY_TIMEOUT          = (34), /*!< Waiting for the SD card to be ready to time out. */
    SPI_SD_NOTGET_START_TOKEN          = (35), /*!< The read data operation did not receive the start token. */
    SPI_SD_TX_RSP_ERROR                = (36), /*!< The read data operation did not receive the start token. */
	//��׼������
	SD_INTERNAL_ERROR, 
	SD_NOT_CONFIGURED,
	SD_REQUEST_PENDING, 
	SD_REQUEST_NOT_APPLICABLE, 
	SD_INVALID_PARAMETER,  
	SD_UNSUPPORTED_FEATURE,  
	SD_UNSUPPORTED_HW,
    SPI_SD_CMD_RSP_ERROR,
	SPI_SD_ERROR,  
	SPI_SD_OK = 0 
} SPI_SD_Error;	


/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
typedef struct __SPI_SD
{
    uint8_t     type;                       ///< Sdcard's type
    uint8_t     cid[16];                    ///< Sdcard's cid
    uint8_t     csd[16];                    ///< Sdcard's csd
    uint32_t    capacity;                   ///< Sdcard's capacity
    
    struct _CRIS
    {
        void    (*_enter)           (void);
        void    (*_exit)            (void);
    }CRIS;
    
    struct _CS
    {
        void    (*_select)          (void);
        void    (*_deselect)        (void);
    }CS;
    
    struct _SPI
    {
        void    (*_set_low_speed)   (void);
        void    (*_set_high_speed)  (void);
        uint8_t (*_read_byte)       (void);
        void    (*_write_byte)      (uint8_t wb);
    }SPI;
}_SPI_SD;
extern _SPI_SD SPI_SD;



/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
void reg_spi_sdcard_cris_cbfunc (void(*cris_en)(void), void(*cris_ex)(void));
void reg_spi_sdcard_cs_cbfunc   (void(*cs_sel)(void), void(*cs_desel)(void));
void reg_spi_sdcard_spi_cbfunc  (void(*spi_sls)(void), void(*spi_shs)(void), uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb));

SPI_SD_Error    SPI_SD_Init             (void);                                         //��ʼ��SD��
SPI_SD_Error    SPI_SD_WaitReady        (void);                                         //�ȴ�SD��׼��
SPI_SD_Error    SPI_SD_ReadDisk         (uint8_t*buf,uint32_t sector,uint8_t cnt);		//����
SPI_SD_Error    SPI_SD_WriteDisk        (uint8_t*buf,uint32_t sector,uint8_t cnt);		//д��
SPI_SD_Error    SPI_SD_GetSectorCount   (void);   					                    //��������


#endif
