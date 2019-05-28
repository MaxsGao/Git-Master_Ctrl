//命令解析  命令构建系列函数
#ifndef  _HEX_PARA_H__
#define  _HEX_PARA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include<string.h>
#include<stdlib.h>

#ifndef  __IN__
#define  __IN__      //参数作为输入
#endif
#ifndef  __OUT_
#define  __OUT_      //参数作为输出
#endif

//#define CMD_PARA_STRING_LEN_NAME  20
//#define CMD_PARA_STRING_LEN_VALUE 40

//typedef struct{
//	char name [CMD_PARA_STRING_LEN_NAME];					//数据名称
//	char value[CMD_PARA_STRING_LEN_VALUE];					//数据值内容
//    int item_begin_pos; //当前 参数的名字 在整个命令中的开始位置
//	int name_str_len;
//	int value_str_len;
//}CmdParaOneItem;

//typedef struct
//{
//    CmdParaOneItem items[30];
//    int            item_cnt;
//}CmdParaInfo;

#define HEX_PARA_ORDER_LEN_DATAS 40

typedef struct{
	unsigned char moudle_ID;
	unsigned char type;
	unsigned char data[HEX_PARA_ORDER_LEN_DATAS];
}HexParaInfo;

/************************************************* 
*Return:      int 分解的名称参数数量
*DESCRIPTION: 分解命令到各个参数 元素中
*************************************************/
//int cmd_para_get_all_items(
//    __IN__ const char* cmd_str,
//    __OUT_ CmdParaInfo* cmdReslut
//    );
	


#ifdef __cplusplus
}
#endif

#endif
