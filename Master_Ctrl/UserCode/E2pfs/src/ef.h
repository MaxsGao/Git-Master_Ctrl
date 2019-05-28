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
#ifndef   __E2PFS_H
#define   __E2PFS_H

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
#include "e2pio.h"


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

/* Type of path name strings on FatFs API */

#if _LFN_UNICODE			/* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 in non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#define _T(x) L ## x
#define _TEXT(x) L ## x
#endif

#else						/* ANSI/OEM string */
#ifndef _INC_TCHAR
typedef char TCHAR;
#define _T(x) x
#define _TEXT(x) x
#endif

#endif


/* File function return code (FRESULT) */

typedef enum {
	E2PFR_OK = 0,				/* (0) Succeeded */
	E2PFR_E2P_ERR,			    /* (1) A hard error occurred in the low level disk I/O layer */
	E2PFR_INT_ERR,				/* (2) Assertion failed */
	E2PFR_NOT_READY,			/* (3) The physical drive cannot work */
	E2PFR_NO_FILE,				/* (4) Could not find the file */
	E2PFR_NO_PATH,				/* (5) Could not find the path */
	E2PFR_INVALID_NAME,		    /* (6) The path name format is invalid */
	E2PFR_DENIED,				/* (7) Access denied due to prohibited access or directory full */
	E2PFR_EXIST,				/* (8) Access denied due to prohibited access */
	E2PFR_INVALID_OBJECT,		/* (9) The file/directory object is invalid */
	E2PFR_WRITE_PROTECTED,		/* (10) The physical drive is write protected */
	E2PFR_INVALID_DRIVE,		/* (11) The logical drive number is invalid */
	E2PFR_NOT_ENABLED,			/* (12) The volume has no work area */
	E2PFR_NO_FILESYSTEM,		/* (13) There is no valid FAT volume */
	E2PFR_MKFS_ABORTED,		    /* (14) The f_mkfs() aborted due to any parameter error */
	E2PFR_TIMEOUT,				/* (15) Could not get a grant to access the volume within defined period */
	E2PFR_LOCKED,				/* (16) The operation is rejected according to the file sharing policy */
	E2PFR_NOT_ENOUGH_CORE,		/* (17) LFN working buffer could not be allocated */
	E2PFR_TOO_MANY_OPEN_FILES,	/* (18) Number of open files > _FS_SHARE */
	E2PFR_INVALID_PARAMETER,	/* (19) Given parameter is invalid */
	E2PFR_NOT_E2PFS,            /* (20) Given parameter is invalid */
	E2PFR_NO_FREE_AREA	        /* (21) Storage is full */
} E2PFRESULT;

/* Results of Disk Functions */
typedef enum {
    DRVSTA_NOTRDY   = 0,    /* 0: */
    DRVSTA_OK       = 1     /* 1: */
} DRVSTA;

//typedef enum {
//} WINSTA;

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
//typedef struct
//{
//    uint8_t BS_55AA[2];
//    uint8_t BS_FilSysType[3];
//    uint8_t BS_BytsPerPage;
//    uint8_t BS_NumPages[2];
//    
//    uint8_t DIR_BasePage[2];
//    uint8_t DIR_SizePerUnit;
//    uint8_t DIR_NumUnits[2];
//    uint8_t DIR_FreeUnits[2];
//    
//    uint8_t FIL_BasePage[2];
//    uint8_t FIL_SizePerUnit;
//    uint8_t FIL_NumUnits[2];
//    uint8_t FIL_FreeUnits[2];
//    
//    uint8_t DAT_BasePage[2];
//    uint8_t DAT_SizePerPage;
//    uint8_t DAT_NumPages[2];
//    uint8_t DAT_FreePage[2];
//}E2P_MBR;

typedef struct
{
    uint8_t FIL_Name[4];        /* �ļ��� */
    uint8_t FIL_EntrPage[2];    /* �ļ����ҳ */
    uint8_t FIL_NumPages[2];    /* �ļ���ҳ�� */
}E2P_DIR;

typedef struct
{
    uint8_t FIL_NextPage[2];
}E2P_FIL;

/* File system object structure (E2PFS) */
typedef struct {
	uint8_t	    drv;			/* ������������ */
    DRVSTA      drv_stat;       /* ������������״̬ */
	uint8_t	    fs_stat;        /* �ļ�ϵͳ״̬ */
    
	uint8_t     psize;			/* ÿҳ�Ĵ�С (1,2,4...128)�ֽ� */
	uint16_t    n_pages;        /* ҳ���� */
    
	uint16_t	dirpage;		/* Ŀ¼����ʼҳ */
    uint8_t     dusize;         /* Ŀ¼��ÿ��Ŀ�Ĵ�С */
    uint16_t    n_dunits;       /* Ŀ¼������Ŀ�� */
    uint16_t    free_dunits;    /* Ŀ¼��ʣ�������Ŀ���� */
    
	uint16_t	filpage;		/* �ļ�ҳ���������ʼҳ */
    uint8_t     fusize;         /* ÿ��Ŀ��С */
    uint16_t    n_funits;       /* ����Ŀ�� */
    uint16_t    free_funits;    /* �ļ�ҳ�������ʣ�������Ŀ���� */
    
	uint16_t	datpage;		/* ��������ʼҳ */
    uint16_t    n_dpages;       /* ��������ҳ�� */
    uint16_t    free_dpages;    /* ������ʣ�����ҳ�� */
    
    uint16_t    page_cnt;       /* ҳ������� */
    uint16_t    dunit_cnt;      /* �ļ�Ŀ¼����Ŀ������ */
    uint16_t    funit_cnt;      /* �ļ�ҳ���������Ŀ������ */
    uint16_t    dpage_cnt;      /* �ļ�������ҳ������� */
    
    uint8_t     wflag;
    uint16_t    winpage;
    uint8_t     win[_E2P_PAGE_SIZE_];
} E2PFS;

/* File object structure (E2PFIL) */
typedef struct {
    E2PFS*      fs;             ///< �ļ����ڵ��ļ�ϵͳ file system
    uint8_t     flag;           ///< �ļ���д״̬�ı��
    
    uint16_t    dir_page;       ///< �ļ�Ŀ¼��Ϣ����ҳ��
    uint16_t    dir_posi;       ///< �ļ�Ŀ¼��Ϣ����ҳ���λ��
    uint8_t     fname[4];       ///< �ļ��� file name
    uint16_t    fsize;          ///< �ļ���С file size
    uint16_t    fsitem;         ///< �ļ����ݵ���ʼҳ file start page
    
    uint16_t    fcitem;         ///< �ļ����ݵĵ�ǰҳ file current page
    uint16_t    fnitem;         ///< �ļ����ݵ���һҳ file next page
    
    unsigned long   fitem_cnt;  ///< �ļ���д
    unsigned long   fptr;       ///< �ļ���дָ��
} E2PFIL;



/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
E2PFRESULT e2pf_mount( E2PFS* fs, const uint8_t drv );  ///< ����e2p�ļ�ϵͳ
E2PFRESULT e2pf_mkfs ( E2PFS* fs, const uint8_t drv );  ///< �����ļ�ϵͳ
E2PFRESULT e2pf_open ( E2PFIL* fp,const uint8_t* path,uint8_t mode );
E2PFRESULT e2pf_close( E2PFIL* fp );
E2PFRESULT e2pf_write( E2PFIL* fp,const void* buff, uint32_t btw, uint32_t* bw );
E2PFRESULT e2pf_read ( E2PFIL* fp,const void* buff, uint32_t btr, uint32_t* br );
/** @defgroup SPI_Private_Defines
  * @{
  */
#define E2PFA_SEEK_SET                  0          ///< I2S I2SE mask
#define E2PFA_SEEK_CUR                  1          ///< I2S I2SE mask
#define E2PFA_SEEK_END                  2          ///< I2S I2SE mask
#define IS_E2PFA_SEEK_FLG(SEEK) (((SEEK) == E2PFA_SEEK_SET) || \
                                 ((SEEK) == E2PFA_SEEK_CUR) || \
                                 ((SEEK) == E2PFA_SEEK_END))
/**
  * @}
  */
E2PFRESULT e2pf_seek ( E2PFIL* fp,long  offset, int fromwhere );


void ef_enter(E2PFS* fs);


/* File access control and file status flags (FIL.flag) */
#define	E2PFA_READ          0x01
#define	E2PFA_OPEN_EXISTING 0x00

#define	E2PFA_WRITE         0x02
#define	E2PFA_CREATE_NEW    0x04
#define	E2PFA_CREATE_ALWAYS 0x08
#define	E2PFA_OPEN_ALWAYS   0x10
#define E2PFA__WRITTEN      0x20
#define E2PFA__DIRTY        0x40



///* Multi-byte word access macros  */
//#if _WORD_ACCESS == 1	/* Enable word access to the FAT structure */
//#define	LD_WORD(ptr)		(WORD)(*(WORD*)(BYTE*)(ptr))
//#define	LD_DWORD(ptr)		(DWORD)(*(DWORD*)(BYTE*)(ptr))
//#define	ST_WORD(ptr,val)	*(WORD*)(BYTE*)(ptr)=(WORD)(val)
//#define	ST_DWORD(ptr,val)	*(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
//#else					/* Use byte-by-byte access to the FAT structure */
#define	LD_WORD(ptr)		(WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#define	LD_DWORD(ptr)		(DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#define	ST_WORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#define	ST_DWORD(ptr,val)	*(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
//#endif








#ifdef __cplusplus
}
#endif

#endif
