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

/*�������*/
#ifndef EOF
#define EOF					(-1)
#endif
#define ERR_ARG				(-2)					/* ��������*/
#define ERR_MEM				(-3)					/* �ռ䲻��*/
#define ERR_WRP				(-4)					/* ҳ���д�����߽�*/	
#define ERR_CRI				(-5)					/* �����ٽ���ʧ��*/
#define ERR_ECC				(-6)					/* У�����*/
#define ERR_EFD				(-7)					/* �����ļ�������*/
#define ERR_FOP				(-8)					/* �ļ��Ѿ��򿪣����ܶ�δ�*/
//#define ERR_FNO				(-8)					/* ������û�ж�Ӧ���ļ�*/

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
int e2pfs_fexist(int id);							/* �жϸ������ļ��Ƿ����*/ 
int e2pfs_ls(void);									/* ��ʾ�ļ���Ϣ��EEPROMʹ����Ϣ*/
int e2pfs_file_pgs(int id);						/* �����ļ�ʹ�õ�ҳ����*/

#endif
