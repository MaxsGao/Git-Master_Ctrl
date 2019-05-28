/******************************* (C) Embest ***********************************
* File Name          : e2pfs_port.c
* Author             : liren
* Date               : 2009-11-6
* Version            : 
* Description        : 
******************************************************************************/
#include "e2pfs_port.h"

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_ecritical(void)
{
	return 0;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_lcritical(void)
{
	return 0;
}

	
#if E2PFS_ECC
#define		_ECC_MAGIC		'X'
/*=============================================================================
* Function		: 
* Description	: 自定义校验函数
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
unsigned int _e2pfs_checkout(void *dat, int len)
{
	int i;
	unsigned char *p = dat, v = _ECC_MAGIC;
	
	for (i=0; i<len; i++) 
		v ^= p[i];
	return v;
}
#endif

/*****************************************************************************
	
				底层EEPROM接口

 *****************************************************************************/
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#endif
#define USERAM	1

#define E2ROM_CAPACITY	(1*1024) 

#if USERAM
unsigned char g_e2rom[E2ROM_CAPACITY];
#else
FILE *fp = NULL;

int setFileLength(FILE* file, unsigned int len)
{
	int fd;
#ifdef _WIN32
	HANDLE hfile;

	fseek(file, len, SEEK_SET);
	fd = _fileno(file);
	hfile = (HANDLE)_get_osfhandle(fd);
	SetEndOfFile(hfile);
	fseek(file, 0, SEEK_SET);
	return 0;
#else
	fd = fileno(file);
	return ftruncate(fd, len);
#endif

}
static void ee_open(void)
{
	if (fp != NULL)
		return;
	fp = fopen("eeprom.img", "a+");
	if (fp == NULL)
		exit(1);
	setFileLength(fp, E2ROM_CAPACITY);
	return;
}
static void ee_close(void)
{
	if (fp == NULL)
		return;
	fclose(fp);
	fp = NULL;
}
#endif

int e2prom_read(int addr, void *_buf, int len)
{

#if USERAM
	unsigned char *prdat = g_e2rom + addr;
#endif
	unsigned char *buf = (unsigned char *)_buf;
	
	if (addr < 0 || (addr + len > E2ROM_CAPACITY))
		return -1;
#if USERAM
	memcpy(buf, prdat, len);
#else
	ee_open();
	fseek(fp, addr, SEEK_SET);
	fread(buf, len, 1, fp);
#endif
	return len;
}

int e2prom_write(int addr, void *_buf, int len)
{
	
#if USERAM
	unsigned char *pwdat = g_e2rom + addr;
#endif
	unsigned char *buf = (unsigned char *)_buf;
	
	if (addr < 0 || (addr + len > E2ROM_CAPACITY))
		return -1;
#if USERAM
	memcpy(pwdat, buf, len);
#else
	ee_open();
	fseek(fp, addr, SEEK_SET);
	fwrite(buf, len, 1, fp);
	ee_close();
#endif
	return len;
}