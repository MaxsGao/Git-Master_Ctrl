/******************************* (C) Embest ***********************************
* File Name          : e2pfs_port.h
* Author             : liren
* Date               : 2009-11-6
* Version            : 
* Description        : 
******************************************************************************/

#ifndef __E2PFS_PORT_H__
#define __E2PFS_PORT_H__

/* 调试信息支持 0：关掉所有输出 1：出错信息 2：出错与警告 3：所有信息*/
#define E2PFS_DEBUG			1						

#define E2PFS_EFUN			1						/* 一些辅助函数支持,调试时有用*/
  
#define	E2PFS_ECC			1						/* 0：不支持 1：校验支持（奇偶）*/

/* 如果支持校验，_ECC_SIZE为校验码长度 不能超过4（B）*/
#define _ECC_SIZE			(E2PFS_ECC ? 1 : 0)	

#define E2PFS_FILES			5						/* E2PFS支持创建的最大文件数 */

#define E2PFS_OFILES		2						/* E2PFS支持打开最大文件数<=E2PFS_FILES*/

//#define E2PFS_FNAME			5						/*文件名长度，没有保存末尾'\0'*/

#define E2PFS_CDATE			0						/* 文件创建时间支持*/

#define E2PFS_ADATE			1						/* 文件最后访问时间支持*/

/* EEPROM 相关选项 */
#define E2PFS_ADDR_BEGIN	0						/* EEPROM中划分给文件系统的起始物理地址*/

#define E2PFS_PAGE_OFFS		4						/* 文件系统页面地址偏移,不能超过15*/

#define E2PFS_PAGES			64						/* 文件系统使用的页面数*/

typedef unsigned short		fsize_t;				/* 系统支持的最大文件长度*/

typedef unsigned char		fid_t;					/* 文件索引号类型*/

extern int e2prom_read(int addr, void *_buf, int len);		/* EEPROM底层随机读函数*/

extern int e2prom_write(int addr, void *_buf, int len);		/* EEPROM底层随机写函数*/
	
int e2pfs_ecritical(void);

int e2pfs_lcritical(void);

#endif

