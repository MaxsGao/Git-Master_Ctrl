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
#include "ef.h"
#include "string.h"


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
#if _E2P_VOLUMES >= 1 || _E2P_VOLUMES <= 10
static
E2PFS *E2PFs[_E2P_VOLUMES];		/* Pointer to the file system objects (logical drives) */
#else
#error Number of e2p volumes must be 1 to 10.
#endif

#define BS_55AA         0
#define BS_FilSysType   2
#define BS_BytsPerPage  5
#define BS_NumPages     6

#define DIR_BasePage    8
#define DIR_SizePerUnit 10
#define DIR_NumUnits    11
#define DIR_FreeUnits   13

#define FIL_BasePage    15
#define FIL_SizePerUnit 17
#define FIL_NumUnits    18
#define FIL_FreeUnits   20

#define DAT_BasePage    22
#define DAT_SizePerPage 24
#define DAT_NumPages    25
#define DAT_FreePages   27



#define DIR_Fname   0
#define DIR_Fsize   4
#define DIR_Fspage  6


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
  * @brief  ���ش��̷��ʴ��ڵ�����
  * @param  
  * @note   
  * @retval 
  */
static E2PFRESULT load_window(
    E2PFS* fs,
    uint16_t page
)
{
    E2PFRESULT res = E2PFR_OK;
    
    if ( page!=fs->winpage ) {
        if ( e2p_read(fs->drv,fs->win,page,1) != ERES_OK ) {
            page = 0xFFFF;
            res = E2PFR_E2P_ERR;
        }
        fs->winpage = page;
    }
    
    return res;
}

static E2PFRESULT MBR_Select(E2PFS* fs)
{
    E2PFRESULT res = E2PFR_OK;
    
    if ( fs->winpage!=_E2P_MBR_BASE_PAGE_ ) {
        res = load_window(fs,_E2P_MBR_BASE_PAGE_);
        if ( res!=ERES_OK )
            return res;
    }
    
    
    return E2PFR_OK;
}
static E2PFRESULT MBR_Update(
    __IN__ E2PFS*   fs,
    __IN__ uint16_t free_dunits,
    __IN__ uint16_t free_funits,
    __IN__ uint16_t free_dpages
)
{
    uint16_t page = 0;
    
    /* �����ļ�Ŀ¼����Ӧҳ������� */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* �޸����� */
    ST_WORD( fs->win+DIR_FreeUnits,free_dunits );
    ST_WORD( fs->win+FIL_FreeUnits,free_funits );
    ST_WORD( fs->win+DAT_FreePages,free_dpages );
    
    /* ���޸���ɵ��ļ�Ŀ¼��Ϣд�뵽EEPROM�д洢���� */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    return E2PFR_OK;
}

/**
  * @brief  �ļ�Ŀ¼��Ϣ�Ĳ��Ҳ���,�����ҵ��Ľ��������ļ��������ṹ��
  * @param  �����ļ�ϵͳ
  * @param  �ļ�·��/�ļ�����
  * @note   
  * @retval ���ҽ��
  */
static E2PFRESULT DIR_Select(
    __IN__ E2PFS*         fs,
    __IN__ const uint8_t* fname,
    __OUT_ uint16_t*      page,
    __OUT_ uint16_t*      posi,
    __OUT_ uint16_t*      fsize,
    __OUT_ uint16_t*      fsitem
)
{
    uint8_t  i,psize,dusize;
    uint16_t page_cnt;
    
    /* ���������Ч�Լ��� */
    if ( fs==NULL || fname==NULL )
        return E2PFR_INVALID_PARAMETER;
    
    psize   = fs->psize;
    dusize  = fs->dusize;
    
    /* ��ҳ���� */
    for ( page_cnt=fs->dirpage;page_cnt<fs->filpage;page_cnt++ ) {
        
        /* �����ļ�Ŀ¼����Ӧҳ������� */
        if ( load_window(fs,page_cnt) != E2PFR_OK )
            return E2PFR_E2P_ERR;
        
        /* �������� */
        for ( i=0;i<(psize/dusize); i++ ) {
            if ( memcmp( fs->win+(i*dusize),fname,4 )==0 ) {
                
                *page   = page_cnt;
                *posi   = i*dusize;
                *fsize  = LD_WORD(fs->win+(i*dusize+DIR_Fsize));
                *fsitem = LD_WORD(fs->win+(i*dusize+DIR_Fspage));
                
                return E2PFR_OK;
            }
        }
    }
    
    return E2PFR_NO_PATH;
}

/**
  * @brief  �ļ�Ŀ¼��Ϣ���޸Ĳ���
  * @param  
  * @note   
  * @retval �޸Ľ��
  */
static E2PFRESULT DIR_Update(
    __IN__ E2PFS*   fs,
    __IN__ uint16_t page,
    __IN__ uint16_t posi,
    __IN__ uint8_t* fname,
    __IN__ uint16_t fsize,
    __IN__ uint16_t fsitem
)
{
    /* �����ļ�Ŀ¼����Ӧҳ������� */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* �޸�Ҫ��ӵ��ļ���Ŀ¼��Ϣ */
    memcpy ( fs->win+posi+DIR_Fname, fname, 4 );
    ST_WORD( fs->win+posi+DIR_Fsize, fsize    );
    ST_WORD( fs->win+posi+DIR_Fspage,fsitem   );
    
    /* ���޸���ɵ��ļ�Ŀ¼��Ϣд�뵽EEPROM�д洢���� */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    return E2PFR_OK;
}

/**
  * @brief  �ļ�Ŀ¼��Ϣ��ɾ������
  * @param  
  * @note   
  * @retval �޸Ľ��
  */
static E2PFRESULT DIR_Delete(
    __IN__ E2PFS*   fs,
    __IN__ uint16_t page,
    __IN__ uint16_t posi
)
{
    /* �����ļ�Ŀ¼����Ӧҳ������� */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* �޸��ļ�Ŀ¼��ϢΪ */
    memset( fs->win+posi+DIR_Fname, 0xFF,fs->dusize );
    
    /* ���޸���ɵ��ļ�Ŀ¼��Ϣд�뵽EEPROM�д洢���� */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    
    fs->free_dunits++;
    return E2PFR_OK;
}


static E2PFRESULT FIL_Select(
    __IN__ E2PFS    *fs,
    __IN__ uint16_t n_item,
    __OUT_ uint16_t *record
)
{
    uint8_t psize,isize;
    
    psize   = fs->psize;
    isize   = fs->fusize;
    
    /* ���ݼ�¼����ţ�ȷ������ҳ����λ�� */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* �����ļ�ҳ���������Ӧҳ������� */
    if ( load_window( fs,page ) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* ��ȡ�ü�¼���м�¼������ */
    *record = LD_WORD( fs->win+(posi) );
    
    return E2PFR_OK;
}
static E2PFRESULT FIL_Update(
    __IN__ E2PFS    *fs,
    __IN__ uint16_t n_item,
    __IN__ uint16_t record
)
{
    uint8_t psize,isize;
    
    psize   = fs->psize;
    isize   = fs->fusize;
    
    /* ���ݼ�¼����ţ�ȷ������ҳ����λ�� */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* �����ļ�Ŀ¼����Ӧҳ������� */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* �޸�Ҫ��ӵ��ļ���Ŀ¼��Ϣ */
    ST_WORD( fs->win+posi, record );
    
    /* ���޸���ɵ��ļ�Ŀ¼��Ϣд�뵽EEPROM�д洢���� */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    return E2PFR_OK;
}
static E2PFRESULT FIL_Delete(
    __IN__ E2PFS    *fs,
    __IN__ uint16_t n_item
)
{
    uint8_t psize,isize;
    
    psize   = fs->psize;
    isize   = fs->fusize;
    
    
    /* ��յ�ǰ��¼���Ӧ����ҳ������ */
    memset( fs->win,0,_E2P_PAGE_SIZE_ );
    if ( e2p_write( fs->drv,fs->win,fs->datpage+n_item,1 ) != ERES_OK)
        return E2PFR_E2P_ERR;
    
    /* ���ݼ�¼����ţ�ȷ������ҳ����λ�� */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* �����ļ�Ŀ¼����Ӧҳ������� */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* ��ȡ�ü�¼���м�¼������ */
    uint16_t n_next_item = LD_WORD( fs->win+(posi) );
    
    /* �޸�Ҫ��ӵ��ļ���Ŀ¼��Ϣ */
    ST_WORD( fs->win+posi, 0xEEEE );
    
    /* ���޸���ɵ��ļ�Ŀ¼��Ϣд�뵽EEPROM�д洢���� */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    /* �ݹ�ֹͣ���� */
    if ( n_next_item != 0xFFFF ) {
        FIL_Delete( fs,n_next_item );
    }
    
    return E2PFR_OK;
}



/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
static E2PFRESULT check_fs(
    E2PFS* fs,
    uint16_t page
)
{
    /* ��ȡ�ļ������������� */
    fs->wflag = 0;
    fs->winpage = 0xFFFF;
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* ����ļ������� */
    if ( LD_WORD(fs->win+BS_55AA) != 0x55AA )
        return E2PFR_NO_FILESYSTEM;
    
    if ( (LD_DWORD(fs->win+BS_FilSysType) & 0xFFFFFF) == 0x455350  )
        return E2PFR_OK;
    
    return E2PFR_NOT_E2PFS;
}

E2PFRESULT mount_e2p_volume(	/* FR_OK(0): successful, !=0: any error occurred */
	E2PFS* fs,		/* Pointer to pointer to the found file system object */
	const uint8_t drv,	/* Pointer to pointer to the path name (drive number) */
	BYTE wmode			/* !=0: Check write protection for write access */
)
{
    E2PFRESULT  res;
    ESTATUS     stat;
    uint16_t    b_fat,b_dir,b_fil,b_dat;
    uint8_t     psize,dusize,fusize;
    uint16_t    n_pages,n_dunits,n_funits,n_dpages,free_dunits,free_funits,free_dpages;

    /* ����ļ��豸״̬�Ƿ��ʼ������ҹ������� */
    if ( fs->drv_stat!=DRVSTA_OK ) {
        stat = e2p_initialize(drv);         ///< ��ʼ���ļ��豸
        if ( stat!=ESTA_OK ) {
            fs->drv_stat = DRVSTA_NOTRDY;
            return E2PFR_NOT_READY;
        }
        fs->drv_stat = DRVSTA_OK;           ///< �޸��ļ��豸��״̬
    }
    
    b_fat = _E2P_MBR_BASE_PAGE_;            ///< ��ȡ�ļ���������ʼҳ��
    res = check_fs( fs,b_fat );             ///< ����ļ�ϵͳ�Ƿ���ȷ
    if ( res!=E2PFR_OK )
        return res;
    
    fs->drv = drv;      ///< ��ȡ�豸��������
    
    psize = LD_WORD( fs->win+BS_BytsPerPage );  ///< ��ȡ�ļ��豸��ҳ���С
    fs->psize = psize;
    
    n_pages = LD_DWORD( fs->win+BS_NumPages );  ///< ��ȡ�ļ��豸��ҳ��������
    fs->n_pages = n_pages;
    
    /* �ļ�Ŀ¼����� */
    b_dir = LD_DWORD( fs->win+DIR_BasePage );   ///< ��ȡ�ļ�Ŀ¼����ʼҳ��
    fs->dirpage = b_dir;
    
    dusize = LD_WORD( fs->win+DIR_SizePerUnit );    ///< ��ȡÿ��Ŀ¼�Ĵ�С
    fs->dusize = dusize;
    
    n_dunits = LD_DWORD( fs->win+DIR_NumUnits );    ///< ���ɴ洢�ļ�������
    fs->n_dunits = n_dunits;
    
    free_dunits = LD_DWORD( fs->win+DIR_FreeUnits );///< ʣ��ɴ洢����
    fs->free_dunits = free_dunits;
    
    /* �ļ�ҳ���������� */
    b_fil = LD_DWORD( fs->win+FIL_BasePage );       ///< ��ȡ�ļ�ҳ�����������ʼҳ��
    fs->filpage = b_fil;
    
    fusize = LD_WORD( fs->win+FIL_SizePerUnit );    ///< ��ȡÿ����¼�Ĵ�С
    fs->fusize = fusize;
    
    n_funits = LD_DWORD( fs->win+FIL_NumUnits );    ///< ��¼���������
    fs->n_funits = n_funits;
    
    free_funits = LD_DWORD( fs->win+FIL_FreeUnits );///< ʣ���¼�������
    fs->free_funits = free_funits;
    
    /* �ļ���������� */
    b_dat = LD_WORD( fs->win+DAT_BasePage );        ///< ��ȡ�ļ�����������ʼҳ��
    fs->datpage = b_dat;
    
    n_dpages = LD_DWORD( fs->win+DAT_NumPages );    ///< ��ȡ��������ҳ��
    fs->n_dpages = n_dpages;
    
    free_dpages = LD_DWORD( fs->win+DAT_FreePages );///< ��ȡ������ʣ��ҳ��
    fs->free_dpages = free_dpages;
    
    return E2PFR_OK;
}

/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/* ����E2P�ļ�ϵͳ
    0����鴫���������Ч��
    1��Check EEPROM()
    2�����ݺ궨�����ã������������ݶ�ȡ����ʽ���Ľṹ����
    3�����������ݵ���ЧУ��
    4�������������м�ֵ��������ȡ��������ŵ��ļ�ϵͳ�����
    5����λ�ļ�ϵͳ�Ĺ���״̬
*/
E2PFRESULT e2pf_mount(
	E2PFS* fs,			/* Pointer to the file system object (NULL:unmount)*/
	const uint8_t drv	/* Logical drive number to be mounted/unmounted */
)
{
    E2PFRESULT res=E2PFR_OK;
    
    E2PFS* cfs;
    
    /* ����豸���������Ƿ���� */
    if ( drv>=_E2P_VOLUMES )
        return E2PFR_INVALID_DRIVE;
    
    cfs = E2PFs[drv];
    if ( cfs )
        memset( cfs,sizeof(E2PFS),0 );
    
    /* ��ʼ���̷��ṹ�� */
    if ( fs ) {
        fs->fs_stat = 0;
        fs->drv_stat = DRVSTA_NOTRDY;
    }
    E2PFs[drv] = fs;
    
    /* ǿ�ƹ����豸 */
    res = mount_e2p_volume( fs,drv,0 );
    return res;
}
/* ����E2P�ļ�ϵͳ 
    1��check EEPROMоƬ
    2�������������⣬������������ȫ������
    3�����ݺ궨������ã�д����������
    4�����ݺ궨������ã�дĿ¼������
    5�����ݺ궨������ã�дҳ�����������
    6�����ݺ궨������ã�д���ݱ��е�����
*/
E2PFRESULT e2pf_mkfs(
	E2PFS* fs,			/* Pointer to the file system object (NULL:unmount)*/
	const uint8_t drv	/* Logical drive number to be mounted/unmounted */
)
{
    ESTATUS     stat;
    uint16_t    b_fat,b_dir,b_fil,b_dat,wpage;
    uint16_t    num_page;                       ///< ����
    uint8_t     siz_page;                       ///< ��С
    
    /* ����������������Ƿ���ȷ */
    if ( drv>=_E2P_VOLUMES )                                       
        return E2PFR_INVALID_DRIVE;
    
    if ( fs )
        memset( fs,sizeof(E2PFS),0 );
    else
        return E2PFR_E2P_ERR;
    
    /* ����ļ��豸״̬�Ƿ��ʼ������ҹ������� */
    if ( fs->drv_stat!=DRVSTA_OK ) {
        stat = e2p_initialize(drv);         ///< ��ʼ���ļ��豸
        if ( stat!=ESTA_OK ) {
            fs->drv_stat = DRVSTA_NOTRDY;
            return E2PFR_NOT_READY;
        }
        fs->drv_stat = DRVSTA_OK;           ///< ��ʼ����ɣ��޸��豸״̬
    }
    
    if ( e2p_ioctl(drv,GET_PAGE_NUM,&num_page)!=ERES_OK )    ///< ��ȡ�ļ��豸��ҳ��������
        return E2PFR_E2P_ERR;
    if ( e2p_ioctl(drv,GET_PAGE_SIZE,&siz_page)!=ERES_OK )   ///< ��ȡ�ļ��豸��ҳ���С
        return E2PFR_E2P_ERR;
    
    memset( fs->win,0x00,siz_page );
    
    /* �����ļ������������� */
    ST_WORD( fs->win+BS_55AA,0x55AA );
    ST_DWORD( fs->win+BS_FilSysType,0x455350 );
    fs->win[BS_BytsPerPage] = siz_page;
    ST_WORD( fs->win+BS_NumPages,num_page );

    b_fat = _E2P_MBR_BASE_PAGE_;    ///< ��ȡ�ļ�����������ʼҳ��
    if ( b_fat>num_page )
        return E2PFR_E2P_ERR;
    
    b_dir = _E2P_DIR_BASE_PAGE_;    ///< ��ȡ�ļ�Ŀ¼������ʼҳ��
    if ( b_dir>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+DIR_BasePage,b_dir );
    fs->win[DIR_SizePerUnit] = _E2P_DIR_UNIT_SIZE_;
    ST_WORD( fs->win+DIR_NumUnits, _E2P_PAGE_SIZE_*_E2P_DIR_PAGE_NUM_/_E2P_DIR_UNIT_SIZE_ );
    ST_WORD( fs->win+DIR_FreeUnits, _E2P_PAGE_SIZE_*_E2P_DIR_PAGE_NUM_/_E2P_DIR_UNIT_SIZE_ );
    
    b_fil = _E2P_FIL_BASE_PAGE_;    ///< ��ȡ�ļ�ҳ�����������ʼҳ��
    if ( b_fil>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+FIL_BasePage,b_fil );
    fs->win[FIL_SizePerUnit] = _E2P_FIL_UNIT_SIZE_;
    ST_WORD( fs->win+FIL_NumUnits, _E2P_PAGE_SIZE_*_E2P_FIL_PAGE_NUM_/_E2P_FIL_UNIT_SIZE_ );
    ST_WORD( fs->win+FIL_FreeUnits, _E2P_PAGE_SIZE_*_E2P_FIL_PAGE_NUM_/_E2P_FIL_UNIT_SIZE_ );
    
    b_dat = _E2P_DAT_BASE_PAGE_;    ///< ��ȡ�ļ�����������ʼҳ��
    if ( b_dat>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+DAT_BasePage,b_dat );
    fs->win[DAT_SizePerPage] = siz_page;
    ST_WORD( fs->win+DAT_NumPages,_E2P_DAT_PAGE_NUM_ );
    ST_WORD( fs->win+DAT_FreePages,_E2P_DAT_PAGE_NUM_ );
    
    if ( e2p_write(drv,fs->win,b_fat,1) != ERES_OK ) ///< д�ļ������������ݵ��豸��
        return E2PFR_E2P_ERR;
    
    /* ����ļ�Ŀ¼������,ȫ��дΪ0xFF */
    memset( fs->win,0xFF,siz_page );
    for ( wpage=b_dir;wpage<b_fil;wpage++ ) {
        if ( e2p_write( drv,fs->win,wpage,1 ) != ERES_OK )
            return E2PFR_E2P_ERR;
    }
    
    /* ��ʼ���ļ�ҳ������������м�¼��дΪ0xEEEE */
    memset( fs->win,0xEE,siz_page );
    for ( wpage=b_fil;wpage<b_dat;wpage++ ) {
        if ( e2p_write( drv,fs->win,wpage,1 ) != ERES_OK )
            return E2PFR_E2P_ERR;
    }
    
    return E2PFR_OK;
}


E2PFRESULT e2pf_open(
    E2PFIL* fp,
    const uint8_t* path,
    uint8_t mode
)
{
    E2PFRESULT  res;
    uint16_t    item_cnt,record;
//    uint16_t    page,posi;
    
    /* ���������Ч�Լ��� */
    if ( !fp )
        return E2PFR_INVALID_OBJECT;
    
    /* �����ļ�·����ȷ���ļ����ڵ��ļ�ϵͳ */
    fp->fs = E2PFs[0];
    

    /* �����ļ�������Ŀ¼�������ļ��Ƿ���� */
    res = DIR_Select( fp->fs,path,&fp->dir_page,&fp->dir_posi,&fp->fsize,&fp->fsitem );
    if ( res==E2PFR_NO_PATH ) {
        
        /* ��һ���ļ������ļ��������򴴽��ļ� */
        if ( mode & (E2PFA_CREATE_ALWAYS | E2PFA_OPEN_ALWAYS | E2PFA_CREATE_NEW) ) {
            if ( (fp->fs->free_dunits==0) || (fp->fs->free_funits==0) )
                return E2PFR_NO_FREE_AREA;
            
            /* ���������ļ�ҳ��������ļ�¼��ҵ�һ�����м�¼����Ϊ�ļ����ݵ���ʼҳ */
            for ( item_cnt=0; item_cnt<(fp->fs->n_funits); item_cnt++ ) {
                res = FIL_Select( fp->fs,item_cnt,&record );
                if ( res!=E2PFR_OK ) return res;
                
                /* �ж��Ƿ�Ϊ���м�¼�� */
                if ( record==0xEEEE ) {
                    memset( fp->fname,0xFF,4 );
                    res = DIR_Select(fp->fs,fp->fname,&fp->dir_page,&fp->dir_posi,&fp->fsize,&fp->fsitem);
                    if ( res!=E2PFR_OK ) return res;
                    
                    memcpy( fp->fname,path,4 );
                    fp->fsize = 0;
                    fp->fsitem = item_cnt;
                    
                    res = DIR_Update(fp->fs,fp->dir_page,fp->dir_posi,fp->fname,fp->fsize,fp->fsitem);
                    if ( res!=E2PFR_OK ) return res;
                    
                    fp->fs->free_dunits--;      ///< Ŀ¼��ʣ�������Ŀ����
                    
                    fp->fcitem = fp->fsitem;
                    fp->fnitem = 0xFFFF;
                    
                    res = FIL_Update( fp->fs,fp->fcitem,fp->fnitem );
                    if ( res!=E2PFR_OK ) return res;
                    
                    fp->fs->free_funits--;      ///< �ļ�ҳ�������ʣ�������Ŀ����
                    fp->fs->free_dpages--;      ///< ������ʣ�����ҳ��
                    
                    res = MBR_Update( fp->fs, fp->fs->free_dunits, fp->fs->free_funits, fp->fs->free_dpages );
                    if ( res!=E2PFR_OK ) return res;
                    
                    break;
                }
            }
            
        }
        else 
            return res;
        
    }
    else if ( res==E2PFR_OK ) {
        memcpy( fp->fname,path,4 );
        fp->fcitem = fp->fsitem;
        res = FIL_Select( fp->fs,fp->fcitem,&fp->fnitem );
        if (res!=E2PFR_OK) return res;
    }
    else
        return res;
    
    if ( res==E2PFR_OK ) {
        fp->flag = mode;
        fp->fptr = 0;
        fp->fitem_cnt = 0;
    }
    
    return res;
}


E2PFRESULT e2pf_read (
    E2PFIL*     fp,
    const void* buff,
    uint32_t    btr,
    uint32_t*   br
)
{
    E2PFRESULT res;
    uint16_t remain;
    uint16_t page,posi;
    uint16_t rcnt,fitem_cnt;
    uint8_t*    rbuff = (uint8_t*)buff;
    uint16_t b_dat;
    
    *br = 0;
    
    /* ���������������Ч�� */
    if ( !fp || !buff  )
        return E2PFR_INVALID_PARAMETER;
    b_dat = fp->fs->datpage;
    
    /* ʣ���ֽ��� */
    remain = (fp->fsize)-(fp->fptr);
    if (btr>remain) btr = remain;
    
    /* �����ļ���дָ�룬ȷ��ָ��ָ�������ҳ */
    fitem_cnt = fp->fptr/(fp->fs->psize);                                   ///< ��ȡ�ļ���дָ��ָ���ļ����ݵĵڼ�ҳ
    
    if ( fp->fitem_cnt != fitem_cnt ) {
        fp->fcitem = fp->fsitem;                                            ///< ����ǰҳָ����ʼҳ
        res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);                    ///< ��ȡ��һ��ҳ
        if ( res!=E2PFR_OK ) return res;
        for ( fp->fitem_cnt=0;fp->fitem_cnt<fitem_cnt;fp->fitem_cnt++ ) {   ///< ͨ������ҳ��������ļ�¼��ҵ�Ŀ��ҳ��Ŀ��ҳ����һҳ
            fp->fcitem = fp->fnitem;
            if ( fp->fcitem==0xFFFF )
                return E2PFR_E2P_ERR;
            
            res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);
            if ( res!=E2PFR_OK ) return res;
        }
    }
    
    /* �ظ���ȡ����ֱ��ȫ����ȡ�� */
    for ( ;btr; rbuff+=rcnt,fp->fptr+=rcnt,*br+=rcnt,btr-=rcnt) {
        if (fp->fcitem==0xFFFF)
            break;
        
        page = fp->fcitem;
        posi = fp->fptr%(fp->fs->psize);
        rcnt = (fp->fs->psize)-posi;
        if (btr<rcnt)
            rcnt = btr;
        else {
            fp->fcitem = fp->fnitem;
            fp->fitem_cnt++;
        }
        
        res  = load_window(fp->fs,b_dat+page);
        if ( res!=E2PFR_OK ) return res;
        memcpy( rbuff,fp->fs->win+posi,rcnt );
        
        res = FIL_Select( fp->fs,fp->fcitem,&fp->fnitem );
        if ( res!=E2PFR_OK ) return res;
    }

    return res;
}

E2PFRESULT e2pf_write(
    E2PFIL*     fp,
    const void* buff,
    uint32_t    btw,
    uint32_t*   bw
)
{
    E2PFRESULT  res;
    uint16_t    b_dat;
    uint16_t    page,posi;
    uint16_t    wcnt,fitem_cnt;
    uint16_t    fsize,record;
    uint8_t*    wbuff = (uint8_t*)buff;
    
    /* �жϲ�������Ч�� */
    if ( !fp || !buff )
        return E2PFR_INVALID_PARAMETER;
    
    /* �ж��ļ��򿪷�ʽ */
    if ( !(fp->flag&E2PFA_WRITE) )
        return E2PFR_DENIED;                ///< ����ļ����ǿ�д��ʽ�򿪣��򷵻��޷���Ȩ��
    
    b_dat = fp->fs->datpage;
    
    /* �����ļ���дָ�룬��λָ��ָ�������ҳ */
    fitem_cnt = fp->fptr/(fp->fs->psize);                                   ///< �����ļ���дָ����ָ���ļ����ݵĵڼ�ҳ
    
    /* �ж���һ���ļ���ֵ�͵�ǰ״ֵ̬�Ƿ�ƥ�䣬����ƥ�䣬������ݼ���ֵ���µ�ǰ��״ֵ̬��fp->fitem_cnt��fp->fcitem��fp->fnitem */
    if ( fp->fitem_cnt != fitem_cnt ) {
        fp->fcitem = fp->fsitem;                                            ///< ���õ�ǰҳΪ�ļ���ʼҳ
        res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);                    ///< ��ȡ��һҳ
        if ( res!=E2PFR_OK ) return res;
        for ( fp->fitem_cnt=0;fp->fitem_cnt<fitem_cnt;fp->fitem_cnt++ ) {   ///< ͨ������ҳ��������ļ�¼��ҵ�Ŀ��ҳ��Ŀ��ҳ����һҳ
            fp->fcitem = fp->fnitem;
            if ( fp->fcitem==0xFFFF )
                return E2PFR_E2P_ERR;
            
            res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);
            if ( res!=E2PFR_OK ) return res;
        }
    }
    
    /* �ظ�д������ֱ��ȫ��д�� */
    wcnt = 0;                                                                           ///< ���ڼ�¼д�����ݵĸ���
    for ( ;btw; wbuff+=wcnt,fp->fptr+=wcnt,(*bw)+=wcnt,btw-=wcnt) {
        
//        printf( "btw::%d\tbw:%d\n",btw,*bw );
        
        /* �ж��Ƿ�Ϊ��һ��д���ݣ�������ǣ������л���ǰҳ����һҳ */
        if ( wcnt ) {
            if ( fp->fnitem==0xFFFF ) {                                                 ///< �����һҳҳ��ֵΪ0XFFFF��˵����ǰҳΪ���һҳ����Ҫ����һ������ҳ
                if ( !(fp->fs->free_funits) || !(fp->fs->free_dpages) )
                    return E2PFR_NO_FREE_AREA;
                
                /* ͨ������ҳ��������ļ�¼�����һ�ſ�����ҳ��Ϊ��ǰҳ�����һҳ */
                for ( fitem_cnt=0; fitem_cnt<(fp->fs->n_funits); fitem_cnt++ ) {
                    res = FIL_Select( fp->fs, fitem_cnt, &record );
                    if ( res!=E2PFR_OK ) return res;
                    
                    if ( record==0xEEEE ) {
                        fp->fnitem = fitem_cnt;
                        
                        res = FIL_Update( fp->fs, fp->fcitem, fp->fnitem );             ///< ���µ�ǰҳ���Ӧ�ļ�¼����ļ�βҳ��־0xFFFF����Ϊ������ҳ��ҳ��
                        if ( res!=E2PFR_OK ) return res;
                        
                        res = FIL_Update( fp->fs, fp->fnitem, 0xFFFF  );                  ///< ���¿�����ҳ��Ӧ�ļ�¼��ɿ��б�־0xEEEE����Ϊ�ļ�βҳ��־0xFFFF.
                        if ( res!=E2PFR_OK ) return res;
                        
//                        res = FIL_Select( fp->fs, fp->fcitem, &page, &posi, &record );  ///< ��ȡ��ǰҳ���Ӧ�ļ�¼��Ĵ洢λ��
//                        if ( res!=E2PFR_OK ) return res;
//                        
//                        res = FIL_Update( fp->fs, page, posi, fp->fnitem );             ///< ���µ�ǰҳ���Ӧ�ļ�¼����ļ�βҳ��־0xFFFF����Ϊ������ҳ��ҳ��
//                        if ( res!=E2PFR_OK ) return res;
                        
                        fp->fcitem = fp->fnitem;            ///< �л���ǰҳ��
                        fp->fnitem = 0xFFFF;                ///< ������һҳ��
                        fp->fitem_cnt++;                    ///< ����ҳ���¼��
                        
                        break;
                    }
                }
            }
            else {
                fp->fcitem = fp->fnitem;
                
                res = FIL_Select( fp->fs, fp->fcitem, &fp->fnitem );
                if ( res!=E2PFR_OK ) return res;
                
                fp->fitem_cnt++;
            }
        }
        
        /* ��ǰҳ��д���� */
        
        if ( fp->fcitem==0xFFFF )
            return E2PFR_E2P_ERR;
        
        page = fp->fcitem;
        posi = fp->fptr%(fp->fs->psize);
        wcnt = (fp->fs->psize)-posi;
        if ( wcnt>=btw )
            wcnt = btw;
        
        res  = load_window(fp->fs,b_dat+page);
        if ( res!=E2PFR_OK ) return res;
        
        memset( fp->fs->win,0,_E2P_PAGE_SIZE_ );
        memcpy( fp->fs->win+posi,wbuff,wcnt );
        if ( e2p_write( fp->fs->drv,fp->fs->win,b_dat+page,1 ) != ERES_OK)
            return E2PFR_E2P_ERR;
    }
    
    /* �����ļ���С */
//    if ( fp->fptr > fp->fsize ) {
//        fp->fsize = fp->fptr;
//        res = DIR_Update( fp->fs,fp->dir_page,fp->dir_posi,fp->fname,fp->fsize,fp->fsitem );
//        if ( res!=E2PFR_OK ) return res;
//    }
    if ( fp->fptr < fp->fsize ) {
        if ( fp->fnitem != 0xFFFF ) {
            FIL_Delete( fp->fs,fp->fnitem );
            FIL_Update( fp->fs,fp->fnitem,0xFFFF );
        }
    }
    fp->fsize = fp->fptr;
    res = DIR_Update( fp->fs,fp->dir_page,fp->dir_posi,fp->fname,fp->fsize,fp->fsitem );
    if ( res!=E2PFR_OK ) return res;
    
    return res;
}

/**
  * @brief  �ض�λ��(������/�ļ�)�ϵ��ļ��ڲ�λ��ָ��
  * @param  fp:�ļ�������
  * @param  offset:ƫ����
  * @param  fromwhere:ƫ����ʼλ��
  * @note   �ļ�ָ��ָ���ļ�/����λ��ָ��ָ���ļ��ڲ����ֽ�λ�ã������ļ��Ķ�ȡ���ƶ����ļ�ָ����������¸�ֵ������ı��ָ�����ļ���
  * @retval 0:�ɹ� ������ʧ�� ����ֵ�ο�@E2PFRESULT
  */
E2PFRESULT e2pf_seek(
    E2PFIL  *fp,
    long    offset,
    int     fromwhere
)
{
    long cur    = fp->fptr;
    long fsize  = fp->fsize;
    int  fptr;
    
    /* �жϲ�������Ч�� */
    if ( !fp || !IS_E2PFA_SEEK_FLG(fromwhere) )
        return E2PFR_INVALID_PARAMETER;
    
    /* �޸��ļ���дָ���λ�� */
    switch ( fromwhere ) {
        case E2PFA_SEEK_SET:
            fptr = offset;
            break;
        
        case E2PFA_SEEK_CUR:
            fptr = cur + offset;
            break;
        
        case E2PFA_SEEK_END:
            fptr = fsize + offset;
            break;
        
        default:
            break;
    }
    if ( fptr<0 || fptr>fsize ) return E2PFR_E2P_ERR;
    
    fp->fptr = fptr;    
    return E2PFR_OK;
}

E2PFRESULT e2pf_close(
    E2PFIL* fp
)
{
    
    
    return E2PFR_OK;
}



