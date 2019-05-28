/******************************* (C) Embest ***********************************
* File Name          : e2pfs.c
* Author             : liren
* Date               : 2009-11-6
* Version            : 
* Description        : 
******************************************************************************/
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#include "e2pfs_port.h"
#include "e2pfs.h"

typedef struct {
	fid_t			i_id;							/* �ļ�������*/
	unsigned short	i_fpage;						/* �ļ����ݴ�ŵ�ʼҳ���*/	
	fsize_t			i_size;							/* �ļ���С*/
#if E2PFS_CDATE
	time_t			i_cdate;						/* �ļ���������*/
#endif
#if E2PFS_ADATE
	time_t			i_adate;
#endif
} Item_Info_t;

typedef struct {
	Item_Info_t		f_info;
	unsigned short  f_page;							/*��ǰoffset���ڵ�ҳ��*/
	unsigned short	f_offset;
} E2P_File;

#define DEBUG_ERR			(0)
#define DEBUG_WAN			(1)
#define DEBUG_MSG			(2)

#define FILE_ITEM_SIZE	(sizeof (Item_Info_t))					/* �ļ�Ŀ¼���С*/

/*ϵͳ����*/
#define E2PFS_PAGE_SIZE		(1<<E2PFS_PAGE_OFFS)				/* �ļ�ϵͳҳ���С*/
#define E2PFS_PAGE_MASK		((~(E2PFS_PAGE_SIZE-1)) & 0xffff)	/* ҳ���С����*/
	
/*ÿһҳ��ϵͳ���������ݴ�С = ҳ���С-ҳ��ECC��С*/
#define FSPAGE_USE_SIZE		(E2PFS_PAGE_SIZE - _ECC_SIZE)

/*�ļ�ϵͳҳ���������ʼҳ���ַ*/
#define FSMENG_B			0

/* �ļ�ϵͳҳ�������ռ��ҳ���� */
#if E2PFS_PAGES <= 0xff
#define FSMENG_ITEM_SIZE	1
#define FSMENG_PER_PAGE		FSPAGE_USE_SIZE
#elif E2PFS_PAGES <= 0xffff 
#define FSMENG_ITEM_SIZE	2
#define FSMENG_PER_PAGE		(FSPAGE_USE_SIZE>>1)
#endif
#define FSMENG_PAGES		((E2PFS_PAGES+FSMENG_PER_PAGE-1)/FSMENG_PER_PAGE)

/*һ��ҳ����Ŀ¼�����*/
#define ITEMS_PER_PAGE		(FSPAGE_USE_SIZE/FILE_ITEM_SIZE)

/* Ŀ¼������ʼҳ���ַ*/
#define FSITEM_B			(FSMENG_B+FSMENG_PAGES)
/*�ļ�ϵͳĿ¼ռ��ҳ����*/
#define FSITEM_PAGES		((E2PFS_FILES+ITEMS_PER_PAGE-1)/ITEMS_PER_PAGE) 
						

/*���ݴ����ʼҳ���ַ*/
#define FSDATA_B			(FSITEM_B+FSITEM_PAGES)
/*���ݴ����ҳ����*/
#define FSDATA_PAGES		(E2PFS_PAGES-FSITEM_PAGES-FSMENG_PAGES)

/*�ļ�idת����Ŀ¼��洢�߼���ַ*/
#define FSID2ADDR(x)		((FSITEM_B+((x)-1)/ITEMS_PER_PAGE)*E2PFS_PAGE_SIZE + \
								((x)-1)%ITEMS_PER_PAGE*FILE_ITEM_SIZE)

/*�߼�ҳ��ַ[0, E2PFS_PAGES)ת����ҳ����������ַ*/
#define PADDR2MADDR(x)		(((FSMENG_B+((x)/FSMENG_PER_PAGE))*E2PFS_PAGE_SIZE) + \
								((x)%FSMENG_PER_PAGE*FSMENG_ITEM_SIZE))

/*�߼�ҳ��ת����ҳ���ַ*/
#define PAGE2ADDR(x)		((x)*E2PFS_PAGE_SIZE)
/*ҳ�ڵ�ַת����ҳ��ַ*/
#define ADDR2PADDR(x)		((x)&E2PFS_PAGE_MASK)
/*ҳ��ַת����ҳ��*/
#define ADDR2PAGE(x)		((x)/E2PFS_PAGE_SIZE)
/*ȡҳ�ڵ�ַ��ҳ�ڵ�ƫ��*/
#define ADDR2OFFS(x)		((x)&(~E2PFS_PAGE_MASK))



static E2P_File sOFiles[E2PFS_OFILES];							/*�򿪵��ļ���Ϣ*/

#define FD2FILEP(x)			((x<0 || x>=E2PFS_OFILES) ? NULL : &sOFiles[x])

#if E2PFS_ECC
static struct _page_buf_t {
	int pg;
	unsigned char data[E2PFS_PAGE_SIZE];
}_dbuf = {
	-1
};
#endif

#if E2PFS_DEBUG
#ifndef WIN32
#define DEBUG_INFO(lv, x...)	do {												\
								if (E2PFS_DEBUG > (lv))  {							\
									printf("LEAVE[%d] : ",(lv));					\
									if ((lv) == DEBUG_ERR)							\
										printf("%s(%d) : ", __FILE__, __LINE__);	\
									printf(x...);									\
								}													\
							} while (0)
#else
static void DEBUG_INFO(int lv, const char * x, ...)
{
	va_list ap;

	if (E2PFS_DEBUG > (lv))  {	
		printf("LEAVE[%d] : ",(lv));				
		if ((lv) == DEBUG_ERR)						
			printf("%s(%d) : ", __FILE__, __LINE__);
		va_start(ap, x);
		vprintf(x, ap);
		va_end(ap);
	}
}
#endif
#else
#define	DEBUG_INFO(lv, x)
#endif

#if E2PFS_ECC
unsigned int _e2pfs_checkout(void *dat, int len);
#endif
/*=============================================================================
* Function		: _fs_page_read
* Description	: �ļ�ϵͳҳ���
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_page_read(int addr, void *dat, int len)
{
	if (ADDR2OFFS(addr) + len > FSPAGE_USE_SIZE) {

		len = FSPAGE_USE_SIZE - ADDR2OFFS(addr);
	}

#if E2PFS_ECC
{
	unsigned int sum = 0;

	if (_dbuf.pg != ADDR2PADDR(addr)) {
		_dbuf.pg = ADDR2PADDR(addr);
		
		e2prom_read(_dbuf.pg +  (E2PFS_ADDR_BEGIN)
			, _dbuf.data, E2PFS_PAGE_SIZE);
		memcpy(&sum, _dbuf.data + FSPAGE_USE_SIZE, _ECC_SIZE);
		if (sum != _e2pfs_checkout(_dbuf.data, FSPAGE_USE_SIZE)) {
			DEBUG_INFO(DEBUG_ERR, "У���\r\n");
			_dbuf.pg = -1;
			return ERR_ECC;
		}
	}

	memcpy(dat, _dbuf.data + (addr & ~E2PFS_PAGE_MASK), len);
}
#else
	len = e2prom_read((E2PFS_ADDR_BEGIN) + addr
		, dat, len);
#endif

	return len;
}
/*=============================================================================
* Function		: _fs_page_write
* Description	: �ļ�ϵͳҳ��д
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_page_write(int addr, void *dat, int len)
{
	if (ADDR2OFFS(addr) + len > FSPAGE_USE_SIZE) {
		len = FSPAGE_USE_SIZE - ADDR2OFFS(addr);
	}

#if E2PFS_ECC
{
	unsigned int sum;

	if (_dbuf.pg != ADDR2PADDR(addr)) {
		_dbuf.pg = ADDR2PADDR(addr);
		e2prom_read(_dbuf.pg + (E2PFS_ADDR_BEGIN)
			, _dbuf.data, E2PFS_PAGE_SIZE);
	}
	memcpy(_dbuf.data+ADDR2OFFS(addr), dat, len);
	sum = _e2pfs_checkout(_dbuf.data, FSPAGE_USE_SIZE);
	memcpy(_dbuf.data + FSPAGE_USE_SIZE, &sum, _ECC_SIZE);
	
	/* д����*/
	e2prom_write(_dbuf.pg + ADDR2OFFS(addr) + (E2PFS_ADDR_BEGIN)
		, _dbuf.data + ADDR2OFFS(addr), len);
	
	/*дУ��*/
	e2prom_write(_dbuf.pg + FSPAGE_USE_SIZE + (E2PFS_ADDR_BEGIN)
		, _dbuf.data + FSPAGE_USE_SIZE, _ECC_SIZE);	
}
#else
	len = e2prom_write(E2PFS_ADDR_BEGIN + addr
		,  dat, len);
#endif

	return len;
}

/*ҳ��ʹ�ñ�� 0��δ�ã�1������ϵͳʹ��, �������ļ���һҳ���*/
#define PAGE_UNUSE			0	
#define PAGE_RESEV			1
/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_get_page_state(int p)
{
	int f = 0;
	if (p<0 || p>E2PFS_PAGES) {
		DEBUG_INFO(DEBUG_ERR, "_fs_get_page_state : �������� p=%d\r\n", p);
		return ERR_ARG;
	}
	if (_fs_page_read(PADDR2MADDR(p), &f, FSMENG_ITEM_SIZE) < 0)
		return ERR_WRP;

	return f;
}
/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_find_unuse_page(void)
{
	int i;
	int f = PAGE_RESEV;

	for (i=FSDATA_B; i<FSDATA_B+FSDATA_PAGES && f!=PAGE_UNUSE; i++) {
		if (_fs_page_read(PADDR2MADDR(i), &f, FSMENG_ITEM_SIZE) < 0)
			return ERR_WRP;
	}
	--i;
	if (f != PAGE_UNUSE) {
		DEBUG_INFO(DEBUG_ERR, "û���ҵ�����ҳ��\r\n");
		return ERR_MEM;
	}

	return i;
}
/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_set_page_state(int p, int s)
{
	if (
		  !(
			(p >= 0 && p < FSDATA_B+FSDATA_PAGES) 
			&&  (p < FSDATA_B && s == PAGE_RESEV)
			|| ((s >= FSDATA_B && (s < FSDATA_B + FSDATA_PAGES)) || s == PAGE_UNUSE)
		  )
		)
	{
		DEBUG_INFO(DEBUG_ERR, "_fs_set_page_state:�������� : p=%d s=%d\r\n", p, s);
		return ERR_ARG;
	}
	if (_fs_page_write(PADDR2MADDR(p), &s, FSMENG_ITEM_SIZE) < 0)
		return ERR_WRP;
	return 0;
}
/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_read_item(int i, Item_Info_t *pit)
{
	if (_fs_page_read(FSID2ADDR(i), pit, FILE_ITEM_SIZE) < 0)
		return ERR_WRP;
	return 0;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _fs_write_item(int i, Item_Info_t *pit)
{
	if (i<=0 || i>E2PFS_FILES) {
		DEBUG_INFO(DEBUG_ERR, "_fs_write_item:�������� id = %d\r\n", i);
		return ERR_ARG;
	}
	if (_fs_page_write(FSID2ADDR(i), pit, FILE_ITEM_SIZE) < 0)
		return ERR_WRP;

	return 0;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _e2pfs_page_read(E2P_File *fp, unsigned char *buf, int len)
{
	int off, llen, rlen;

	assert(fp != NULL);
	assert(fp->f_info.i_id != 0);

	off = fp->f_offset % FSPAGE_USE_SIZE;
	llen = fp->f_info.i_size - fp->f_offset;

	if (llen == 0)
		return EOF;
	rlen = len<llen?len:llen;

	rlen = _fs_page_read(PAGE2ADDR(fp->f_page)+off, buf, rlen);
	if (rlen < 0) 
		return ERR_WRP;
	
	fp->f_offset += rlen;

	if (fp->f_offset % FSPAGE_USE_SIZE == 0)
		fp->f_page = _fs_get_page_state(fp->f_page);
	return rlen;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
static int _e2pfs_page_write(E2P_File *fp, unsigned char *buf, int len)
{
	int off, llen, wlen;
	
	assert(fp != NULL);
	assert(fp->f_info.i_id != 0);

	off = fp->f_offset % FSPAGE_USE_SIZE;
	llen = FSPAGE_USE_SIZE - off;
	if (off == 0 && fp->f_offset == fp->f_info.i_size) {
		/*�����ҳ*/
		int pg;
		pg = _fs_find_unuse_page();
		if (pg < 0) {
			return ERR_MEM;
		}
		if (fp->f_info.i_fpage == 0) 
			fp->f_info.i_fpage = pg;
		else
			_fs_set_page_state(fp->f_page, pg);

		_fs_set_page_state(pg, pg);
		fp->f_page = pg;
	}
	wlen = len<llen?len:llen;
	wlen = _fs_page_write(PAGE2ADDR(fp->f_page)+off, buf, wlen);
	if (wlen < 0) {
		DEBUG_INFO(DEBUG_ERR, "_fs_page_write\r\n");
		return wlen;
	}
	fp->f_offset += wlen;
	if (fp->f_offset > fp->f_info.i_size)
		fp->f_info.i_size = fp->f_offset;
	return wlen;
}


static int _e2pfs_get_fd(int fid)
{
	int i;
	int fd = ERR_EFD;

	for (i=0; i<E2PFS_OFILES; i++) {
		if (sOFiles[i].f_info.i_id == 0) {
			fd = i;
		}
		if (sOFiles[i].f_info.i_id == fid)
			/* �ļ��Ѵ�*/
			return ERR_FOP;	
	}
	/* û�п����ļ�������*/
	return fd;
}
static int _e2pfs_free_fd(int fd)
{
	memset(sOFiles+fd, 0, sizeof (sOFiles));
	return 0;
}
/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_format(void)
{
	int i;
	Item_Info_t fi;

	DEBUG_INFO(DEBUG_MSG, "      FSռ�ÿռ��С: %d(K)\r\n", 
									(E2PFS_PAGES*E2PFS_PAGE_SIZE)>>10);
	DEBUG_INFO(DEBUG_MSG, "          FSҳ���С: %d(B)\r\n", E2PFS_PAGE_SIZE);
	DEBUG_INFO(DEBUG_MSG, "            FSҳ����: %d\r\n", E2PFS_PAGES);
	DEBUG_INFO(DEBUG_MSG, "    ҳ�������ʼҳ��: %d\r\n", FSMENG_B);
	DEBUG_INFO(DEBUG_MSG, "    ÿһҳ������ҳ: %d\r\n", FSMENG_PER_PAGE);
	DEBUG_INFO(DEBUG_MSG, "    ҳ�������ҳ����: %d\r\n", FSMENG_PAGES);
	DEBUG_INFO(DEBUG_MSG, "        Ŀ¼��ʼҳ��: %d\r\n", FSITEM_B);
	DEBUG_INFO(DEBUG_MSG, "        Ŀ¼��ҳ����: %d\r\n", FSITEM_PAGES);
	DEBUG_INFO(DEBUG_MSG, "        ������ʼҳ��: %d\r\n", FSDATA_B);
	DEBUG_INFO(DEBUG_MSG, "        ������ҳ����: %d\r\n", FSDATA_PAGES);

	/* ��ʽ��Ŀ¼������ */
	memset(&fi, 0, FILE_ITEM_SIZE);
	for (i=1; i<=E2PFS_FILES; i++)
		_fs_write_item(i, &fi);
	
	/* ��ʽ��ҳ������� */
	for (i=0; i<E2PFS_PAGES; i++) {
		int f = PAGE_RESEV;
		if (i >= FSDATA_B && i < (FSDATA_B+FSDATA_PAGES)) 
			f = PAGE_UNUSE;
		_fs_set_page_state(i, f);
				
	}

	return 0;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_open(int fid/*, E2P_File *file*/)
{
	int ret;
	int fd;
	E2P_File *file;

//	assert(file != NULL);
	if (fid<=0 || fid>E2PFS_FILES) {
		DEBUG_INFO(DEBUG_ERR, "e2pfs_open:�������� id=%d\r\n", fid);
		return ERR_ARG;
	}
	fd = _e2pfs_get_fd(fid);
	if (fd < 0) {
		DEBUG_INFO(DEBUG_ERR, "û�п����ļ������� fid=%d\r\n", fid);
		return ERR_EFD;
	}
	file = FD2FILEP(fd);
	 
	ret = _fs_read_item(fid, &file->f_info);
	if (ret < 0) {
		return ret;
	}
	file->f_offset = 0;
	file->f_page = file->f_info.i_fpage;
	if (file->f_info.i_id != 0) {
		DEBUG_INFO(DEBUG_MSG, "���ļ�id : %d\r\n", fid);
		return fd;
	}
	/*�����ļ�*/
	file->f_info.i_id    = fid;
	file->f_info.i_fpage = 0;
	file->f_info.i_size  = 0;
#if E2PFS_CDATE
	file->f_info.i_cdate  = time(NULL);
#endif
#if E2PFS_ADATE
	file->f_info.i_adate  = time(NULL);
#endif
	/**/
	file->f_page = 0;

	DEBUG_INFO(DEBUG_MSG, "�����ļ�id : %d\r\n", fid);

	return fd;
}



/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_read(int fd/*E2P_File *fp*/, void *buf, int len)
{
	unsigned char *_buf = buf;
	int rlen, tlen = 0;
	E2P_File *fp = FD2FILEP(fd);

	assert(buf != NULL);

	if (fp == NULL || fp->f_info.i_id == 0) {
		DEBUG_INFO(DEBUG_ERR, "������[%d]û�ж�Ӧ�򿪵��ļ�\r\n", fd);
		return ERR_EFD;
	}
r:	
	rlen = _e2pfs_page_read(fp, _buf+tlen, len-tlen);
	if (rlen < 0) {
		if (tlen)
			return tlen;
		return EOF;
	}
	tlen += rlen;
	if (tlen < len)
		goto r;
	return tlen;
}


/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_write(int fd/*E2P_File *fp*/, void *buf, int len)
{
	unsigned char *_buf = buf;
	int wlen, tlen = 0;
	E2P_File *fp = FD2FILEP(fd);
	
	assert(_buf != NULL);
	
	if (fp == NULL || fp->f_info.i_id == 0) {
		DEBUG_INFO(DEBUG_ERR, "������[%d]û�ж�Ӧ�򿪵��ļ�\r\n", fd);
		return ERR_EFD;
	}

w:
	wlen = _e2pfs_page_write(fp, _buf+tlen, len-tlen);
	if (wlen < 0) {
		if (tlen > 0)
			return tlen;

		DEBUG_INFO(DEBUG_ERR, "_fs_page_write : ���ܳ���eeprom��ַ�ռ�\r\n");
		return wlen;
	}
	tlen += wlen;
	if (tlen < len)
		goto w;
	return tlen;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_lseek(int fd, /*E2P_File *fp,*/ int offset, int which)
{
	E2P_File *fp = FD2FILEP(fd);
	long cur   = fp->f_offset;	
	long fsize = fp->f_info.i_size;
	unsigned short  npg;
	int	 noff;
	
	if (fp == NULL || fp->f_info.i_id == 0) {
		DEBUG_INFO(DEBUG_ERR, "������[%d]û�ж�Ӧ�򿪵��ļ�\r\n", fd);
		return ERR_EFD;
	}

	switch (which) {
	case SEEK_SET:
		noff = offset;
		break;
	case SEEK_CUR:
		noff = cur + offset;
		break;
	case SEEK_END:
		noff = fsize + offset;
		break;
	default:
		return ERR_ARG;
	}
	if (noff < 0 || noff > fsize)
		return ERR_ARG;
	npg = fp->f_page;
	if (noff<cur) {
		npg = fp->f_info.i_fpage;
		cur = 0;
	} else {
		cur = cur / FSPAGE_USE_SIZE * FSPAGE_USE_SIZE; 
	}
	while (cur + FSPAGE_USE_SIZE <= noff) {
		npg = _fs_get_page_state(npg);
		cur += FSPAGE_USE_SIZE;
	}
	fp->f_page = npg;
	fp->f_offset = noff;
	return noff;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_close(int fd/*E2P_File *fp*/)
{
	E2P_File *fp = FD2FILEP(fd);
	
	if (fp == NULL || fp->f_info.i_id == 0) {
		DEBUG_INFO(DEBUG_ERR, "������[%d]û�ж�Ӧ�򿪵��ļ�\r\n", fd);
		return ERR_EFD;
	}
//	assert(fp != NULL);
#if E2PFS_ADATE
	fp->f_info.i_adate  = time(NULL);
#endif
	_fs_write_item(fp->f_info.i_id, &fp->f_info);
	
	DEBUG_INFO(DEBUG_MSG, "�ر��ļ�id : %d\r\n", fp->f_info.i_id);

	/* �ͷ��ļ�������*/
	_e2pfs_free_fd(fd);

	return 0;
}

/*=============================================================================
* Function		: 
* Description	: 
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_remove(int fid)
{
	Item_Info_t it;
	int pg, np;

	if (fid<=0 || fid>E2PFS_FILES) {
		DEBUG_INFO(DEBUG_ERR, "e2pfs_remove:�������� id=%d\r\n", fid);
		return ERR_ARG;
	}

	_fs_read_item(fid, &it);
	if (it.i_fpage == 0)
		goto rm;
	/*ɾ���ļ�ʹ�õ�ҳ��*/
	pg = it.i_fpage;
	np = _fs_get_page_state(pg);
	while (pg != np) {
		_fs_set_page_state(pg, PAGE_UNUSE);
		pg = np;
		np = _fs_get_page_state(pg);
	}
	_fs_set_page_state(pg, PAGE_UNUSE);
rm:
	memset(&it, 0, FILE_ITEM_SIZE);
	_fs_write_item(fid, &it);
	return 0;
}

#if E2PFS_EFUN

/*=============================================================================
* Function		: e2pfs_ls 
* Description	: ��ʾ�ļ���Ϣ��EEPROMʹ����Ϣ
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_fsinfo(void)
{
	printf("         E2PFS �����\r\n");
	printf("=====================================\r\n");
	printf("FSEEPROM�ܿռ�: %-8d(K)\r\n", (E2PFS_PAGES*E2PFS_PAGE_SIZE)>>10);
	printf("ҳ�������ռ��: %-8d(B)  %5.1f %%\r\n", FSMENG_PAGES*FSPAGE_USE_SIZE,
		FSMENG_PAGES*FSPAGE_USE_SIZE*100.0/((E2PFS_PAGES*E2PFS_PAGE_SIZE)));
	printf("Ŀ¼������ռ��: %-8d(B)  %5.1f %%\r\n", FSITEM_PAGES*FSPAGE_USE_SIZE,
		FSITEM_PAGES*FSPAGE_USE_SIZE*100.0/((E2PFS_PAGES*E2PFS_PAGE_SIZE)));
	printf("       ECCռ��: %-8d(B)  %5.1f %%\r\n", _ECC_SIZE*E2PFS_PAGES,
		_ECC_SIZE*E2PFS_PAGES*100.0/((E2PFS_PAGES*E2PFS_PAGE_SIZE)));
	printf("  EEPROM������: %-8c     %5.1f %%\r\n", ' ', 
		(FSDATA_PAGES*FSPAGE_USE_SIZE*100.0)/(E2PFS_PAGES*E2PFS_PAGE_SIZE));
	
	return 0;
}

/*=============================================================================
* Function		: e2pfs_file_pgs
* Description	: �����ļ�ʹ�õ�ҳ����
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_file_pgs(int id)
{
	int bpg, pg, num = 1;
	Item_Info_t it;

	if (id<=0 || id>E2PFS_FILES) {
		DEBUG_INFO(DEBUG_ERR, "e2pfs_remove:�������� id=%d\r\n", id);
		return ERR_ARG;
	}

	_fs_read_item(id, &it);
	bpg = it.i_fpage;

	if (it.i_id == 0 || bpg == 0)
		return 0;

	pg = _fs_get_page_state(bpg);
	while (pg != bpg) {
		num++;
		bpg = pg;
		pg = _fs_get_page_state(pg);
	}
	return num;
}

/*=============================================================================
* Function		: e2pfs_fexist
* Description	: �жϸ������ļ��Ƿ����
* Input Para	: 
* Output Para	: 
* Return Value  : 0�������ڣ������������ļ���Ϊid���ļ�
=============================================================================*/
int e2pfs_fexist(int id)
{
	Item_Info_t it;
	if (id<=0 || id>E2PFS_FILES) {
		DEBUG_INFO(DEBUG_ERR, "e2pfs_fexist:�������� id=%d\r\n", id);
		return ERR_ARG;
	}

	if (_fs_read_item(id, &it) < 0)
		return ERR_WRP;
	return it.i_id;
}

/*=============================================================================
* Function		: e2pfs_ls 
* Description	: ��ʾ�ļ���Ϣ��EEPROMʹ����Ϣ
* Input Para	: 
* Output Para	: 
* Return Value  : 
=============================================================================*/
int e2pfs_ls(void)
{
	fid_t			i; 
	int				pgs;
	Item_Info_t		it;
	unsigned int	tbyte;
	unsigned int	ubyte = 0;
#if E2PFS_CDATE || E2PFS_ADATE
	time_t			tm = 0;
	char			data[16];
#endif

	printf("�ļ�id  ");//��������          �� ��   ��ʼҳ��  ҳ����  �ٷֱ�\r\n");
#if E2PFS_CDATE
	printf("��������        ");
#endif
#if E2PFS_ADATE
	printf("����������    ");
#endif
	printf("�� ��   ��ʼҳ��  ҳ����  �ٷֱ�\r\n");
	printf("=========================================");
#if E2PFS_CDATE
	printf("================");
#endif
#if E2PFS_ADATE
	printf("================");
#endif
	printf("\r\n");
	for (i=1; i<=E2PFS_FILES; i++) {
		_fs_read_item(i, &it);
		if (it.i_id == 0)
			continue;
		if (it.i_id != i) { 
			DEBUG_INFO(DEBUG_WAN, "Item �����ݴ��� fid = %d\r\n", i);
			continue;
		}
		ubyte += it.i_size;
		pgs = e2pfs_file_pgs(i);

		printf("%-6u  ", it.i_id);
#if E2PFS_CDATE
		tm = it.i_cdate;
		strncpy(data, &(ctime(&tm)[11]), 13);
		data[13] = 0;
		printf("%-16s", data);
#endif
#if E2PFS_ADATE
		tm = it.i_adate;
		strncpy(data, &(ctime(&tm)[11]), 13);
		data[13] = 0;
		printf("%-16s", data);
#endif
		printf("%-6u  %-8u  %-6u  %-6.1f%%\r\n",
			it.i_size, it.i_fpage, pgs, (100.0*pgs)/FSDATA_PAGES);
	}
	
	tbyte = FSDATA_PAGES*FSPAGE_USE_SIZE;
	
	printf("=========================================");
#if E2PFS_CDATE
	printf("================");
#endif
#if E2PFS_ADATE
	printf("================");
#endif
	printf("\r\n");
	printf("����: %d(B) ����: %d(B) ʣ��: %d(B) ʹ����: %.1f %%\r\n", 
		tbyte, ubyte, tbyte-ubyte, 100.0*ubyte/tbyte);
		
	return 0;
}

#endif
