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
#include "ef.h"
#include "string.h"


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
*                                       VARIABLES           变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  加载磁盘访问窗口的数据
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
    
    /* 加载文件目录区相应页面的数据 */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 修改数据 */
    ST_WORD( fs->win+DIR_FreeUnits,free_dunits );
    ST_WORD( fs->win+FIL_FreeUnits,free_funits );
    ST_WORD( fs->win+DAT_FreePages,free_dpages );
    
    /* 将修改完成的文件目录信息写入到EEPROM中存储起来 */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    return E2PFR_OK;
}

/**
  * @brief  文件目录信息的查找操作,将查找到的结果输出到文件描述符结构体
  * @param  所在文件系统
  * @param  文件路径/文件名称
  * @note   
  * @retval 查找结果
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
    
    /* 输入参数有效性检验 */
    if ( fs==NULL || fname==NULL )
        return E2PFR_INVALID_PARAMETER;
    
    psize   = fs->psize;
    dusize  = fs->dusize;
    
    /* 逐页遍历 */
    for ( page_cnt=fs->dirpage;page_cnt<fs->filpage;page_cnt++ ) {
        
        /* 加载文件目录区相应页面的数据 */
        if ( load_window(fs,page_cnt) != E2PFR_OK )
            return E2PFR_E2P_ERR;
        
        /* 逐条遍历 */
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
  * @brief  文件目录信息的修改操作
  * @param  
  * @note   
  * @retval 修改结果
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
    /* 加载文件目录区相应页面的数据 */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 修改要添加的文件的目录信息 */
    memcpy ( fs->win+posi+DIR_Fname, fname, 4 );
    ST_WORD( fs->win+posi+DIR_Fsize, fsize    );
    ST_WORD( fs->win+posi+DIR_Fspage,fsitem   );
    
    /* 将修改完成的文件目录信息写入到EEPROM中存储起来 */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    return E2PFR_OK;
}

/**
  * @brief  文件目录信息的删除操作
  * @param  
  * @note   
  * @retval 修改结果
  */
static E2PFRESULT DIR_Delete(
    __IN__ E2PFS*   fs,
    __IN__ uint16_t page,
    __IN__ uint16_t posi
)
{
    /* 加载文件目录区相应页面的数据 */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 修改文件目录信息为 */
    memset( fs->win+posi+DIR_Fname, 0xFF,fs->dusize );
    
    /* 将修改完成的文件目录信息写入到EEPROM中存储起来 */
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
    
    /* 根据记录项序号，确定所在页数和位置 */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* 加载文件页面分配区相应页面的数据 */
    if ( load_window( fs,page ) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 获取该记录项中记录的内容 */
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
    
    /* 根据记录项序号，确定所在页数和位置 */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* 加载文件目录区相应页面的数据 */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 修改要添加的文件的目录信息 */
    ST_WORD( fs->win+posi, record );
    
    /* 将修改完成的文件目录信息写入到EEPROM中存储起来 */
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
    
    
    /* 清空当前记录项对应数据页的内容 */
    memset( fs->win,0,_E2P_PAGE_SIZE_ );
    if ( e2p_write( fs->drv,fs->win,fs->datpage+n_item,1 ) != ERES_OK)
        return E2PFR_E2P_ERR;
    
    /* 根据记录项序号，确定所在页数和位置 */
    uint16_t page = fs->filpage + n_item/(psize/isize);
    uint16_t posi = n_item%(psize/isize)*isize;
    
    /* 加载文件目录区相应页面的数据 */
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 获取该记录项中记录的内容 */
    uint16_t n_next_item = LD_WORD( fs->win+(posi) );
    
    /* 修改要添加的文件的目录信息 */
    ST_WORD( fs->win+posi, 0xEEEE );
    
    /* 将修改完成的文件目录信息写入到EEPROM中存储起来 */
    if ( e2p_write( fs->drv,fs->win,page,1 ) != ERES_OK )
        return E2PFR_E2P_ERR;
    
    /* 递归停止条件 */
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
    /* 读取文件引导区的内容 */
    fs->wflag = 0;
    fs->winpage = 0xFFFF;
    if ( load_window(fs,page) != E2PFR_OK )
        return E2PFR_E2P_ERR;
    
    /* 检查文件引导区 */
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

    /* 检查文件设备状态是否初始化完成且功能正常 */
    if ( fs->drv_stat!=DRVSTA_OK ) {
        stat = e2p_initialize(drv);         ///< 初始化文件设备
        if ( stat!=ESTA_OK ) {
            fs->drv_stat = DRVSTA_NOTRDY;
            return E2PFR_NOT_READY;
        }
        fs->drv_stat = DRVSTA_OK;           ///< 修改文件设备的状态
    }
    
    b_fat = _E2P_MBR_BASE_PAGE_;            ///< 获取文件引导区起始页码
    res = check_fs( fs,b_fat );             ///< 检查文件系统是否正确
    if ( res!=E2PFR_OK )
        return res;
    
    fs->drv = drv;      ///< 获取设备驱动器号
    
    psize = LD_WORD( fs->win+BS_BytsPerPage );  ///< 提取文件设备的页面大小
    fs->psize = psize;
    
    n_pages = LD_DWORD( fs->win+BS_NumPages );  ///< 提取文件设备的页面总数量
    fs->n_pages = n_pages;
    
    /* 文件目录区相关 */
    b_dir = LD_DWORD( fs->win+DIR_BasePage );   ///< 提取文件目录区起始页码
    fs->dirpage = b_dir;
    
    dusize = LD_WORD( fs->win+DIR_SizePerUnit );    ///< 获取每条目录的大小
    fs->dusize = dusize;
    
    n_dunits = LD_DWORD( fs->win+DIR_NumUnits );    ///< 最多可存储文件的数量
    fs->n_dunits = n_dunits;
    
    free_dunits = LD_DWORD( fs->win+DIR_FreeUnits );///< 剩余可存储数量
    fs->free_dunits = free_dunits;
    
    /* 文件页面管理区相关 */
    b_fil = LD_DWORD( fs->win+FIL_BasePage );       ///< 提取文件页面分配区的起始页码
    fs->filpage = b_fil;
    
    fusize = LD_WORD( fs->win+FIL_SizePerUnit );    ///< 获取每条记录的大小
    fs->fusize = fusize;
    
    n_funits = LD_DWORD( fs->win+FIL_NumUnits );    ///< 记录项的总数量
    fs->n_funits = n_funits;
    
    free_funits = LD_DWORD( fs->win+FIL_FreeUnits );///< 剩余记录项的数量
    fs->free_funits = free_funits;
    
    /* 文件数据区相关 */
    b_dat = LD_WORD( fs->win+DAT_BasePage );        ///< 提取文件数据区的起始页码
    fs->datpage = b_dat;
    
    n_dpages = LD_DWORD( fs->win+DAT_NumPages );    ///< 获取数据区总页数
    fs->n_dpages = n_dpages;
    
    free_dpages = LD_DWORD( fs->win+DAT_FreePages );///< 获取数据区剩余页数
    fs->free_dpages = free_dpages;
    
    return E2PFR_OK;
}

/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/* 挂载E2P文件系统
    0、检查传入参数的有效性
    1、Check EEPROM()
    2、根据宏定义配置，将引导区内容读取到格式化的结构体中
    3、引导区内容的有效校验
    4、将引导区中有价值的内容提取出来并存放到文件系统句柄中
    5、置位文件系统的挂载状态
*/
E2PFRESULT e2pf_mount(
	E2PFS* fs,			/* Pointer to the file system object (NULL:unmount)*/
	const uint8_t drv	/* Logical drive number to be mounted/unmounted */
)
{
    E2PFRESULT res=E2PFR_OK;
    
    E2PFS* cfs;
    
    /* 检查设备驱动器号是否合理 */
    if ( drv>=_E2P_VOLUMES )
        return E2PFR_INVALID_DRIVE;
    
    cfs = E2PFs[drv];
    if ( cfs )
        memset( cfs,sizeof(E2PFS),0 );
    
    /* 初始化盘符结构体 */
    if ( fs ) {
        fs->fs_stat = 0;
        fs->drv_stat = DRVSTA_NOTRDY;
    }
    E2PFs[drv] = fs;
    
    /* 强制挂载设备 */
    res = mount_e2p_volume( fs,drv,0 );
    return res;
}
/* 创建E2P文件系统 
    1、check EEPROM芯片
    2、将除数据区外，其他所有内容全部清零
    3、根据宏定义的配置，写引导区内容
    4、根据宏定义的配置，写目录区内容
    5、根据宏定义的配置，写页面分配区内容
    6、根据宏定义的配置，写数据表中的内容
*/
E2PFRESULT e2pf_mkfs(
	E2PFS* fs,			/* Pointer to the file system object (NULL:unmount)*/
	const uint8_t drv	/* Logical drive number to be mounted/unmounted */
)
{
    ESTATUS     stat;
    uint16_t    b_fat,b_dir,b_fil,b_dat,wpage;
    uint16_t    num_page;                       ///< 数量
    uint8_t     siz_page;                       ///< 大小
    
    /* 检查物理驱动器号是否正确 */
    if ( drv>=_E2P_VOLUMES )                                       
        return E2PFR_INVALID_DRIVE;
    
    if ( fs )
        memset( fs,sizeof(E2PFS),0 );
    else
        return E2PFR_E2P_ERR;
    
    /* 检查文件设备状态是否初始化完成且功能正常 */
    if ( fs->drv_stat!=DRVSTA_OK ) {
        stat = e2p_initialize(drv);         ///< 初始化文件设备
        if ( stat!=ESTA_OK ) {
            fs->drv_stat = DRVSTA_NOTRDY;
            return E2PFR_NOT_READY;
        }
        fs->drv_stat = DRVSTA_OK;           ///< 初始化完成，修改设备状态
    }
    
    if ( e2p_ioctl(drv,GET_PAGE_NUM,&num_page)!=ERES_OK )    ///< 获取文件设备的页面总数量
        return E2PFR_E2P_ERR;
    if ( e2p_ioctl(drv,GET_PAGE_SIZE,&siz_page)!=ERES_OK )   ///< 获取文件设备的页面大小
        return E2PFR_E2P_ERR;
    
    memset( fs->win,0x00,siz_page );
    
    /* 配置文件引导区的内容 */
    ST_WORD( fs->win+BS_55AA,0x55AA );
    ST_DWORD( fs->win+BS_FilSysType,0x455350 );
    fs->win[BS_BytsPerPage] = siz_page;
    ST_WORD( fs->win+BS_NumPages,num_page );

    b_fat = _E2P_MBR_BASE_PAGE_;    ///< 获取文件引导区的起始页码
    if ( b_fat>num_page )
        return E2PFR_E2P_ERR;
    
    b_dir = _E2P_DIR_BASE_PAGE_;    ///< 获取文件目录区的起始页码
    if ( b_dir>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+DIR_BasePage,b_dir );
    fs->win[DIR_SizePerUnit] = _E2P_DIR_UNIT_SIZE_;
    ST_WORD( fs->win+DIR_NumUnits, _E2P_PAGE_SIZE_*_E2P_DIR_PAGE_NUM_/_E2P_DIR_UNIT_SIZE_ );
    ST_WORD( fs->win+DIR_FreeUnits, _E2P_PAGE_SIZE_*_E2P_DIR_PAGE_NUM_/_E2P_DIR_UNIT_SIZE_ );
    
    b_fil = _E2P_FIL_BASE_PAGE_;    ///< 获取文件页面分配区的起始页码
    if ( b_fil>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+FIL_BasePage,b_fil );
    fs->win[FIL_SizePerUnit] = _E2P_FIL_UNIT_SIZE_;
    ST_WORD( fs->win+FIL_NumUnits, _E2P_PAGE_SIZE_*_E2P_FIL_PAGE_NUM_/_E2P_FIL_UNIT_SIZE_ );
    ST_WORD( fs->win+FIL_FreeUnits, _E2P_PAGE_SIZE_*_E2P_FIL_PAGE_NUM_/_E2P_FIL_UNIT_SIZE_ );
    
    b_dat = _E2P_DAT_BASE_PAGE_;    ///< 获取文件数据区的起始页码
    if ( b_dat>num_page )
        return E2PFR_E2P_ERR;
    ST_WORD( fs->win+DAT_BasePage,b_dat );
    fs->win[DAT_SizePerPage] = siz_page;
    ST_WORD( fs->win+DAT_NumPages,_E2P_DAT_PAGE_NUM_ );
    ST_WORD( fs->win+DAT_FreePages,_E2P_DAT_PAGE_NUM_ );
    
    if ( e2p_write(drv,fs->win,b_fat,1) != ERES_OK ) ///< 写文件引导区的内容到设备中
        return E2PFR_E2P_ERR;
    
    /* 清空文件目录区内容,全部写为0xFF */
    memset( fs->win,0xFF,siz_page );
    for ( wpage=b_dir;wpage<b_fil;wpage++ ) {
        if ( e2p_write( drv,fs->win,wpage,1 ) != ERES_OK )
            return E2PFR_E2P_ERR;
    }
    
    /* 初始化文件页面分配区的所有记录项写为0xEEEE */
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
    
    /* 输入参数有效性检验 */
    if ( !fp )
        return E2PFR_INVALID_OBJECT;
    
    /* 根据文件路径，确定文件所在的文件系统 */
    fp->fs = E2PFs[0];
    

    /* 根据文件名，在目录区查找文件是否存在 */
    res = DIR_Select( fp->fs,path,&fp->dir_page,&fp->dir_posi,&fp->fsize,&fp->fsitem );
    if ( res==E2PFR_NO_PATH ) {
        
        /* 打开一个文件，若文件不存在则创建文件 */
        if ( mode & (E2PFA_CREATE_ALWAYS | E2PFA_OPEN_ALWAYS | E2PFA_CREATE_NEW) ) {
            if ( (fp->fs->free_dunits==0) || (fp->fs->free_funits==0) )
                return E2PFR_NO_FREE_AREA;
            
            /* 逐条遍历文件页面分配区的记录项，找到一个空闲记录项作为文件内容的起始页 */
            for ( item_cnt=0; item_cnt<(fp->fs->n_funits); item_cnt++ ) {
                res = FIL_Select( fp->fs,item_cnt,&record );
                if ( res!=E2PFR_OK ) return res;
                
                /* 判断是否为空闲记录项 */
                if ( record==0xEEEE ) {
                    memset( fp->fname,0xFF,4 );
                    res = DIR_Select(fp->fs,fp->fname,&fp->dir_page,&fp->dir_posi,&fp->fsize,&fp->fsitem);
                    if ( res!=E2PFR_OK ) return res;
                    
                    memcpy( fp->fname,path,4 );
                    fp->fsize = 0;
                    fp->fsitem = item_cnt;
                    
                    res = DIR_Update(fp->fs,fp->dir_page,fp->dir_posi,fp->fname,fp->fsize,fp->fsitem);
                    if ( res!=E2PFR_OK ) return res;
                    
                    fp->fs->free_dunits--;      ///< 目录区剩余可用条目数量
                    
                    fp->fcitem = fp->fsitem;
                    fp->fnitem = 0xFFFF;
                    
                    res = FIL_Update( fp->fs,fp->fcitem,fp->fnitem );
                    if ( res!=E2PFR_OK ) return res;
                    
                    fp->fs->free_funits--;      ///< 文件页面分配区剩余可用条目数量
                    fp->fs->free_dpages--;      ///< 数据区剩余可用页数
                    
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
    
    /* 检验输入参数的有效性 */
    if ( !fp || !buff  )
        return E2PFR_INVALID_PARAMETER;
    b_dat = fp->fs->datpage;
    
    /* 剩余字节数 */
    remain = (fp->fsize)-(fp->fptr);
    if (btr>remain) btr = remain;
    
    /* 根据文件读写指针，确定指针指向的所在页 */
    fitem_cnt = fp->fptr/(fp->fs->psize);                                   ///< 获取文件读写指针指向文件内容的第几页
    
    if ( fp->fitem_cnt != fitem_cnt ) {
        fp->fcitem = fp->fsitem;                                            ///< 将当前页指向起始页
        res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);                    ///< 获取下一个页
        if ( res!=E2PFR_OK ) return res;
        for ( fp->fitem_cnt=0;fp->fitem_cnt<fitem_cnt;fp->fitem_cnt++ ) {   ///< 通过遍历页面分配区的记录项，找到目标页和目标页的下一页
            fp->fcitem = fp->fnitem;
            if ( fp->fcitem==0xFFFF )
                return E2PFR_E2P_ERR;
            
            res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);
            if ( res!=E2PFR_OK ) return res;
        }
    }
    
    /* 重复读取数据直到全部读取完 */
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
    
    /* 判断参数的有效性 */
    if ( !fp || !buff )
        return E2PFR_INVALID_PARAMETER;
    
    /* 判断文件打开方式 */
    if ( !(fp->flag&E2PFA_WRITE) )
        return E2PFR_DENIED;                ///< 如果文件不是可写方式打开，则返回无访问权限
    
    b_dat = fp->fs->datpage;
    
    /* 根据文件读写指针，定位指针指向的所在页 */
    fitem_cnt = fp->fptr/(fp->fs->psize);                                   ///< 计算文件读写指针所指向文件内容的第几页
    
    /* 判断上一步的计算值和当前状态值是否匹配，若不匹配，则需根据计算值更新当前的状态值，fp->fitem_cnt、fp->fcitem、fp->fnitem */
    if ( fp->fitem_cnt != fitem_cnt ) {
        fp->fcitem = fp->fsitem;                                            ///< 设置当前页为文件起始页
        res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);                    ///< 获取下一页
        if ( res!=E2PFR_OK ) return res;
        for ( fp->fitem_cnt=0;fp->fitem_cnt<fitem_cnt;fp->fitem_cnt++ ) {   ///< 通过遍历页面分配区的记录项，找到目标页和目标页的下一页
            fp->fcitem = fp->fnitem;
            if ( fp->fcitem==0xFFFF )
                return E2PFR_E2P_ERR;
            
            res = FIL_Select(fp->fs,fp->fcitem,&fp->fnitem);
            if ( res!=E2PFR_OK ) return res;
        }
    }
    
    /* 重复写入数据直到全部写完 */
    wcnt = 0;                                                                           ///< 用于记录写入数据的个数
    for ( ;btw; wbuff+=wcnt,fp->fptr+=wcnt,(*bw)+=wcnt,btw-=wcnt) {
        
//        printf( "btw::%d\tbw:%d\n",btw,*bw );
        
        /* 判断是否为第一次写数据，如果不是，则需切换当前页到下一页 */
        if ( wcnt ) {
            if ( fp->fnitem==0xFFFF ) {                                                 ///< 如果下一页页码值为0XFFFF，说明当前页为最后一页，需要申请一空闲新页
                if ( !(fp->fs->free_funits) || !(fp->fs->free_dpages) )
                    return E2PFR_NO_FREE_AREA;
                
                /* 通过遍历页面分配区的记录项，申请一张空闲新页作为当前页面的下一页 */
                for ( fitem_cnt=0; fitem_cnt<(fp->fs->n_funits); fitem_cnt++ ) {
                    res = FIL_Select( fp->fs, fitem_cnt, &record );
                    if ( res!=E2PFR_OK ) return res;
                    
                    if ( record==0xEEEE ) {
                        fp->fnitem = fitem_cnt;
                        
                        res = FIL_Update( fp->fs, fp->fcitem, fp->fnitem );             ///< 更新当前页面对应的记录项，由文件尾页标志0xFFFF更改为空闲新页的页码
                        if ( res!=E2PFR_OK ) return res;
                        
                        res = FIL_Update( fp->fs, fp->fnitem, 0xFFFF  );                  ///< 更新空闲新页对应的记录项，由空闲标志0xEEEE更改为文件尾页标志0xFFFF.
                        if ( res!=E2PFR_OK ) return res;
                        
//                        res = FIL_Select( fp->fs, fp->fcitem, &page, &posi, &record );  ///< 获取当前页面对应的记录项的存储位置
//                        if ( res!=E2PFR_OK ) return res;
//                        
//                        res = FIL_Update( fp->fs, page, posi, fp->fnitem );             ///< 更新当前页面对应的记录项，由文件尾页标志0xFFFF更改为空闲新页的页码
//                        if ( res!=E2PFR_OK ) return res;
                        
                        fp->fcitem = fp->fnitem;            ///< 切换当前页面
                        fp->fnitem = 0xFFFF;                ///< 更新下一页面
                        fp->fitem_cnt++;                    ///< 更新页面记录器
                        
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
        
        /* 向当前页面写数据 */
        
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
    
    /* 更新文件大小 */
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
  * @brief  重定位流(数据流/文件)上的文件内部位置指针
  * @param  fp:文件描述符
  * @param  offset:偏移量
  * @param  fromwhere:偏移起始位置
  * @note   文件指针指向文件/流。位置指针指向文件内部的字节位置，随着文件的读取会移动，文件指针如果不重新赋值将不会改变或指向别的文件。
  * @retval 0:成功 其他：失败 返回值参考@E2PFRESULT
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
    
    /* 判断参数的有效性 */
    if ( !fp || !IS_E2PFA_SEEK_FLG(fromwhere) )
        return E2PFR_INVALID_PARAMETER;
    
    /* 修改文件读写指针的位置 */
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



