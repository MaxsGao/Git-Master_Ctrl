//�������  �����ϵ�к���
#ifndef  _HEX_PARA_H__
#define  _HEX_PARA_H__

#ifdef __cplusplus
extern "C" {
#endif

#include<string.h>
#include<stdlib.h>

#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif

//#define CMD_PARA_STRING_LEN_NAME  20
//#define CMD_PARA_STRING_LEN_VALUE 40

//typedef struct{
//	char name [CMD_PARA_STRING_LEN_NAME];					//��������
//	char value[CMD_PARA_STRING_LEN_VALUE];					//����ֵ����
//    int item_begin_pos; //��ǰ ���������� �����������еĿ�ʼλ��
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
*Return:      int �ֽ�����Ʋ�������
*DESCRIPTION: �ֽ������������ Ԫ����
*************************************************/
//int cmd_para_get_all_items(
//    __IN__ const char* cmd_str,
//    __OUT_ CmdParaInfo* cmdReslut
//    );
	


#ifdef __cplusplus
}
#endif

#endif
