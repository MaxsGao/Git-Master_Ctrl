//�������  �����ϵ�к���
#ifndef  _CMD_PARA_H__
#define  _CMD_PARA_H__

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

#define CMD_PARA_STRING_LEN_NAME  20
#define CMD_PARA_STRING_LEN_VALUE 2048

typedef struct{
	char name [CMD_PARA_STRING_LEN_NAME];					//��������
	char value[CMD_PARA_STRING_LEN_VALUE];					//����ֵ����
    int item_begin_pos; //��ǰ ���������� �����������еĿ�ʼλ��
	int name_str_len;
	int value_str_len;
}CmdParaOneItem;

typedef struct
{
    CmdParaOneItem items[10];
    int            item_cnt;
}CmdParaInfo;


/************************************************* 
*Return:      int �ֽ�����Ʋ�������
*DESCRIPTION: �ֽ������������ Ԫ����
*************************************************/
int cmd_para_get_all_items(
    __IN__ const char* cmd_str,
    __OUT_ CmdParaInfo* cmdReslut
    );
	
/************************************************* 
*Return:      int ��������Ӧ��ID���� <0Ϊδ�ҵ�
*DESCRIPTION: ���Ҳ����б��У�ĳһ�����ƵĲ���������λ��
*************************************************/
int cmd_para_get_id_by_name(
    __IN__ const CmdParaInfo* cmdInfo ,
    __IN__ const char*  para_name
    );
	
	
/************************************************* 
*Return:      int �ҵ���int���ݵ�����
*DESCRIPTION: ����ֵ �ַ����� int ������ ��ʽΪ 12,34,56 �м䲻���пո�� �ָ���ֻ���� ,��
*************************************************/
int cmd_para_get_int_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ int* pIntVals ,
    __IN__ int  nIntCntMax
    );
	
	
/************************************************* 
*Return:      int �ҵ���float���ݵ�����
*DESCRIPTION: ����ֵ �ַ����� float ������ �ָ���ֻ���� ,��
*************************************************/
int cmd_para_get_float_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ float* pFloatVals ,
    __IN__ int    nValCntMax
    );

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1�� �ַ���
*************************************************/
char* cmd_para_build_para_string_str(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ const char*  val
    );

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1��int ���� name=234\t
*************************************************/
char* cmd_para_build_para_string_int(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int    val
    );

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ ���int ���� name=234,255\t
*************************************************/
char* cmd_para_build_para_string_ints(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int*    vals ,
    __IN__ int     val_cnt
    );

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1��float ���� name=234.2\t
*************************************************/
char* cmd_para_build_para_string_float(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float        val ,
    __IN__ int          point_cnt   //����С��������
    );

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ ���float ���� name=234.2,-33.2\t
*************************************************/
char* cmd_para_build_para_string_floats(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float*       vals ,
    __IN__ int          val_cnt ,
    __IN__ int          point_cnt   //����С��������
    );

//////////////////////////////////////////////////////////////////////////

//char *int_to_str(signed int value, char *string, int radix);
//char* float_to_str(char *string,float val,int point_cnt);
    
//����ת�Ƶ�һЩ�궨��
#define CMD_PARA_VAL_TRANS_2( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; }
#define CMD_PARA_VAL_TRANS_3( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; vals1[2]=vals2[2]; }
#define CMD_PARA_VAL_TRANS_4( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; vals1[2]=vals2[2]; vals1[3]=vals2[3]; }

#ifdef __cplusplus
}
#endif

#endif
