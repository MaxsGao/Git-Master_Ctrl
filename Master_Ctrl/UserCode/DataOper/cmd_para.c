/**************************************************************
COPYRIGHT (C), BOLIGTHED Co., Ltd.  
*Filename:  		cmd_para.c
*Version:			V
*Data:			2016.11.30
*Description:  	通道管理和收发数据处理的文件
***************************************************************/
#include "cmd_para.h"

//拷贝字符串，并返回结束位置的 指针
///return: 源字符串的尾，也就是新拷贝的字符串的头
static char* box_str_cpy_rt_pos(char* pOut,const char* pIn)
{
    while( *pIn )
    {
        *pOut = *pIn ;
        pOut++;
        pIn++;
    }
    *pOut = 0 ;
    return pOut;
}

#define CMD_PARA_IS_CHAR_DIVIER(cur_ch)  ( ('='==(cur_ch)) || (' '==(cur_ch)) || ('\t'==(cur_ch)) || (':'==(cur_ch)) )
#define CMD_PARA_IS_CHAR_END_CMD(cur_ch)  (('\r'==(cur_ch)) || ('\n'==(cur_ch)) || ('\0'==(cur_ch)))

//需要当前字符串的 结束符位置
//结束符为 ' ' '\t' '\r' '\n' '=' '\0'
const char* cmd_para_get_cur_str_end(const char* pstr)
{
    char cur_ch ;
    while(1)
    {
        cur_ch = *pstr ;
        if( CMD_PARA_IS_CHAR_DIVIER(cur_ch) || CMD_PARA_IS_CHAR_END_CMD(cur_ch) )
        {
            return pstr;
        }
        pstr++;
    }
}

//查找下一个 名称或者 值的开始位置 数字，字母，-,+
const char* cmd_para_get_next_str_begin(const char* pstr)
{
    char cur_ch ;
    while(1)
    {
        cur_ch = *pstr ;
        //如果字符串已经结束了
        if ( CMD_PARA_IS_CHAR_END_CMD(cur_ch) )
        {
            return pstr;
        }

        if( CMD_PARA_IS_CHAR_DIVIER(cur_ch) )
        {
            pstr++;
        }
        else
        {
            return pstr;
        }
    }
}

/************************************************* 
*Return:      int 分解的名称参数数量
*DESCRIPTION: 分解命令到各个参数 元素中
*************************************************/
int cmd_para_get_all_items(
    __IN__ const char* cmd_str,
    __OUT_ CmdParaInfo* cmdReslut
    )
{
    const char* str = cmd_str;
    const char* str_name_begin;
    CmdParaOneItem* pitem = cmdReslut->items;
    cmdReslut->item_cnt = 0;
    str_name_begin = str;
    while( 1 )
    {
        pitem->item_begin_pos = str_name_begin - cmd_str; //开始位置
        //先寻找分隔符
        str = cmd_para_get_cur_str_end(str_name_begin); //查找分隔符
        if ( *str == '=' ) //如果分隔符为 ‘=’ 字符串正常
        {
            pitem->name_str_len = str - str_name_begin;
            if ( (pitem->name_str_len == 0) || 
                 (pitem->name_str_len >= sizeof(pitem->name)) )
            { //名称长度异常 返回0
                cmdReslut->item_cnt = 0;
                return 0;
            }
            memcpy( pitem->name , str_name_begin , pitem->name_str_len );
            pitem->name[pitem->name_str_len] = 0;

            //查找值的开始位置
            str_name_begin = cmd_para_get_next_str_begin(str);
            if ( CMD_PARA_IS_CHAR_END_CMD(*str_name_begin) )
            {
                cmdReslut->item_cnt = 0;
                return 0;
            }
            str = cmd_para_get_cur_str_end(str_name_begin); //查找分隔符
            pitem->value_str_len = str - str_name_begin;
            if ( (pitem->value_str_len >= sizeof(pitem->value)) )
            { //名称长度异常 返回0
                cmdReslut->item_cnt = 0;
                return 0;
            }
            memcpy( pitem->value , str_name_begin , pitem->value_str_len );
            pitem->value[pitem->value_str_len] = 0;

            pitem++;
            cmdReslut->item_cnt++;

            if ( cmdReslut->item_cnt >= ( sizeof(cmdReslut->items)/sizeof(cmdReslut->items[0]) ) )
            {
                return cmdReslut->item_cnt;
            }
            //找到下一条的开始位置
            str_name_begin = cmd_para_get_next_str_begin(str);
            if ( CMD_PARA_IS_CHAR_END_CMD(*str_name_begin) ) //已经到结束位置了 直接返回
            {
                return cmdReslut->item_cnt;
            }
        }
        else
        {
            cmdReslut->item_cnt = 0;
            return 0;
        }
    }
    //return cmdReslut->item_cnt;
}

/************************************************* 
*Return:      int 参数名对应的ID号码 <0为未找到
*DESCRIPTION: 查找参数列表中，某一个名称的参数名所在位置
*************************************************/
int cmd_para_get_id_by_name(
    __IN__ const CmdParaInfo* cmdInfo ,
    __IN__ const char*  para_name
    )
{
    int i;
    int name_len = strlen(para_name);
    if ( (cmdInfo->item_cnt<=0) || ( cmdInfo->item_cnt > ( sizeof(cmdInfo->items)/sizeof(cmdInfo->items[0])) ) )
    {
        return -1;
    }
    for( i=0 ; i<cmdInfo->item_cnt ; i++ )
    {
        if ( cmdInfo->items[i].name_str_len == name_len ) //名称字符串长度相同
        {
            if ( 0 == memcmp(cmdInfo->items[i].name , para_name , name_len) ) //直接使用memcmp函数，效率高
            {
                return i;
            }
        }
    }
    return -1;
}

#define CMD_PARA_IS_STRING_NUMBER(ch) ( (((ch)<='9')&&((ch)>='0')) || ((ch)=='-') || ((ch)=='+') )
#define CMD_PARA_IS_STRING_FLOAT(ch) ( (((ch)<='9')&&((ch)>='0')) || ((ch)=='-') || ((ch)=='+') || ((ch)=='.') )

/************************************************* 
*Return:      int 找到的int数据的数量
*DESCRIPTION: 解析值 字符串中 int 型数据 格式为 12,34,56 中间不能有空格等 分隔符只能是 ,号
*************************************************/
int cmd_para_get_int_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ int* pIntVals ,
    __IN__ int  nIntCntMax
    )
{
    int nIntCnt = 0;
    char ch_cur = *para_val;
    while( CMD_PARA_IS_STRING_NUMBER(ch_cur) && (nIntCnt<nIntCntMax) )
    {
        *pIntVals = atoi(para_val);

        pIntVals++;
        nIntCnt++;

        //查找分隔符，并且查找下一个数字的开始位置
        do 
        {
            para_val++;
            ch_cur = *para_val;
        } while ( CMD_PARA_IS_STRING_NUMBER(ch_cur) );
        if ( ch_cur != ',' ) //如果数字间分隔符不是 , 号，表示结束了
        {
            return nIntCnt;
        }
        //定位到下一个数字的开始位置
        para_val++;
        ch_cur = *para_val;
    }
    return nIntCnt;
}

/************************************************* 
*Return:      int 找到的float数据的数量
*DESCRIPTION: 解析值 字符串中 float 型数据 分隔符只能是 ,号
*************************************************/
int cmd_para_get_float_by_val_str(
    __IN__ const char*  para_val ,
    __OUT_ float* pFloatVals ,
    __IN__ int    nValCntMax
    )
{
    int nIntCnt = 0;
    char ch_cur = *para_val;
    while( CMD_PARA_IS_STRING_FLOAT(ch_cur) && (nIntCnt<nValCntMax) )
    {
        *pFloatVals = atof(para_val);

        pFloatVals++;
        nIntCnt++;

        //查找分隔符，并且查找下一个数字的开始位置
        do 
        {
            para_val++;
            ch_cur = *para_val;
        } while ( CMD_PARA_IS_STRING_FLOAT(ch_cur) );
        if ( ch_cur != ',' ) //如果数字间分隔符不是 , 号，表示结束了
        {
            return nIntCnt;
        }
        //定位到下一个数字的开始位置
        para_val++;
        ch_cur = *para_val;
    }
    return nIntCnt;
}

/******************************************************
		整形数据转字符串函数
        char *itoa(int value, char *string, int radix)
		radix=10 标示是10进制	非十进制，转换结果为0;  

	    例：d=-379;
		执行	itoa(d, buf, 10); 后
		
		buf="-379"							
        返回结束位置
**********************************************************/
static char *int_to_str(signed int value, char *string, int radix)
{
    char    buf_fan[13];
    char    bit_Cnt=0;

    if ( value < 0 )
    {
        *string++ = '-';
        value = -value;
    }

    while( value && (bit_Cnt<radix) )
    {
        buf_fan[bit_Cnt++] = value%10 + '0';
        value /= 10;
    }

    if ( bit_Cnt < 1 )
    {
        buf_fan[bit_Cnt++] = '0';
    }

    while ( bit_Cnt )
    {
        bit_Cnt--;
        *string++ = buf_fan[bit_Cnt];
    }
    *string = 0;
   return string;
}

//下面的float转为字符串函数，只支持非冥表示方式 值的范围需要 绝对值小于 10^8
static char* float_to_str(char *string,float val,int point_cnt)
{
    signed int ZsVal;
    if ( val < 0 )
    {
        *string++ = '-';
        val = -val;
    }

    ZsVal = (signed int)(val);
    val -= ZsVal;
    string = int_to_str(ZsVal,string,8);

    //下面转换小数
    *string++ = '.';
    while( point_cnt-- )
    {
        val *= 10;
        ZsVal = (signed int)(val);
        val -= ZsVal;
        *string++ = ZsVal+'0';
    }
    *string = 0;
    return string;
}

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个int 数据 name=234\t
*************************************************/
char* cmd_para_build_para_string_int(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int    val
    )
{
    str_cmd = box_str_cpy_rt_pos(str_cmd,str_name);
    *str_cmd++ = '=' ;
    str_cmd = int_to_str( val , str_cmd , 10 );
    *str_cmd++ = '\t' ;
    *str_cmd = 0;
    return str_cmd;
}

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 多个int 数据 name=234,255\t
*************************************************/
char* cmd_para_build_para_string_ints(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ int*    vals ,
    __IN__ int     val_cnt
    )
{
    str_cmd = box_str_cpy_rt_pos(str_cmd,str_name);
    *str_cmd++ = '=' ;
    if ( val_cnt <= 0 )
    {
        *str_cmd++ = 'X' ;
    }
    else
    {
        while(val_cnt)
        {
            str_cmd = int_to_str( *vals++ , str_cmd , 10 );
            *str_cmd++ = ',' ;
            val_cnt--;
        }
        str_cmd--; //删除最有一个 ,
        *str_cmd++ = '\t' ;
    }
    *str_cmd = 0;
    return str_cmd;

}

/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个float 数据 name=234.2\t
*************************************************/
char* cmd_para_build_para_string_float(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float        val ,
    __IN__ int          point_cnt   //数据小数的数量
    )
{
    str_cmd = box_str_cpy_rt_pos(str_cmd,str_name);
    *str_cmd++ = '=' ;
    str_cmd = float_to_str( str_cmd , val , point_cnt );
    *str_cmd++ = '\t' ;
    *str_cmd = 0;
    return str_cmd;
}

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
    )
{
    str_cmd = box_str_cpy_rt_pos(str_cmd,str_name);
    *str_cmd++ = '=' ;
    if ( val_cnt <= 0 )
    {
        *str_cmd++ = 'X' ;
    }
    else
    {
        while(val_cnt)
        {
            str_cmd = float_to_str( str_cmd , *vals++ , point_cnt );
            *str_cmd++ = ',' ;
            val_cnt--;
        }
        str_cmd--; //删除最有一个 ,
        *str_cmd++ = '\t' ;
    }
    *str_cmd = 0;
    return str_cmd;
}


/************************************************* 
*Return:      char* 返回结束位置
*DESCRIPTION: 生成参数字符串，参数值为 1个 字符串
*************************************************/
char* cmd_para_build_para_string_str(
    __OUT_ char*  str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ const char*  val
    )
{
    str_cmd = box_str_cpy_rt_pos(str_cmd,str_name);
    *str_cmd++ = '=' ;
    str_cmd = box_str_cpy_rt_pos(str_cmd,val);
    *str_cmd++ = '\t' ;
    *str_cmd = 0;
    return str_cmd;
}


