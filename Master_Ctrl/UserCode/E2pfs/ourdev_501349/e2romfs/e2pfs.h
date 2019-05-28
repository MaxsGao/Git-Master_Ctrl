/******************************* (C) Embest ***********************************
* File Name          : e2pfs.h
* Author             : liren
* Date               : 2009-11-6
* Version            : 
* Description        : 
******************************************************************************/

#ifndef __E2ROMFS_H__
#define __E2ROMFS_H__

/* --------------------------------------------------------------------------------*/

/*错误代码*/
#ifndef EOF
#define EOF					(-1)
#endif
#define ERR_ARG				(-2)					/* 参数错误*/
#define ERR_MEM				(-3)					/* 空间不足*/
#define ERR_WRP				(-4)					/* 页面读写超出边界*/	
#define ERR_CRI				(-5)					/* 进入临界区失败*/
#define ERR_ECC				(-6)					/* 校验错误*/
#define ERR_EFD				(-7)					/* 错误文件描述符*/
#define ERR_FOP				(-8)					/* 文件已经打开，不能多次打开*/
//#define ERR_FNO				(-8)					/* 描述符没有对应打开文件*/

#ifndef SEEK_SET
#define SEEK_SET	1
#endif
#ifndef SEEK_CUR
#define SEEK_CUR	2
#endif
#ifndef SEEK_END
#define SEEK_END	3
#endif

int e2pfs_format(void);
int e2pfs_open(int id);
int e2pfs_read(int fd, void *buf, int len);
int e2pfs_write(int fd, void *buf, int len);
int e2pfs_lseek(int fd, int offset, int which);
int e2pfs_close(int fd);
int e2pfs_remove(int fid);

int e2pfs_fsinfo(void);
int e2pfs_file_pgs(int id);
int e2pfs_fexist(int id);							/* 判断给定的文件是否存在*/ 
int e2pfs_ls(void);									/* 显示文件信息及EEPROM使用信息*/
int e2pfs_file_pgs(int id);						/* 计算文件使用的页面数*/

#endif
