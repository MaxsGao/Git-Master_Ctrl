/******************************* (C) Embest ***********************************
* File Name          : e2pfs_port.h
* Author             : liren
* Date               : 2009-11-6
* Version            : 
* Description        : 
******************************************************************************/

#ifndef __E2PFS_PORT_H__
#define __E2PFS_PORT_H__

/* ������Ϣ֧�� 0���ص�������� 1��������Ϣ 2�������뾯�� 3��������Ϣ*/
#define E2PFS_DEBUG			1						

#define E2PFS_EFUN			1						/* һЩ��������֧��,����ʱ����*/
  
#define	E2PFS_ECC			1						/* 0����֧�� 1��У��֧�֣���ż��*/

/* ���֧��У�飬_ECC_SIZEΪУ���볤�� ���ܳ���4��B��*/
#define _ECC_SIZE			(E2PFS_ECC ? 1 : 0)	

#define E2PFS_FILES			5						/* E2PFS֧�ִ���������ļ��� */

#define E2PFS_OFILES		2						/* E2PFS֧�ִ�����ļ���<=E2PFS_FILES*/

//#define E2PFS_FNAME			5						/*�ļ������ȣ�û�б���ĩβ'\0'*/

#define E2PFS_CDATE			0						/* �ļ�����ʱ��֧��*/

#define E2PFS_ADATE			1						/* �ļ�������ʱ��֧��*/

/* EEPROM ���ѡ�� */
#define E2PFS_ADDR_BEGIN	0						/* EEPROM�л��ָ��ļ�ϵͳ����ʼ�����ַ*/

#define E2PFS_PAGE_OFFS		4						/* �ļ�ϵͳҳ���ַƫ��,���ܳ���15*/

#define E2PFS_PAGES			64						/* �ļ�ϵͳʹ�õ�ҳ����*/

typedef unsigned short		fsize_t;				/* ϵͳ֧�ֵ�����ļ�����*/

typedef unsigned char		fid_t;					/* �ļ�����������*/

extern int e2prom_read(int addr, void *_buf, int len);		/* EEPROM�ײ����������*/

extern int e2prom_write(int addr, void *_buf, int len);		/* EEPROM�ײ����д����*/
	
int e2pfs_ecritical(void);

int e2pfs_lcritical(void);

#endif

