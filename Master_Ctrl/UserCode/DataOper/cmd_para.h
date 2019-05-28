//命令解析  命令构建系列函数
#ifndef  _CMD_PARA_H__
#define  _CMD_PARA_H__

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

#define CMD_PARA_STRING_LEN_NAME  20
#define CMD_PARA_STRING_LEN_VALUE 2048

typedef struct{
	char name [CMD_PARA_STRING_LEN_NAME];					//数据名称
	char value[CMD_PARA_STRING_LEN_VALUE];					//数据值内容
    int item_begin_pos; //当前 参数的名字 在整个命令中的开始位置
	int name_str_len;
	int value_str_len;
}CmdParaOneItem;

typedef struct
{
    CmdParaOneItem items[10];
    int            item_cnt;
}CmdParaInfo;


/************************************************* 
*Return:      int 分解的名称参数数量
*DESCRIPTION: 分解命令到各个参数 元素中
*************************************************/
int cmd_para_get_all_items(
    __IN__ const char* cmd_str,
    __OUT_ CmdParaInfo* cmdReslut
    );
	
/************************************************* 
*Return:      int 参数名对应的ID号码 <0为未找到
*DESCRIPTION: 查找参数列表中，某一个名称的参数名所在位置
*************************************************/
int cmd_para_get_id_by_name(
    __IN__ const CmdParaInfo* cmdInfo ,
    __IN__ const char*  para_name
    );
	
	
/************************************************* 
*Return:      int 找到的int数据的数量
*DESCRIPTION: 解析值 字符串中 int 型数据 格式为 12,34,56 中间不能有空格等 分隔符只能是 ,号
*************************************************/
int cmd_para_get_int_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ int* pIntVals ,
    __IN__ int  nIntCntMax
    );
	
	
/************************************************* 
*Return:      int 找到的float数据的数量
*DESCRIPTION: 解析值 字符串中 float 型数据 分隔符只能是 ,号
*************************************************/
int cmd_para_get_float_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ float* pFloatVals ,
    __IN__ int    nValCntMax
    );

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个 字符串
*************************************************/
char* cmd_para_build_para_string_str(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ const char*  val
    );

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个int 数据 name=234\t
*************************************************/
char* cmd_para_build_para_string_int(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int    val
    );

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 多个int 数据 name=234,255\t
*************************************************/
char* cmd_para_build_para_string_ints(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int*    vals ,
    __IN__ int     val_cnt
    );

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个float 数据 name=234.2\t
*************************************************/
char* cmd_para_build_para_string_float(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float        val ,
    __IN__ int          point_cnt   //数据小数的数量
    );

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 多个float 数据 name=234.2,-33.2\t
*************************************************/
char* cmd_para_build_para_string_floats(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float*       vals ,
    __IN__ int          val_cnt ,
    __IN__ int          point_cnt   //数据小数的数量
    );

//////////////////////////////////////////////////////////////////////////

//char *int_to_str(signed int value, char *string, int radix);
//char* float_to_str(char *string,float val,int point_cnt);
    
//数据转移的一些宏定义
#define CMD_PARA_VAL_TRANS_2( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; }
#define CMD_PARA_VAL_TRANS_3( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; vals1[2]=vals2[2]; }
#define CMD_PARA_VAL_TRANS_4( vals1 , vals2 ) { vals1[0]=vals2[0] ; vals1[1]=vals2[1]; vals1[2]=vals2[2]; vals1[3]=vals2[3]; }

#ifdef __cplusplus
}
#endif

#endif
