#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "e2pfs.h"

struct cmd {
	char *cmdName;
	int (*run) (int argc, char **argv);
	char *help;
};

static int command_help(int argc, char **argv);
static int command_format(int argc, char *argv[]);
static int command_touch(int argc, char *argv[]);
static int command_rm(int argc, char *argv[]);
static int command_ls(int argc, char *argv[]);
static int command_more(int argc, char *argv[]);
static int command_od(int argc, char *argv[]);
static int command_fsinfo(int argc, char *argv[]);


static int command_write(int argc, char *argv[]);

static struct cmd cmds[] = {
    {"help",    command_help,	"Show this messages."},
	{"touch",	command_touch,	"Create file."},
	{"format",  command_format, "Format file system."},
	{"rm",		command_rm,		"Delete file."},
	{"ls",		command_ls,		"list file."},
	{"more",	command_more,	"display file."},
	{"od",		command_od,		"dump file."},
	{"fsinfo",	command_fsinfo, "file system infomation."},

	{"write",	command_write,  "write file."},
	{NULL,  NULL,   NULL},
};

static int command_help(int argc, char **argv)
{
	struct cmd *pcmd = cmds;

    printf("\tname        help");
    printf("\r\n\t----------  --------------------");
	for (; pcmd->cmdName != NULL; pcmd++) {
		printf("\r\n\t%-10s  %s", pcmd->cmdName, pcmd->help);
	}
	printf("\r\n");
	return 0;
}
static int command_format(int argc, char *argv[])
{
	e2pfs_format();
	return 0;
}
static int command_touch(int argc, char *argv[])
{
	int id;
	int fd;
	int ret;

	if (argc != 2)
		goto help;
	id = atoi(argv[1]);
	ret = e2pfs_fexist(id);
	if (ret < 0) {
		return 0;
	}

	if (ret > 0) {
		printf("文件id [%d] 已存在\r\n", id);
		return 0;
	}
	if ((fd = e2pfs_open(id)) < 0) {
		printf("创建文件失败 fid=%d\r\n", id);
		return 0;
	}
	e2pfs_close(fd);

	return 0;
help:
	printf("usage:\r\n");
	printf("\ttouch <fid>\r\n");

	return 0;
}
static int command_rm(int argc, char *argv[])
{
	int id;

	if (argc != 2)
		goto help;
	id = atoi(argv[1]);

	if (e2pfs_fexist(id) == 0) {
		printf("文件id [%d] 不存在\r\n", id);
		return 0;
	}
	e2pfs_remove(id);

	return 0;
help:
	printf("usage:\r\n");
	printf("\rm <fid>\r\n");
	
	return 0;
}
static int command_ls(int argc, char *argv[])
{
	e2pfs_ls();
	return 0;
}

static int command_more(int argc, char *argv[])
{
	int id;
	int fd;
	char dat[64];
	int rlen;

	if (argc != 2)
		goto help;

	id = atoi(argv[1]);

	if (e2pfs_fexist(id) == 0) {
		printf("文件id [%d] 不存在\r\n", id);
		return 0;
	}
	fd = e2pfs_open(id);
	if (fd < 0) {
		printf("打开文件 [%d] 错误\r\n", id);
		return -1;
	}
	while ((rlen = e2pfs_read(fd, &dat, 64)) > 0) {
		int i;
		for (i=0; i<rlen; i++)
			printf("%c", dat[i]);
	}
	printf("\r\n");
	e2pfs_close(fd);

	return 0;

help:
	printf("usage:\r\n");
	printf("more <fid>\r\n");
	return 0;
}

static int command_od(int argc, char *argv[])
{
	int id;
	int fd;
	char dat[64];
	int rlen;

	if (argc != 2)
		goto help;

	id = atoi(argv[1]);

	if (e2pfs_fexist(id) == 0) {
		printf("文件id [%d] 不存在\r\n", id);
		return 0;
	}
	fd = e2pfs_open(id);
	if (fd < 0) {
		printf("打开文件 [%d] 错误\r\n", id);
		return -1;
	}
	while ((rlen = e2pfs_read(fd, &dat, 64)) > 0) {
		int i;
		for (i=0; i<rlen; i++)
			printf("%02X ", dat[i]&0xff);
	}
	printf("\r\n");
	e2pfs_close(fd);

	return 0;

help:
	printf("usage:\r\n");
	printf("od <fid>\r\n");
	
	return 0;
}

static int command_fsinfo(int argc, char *argv[])
{
	e2pfs_fsinfo();
	return 0;
}


static int command_write(int argc, char *argv[])
{
	int id;
	int fd;
	int wlen;

	if (argc != 3)
		goto help;

	id = atoi(argv[1]);

	if (e2pfs_fexist(id) == 0) {
		printf("文件id [%d] 不存在\r\n", id);
		return 0;
	}
	fd = e2pfs_open(id);
	if (fd < 0) {
		printf("打开文件 [%d] 错误\r\n", id);
		return -1;
	}
	wlen = e2pfs_write(fd, argv[2], strlen(argv[2]));
	printf("文件[%d]写入%d字节\r\n", id, wlen);
	e2pfs_close(fd);
	
	return 0;

help:
	printf("usage:\r\n");
	printf("write <fid> <datas>\r\n");

	return 0;
}

// 分解字符串为多个子串
int make_argv(char* s, int size, char *argv[], int av_max) {
	char* se;
	int n;

	n = 0;
	se = s + size - 1;
	while (s < se && *s && isspace(*s)) {
		s++;
	}

	while (s < se && *s && n < av_max - 1) {
		if (*s) {
			argv[n++] = s;
		}

		while (s < se && *s && ! isspace(*s)) {
			s++;
		}

		if (s < se && *s) {
			*s++ = '\0';
		}

		while (s < se && *s && isspace(*s)) {
			s++;
		}
	}

	argv[n] = NULL;

	return n;
}
#define ARGS_MAX_COUNT 16
static char _bufline[81];
static char *argv[ARGS_MAX_COUNT];
int main(void)
{
    int argc;
    struct cmd *pcmd;

    while (1) {             
        printf("liren@E2PFS> ");                 
        gets(_bufline);
        argc = make_argv(_bufline, 80, argv, ARGS_MAX_COUNT);
        if (argc <= 0)
            continue;

        pcmd = cmds;
		while (pcmd->cmdName != NULL) {
			if (!strcmp(argv[0], pcmd->cmdName)) {
				pcmd->run(argc, argv);
				break;
			}
			pcmd++;
		}
        if (pcmd->cmdName == NULL)
            printf("No cmmand name is : %s\r\n", argv[0]);
    }

	return 0;
}