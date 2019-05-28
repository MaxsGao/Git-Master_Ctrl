/**************************************************************
COPYRIGHT (C), BOLIGTHED Co., Ltd.  
*Filename:  		cmd_para.c
*Version:			V
*Data:			2016.11.30
*Description:  	ͨ��������շ����ݴ�����ļ�
***************************************************************/
#include "cmd_para.h"

//�����ַ����������ؽ���λ�õ� ָ��
///return: Դ�ַ�����β��Ҳ�����¿������ַ�����ͷ
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

//��Ҫ��ǰ�ַ����� ������λ��
//������Ϊ ' ' '\t' '\r' '\n' '=' '\0'
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

//������һ�� ���ƻ��� ֵ�Ŀ�ʼλ�� ���֣���ĸ��-,+
const char* cmd_para_get_next_str_begin(const char* pstr)
{
    char cur_ch ;
    while(1)
    {
        cur_ch = *pstr ;
        //����ַ����Ѿ�������
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
*Return:      int �ֽ�����Ʋ�������
*DESCRIPTION: �ֽ������������ Ԫ����
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
        pitem->item_begin_pos = str_name_begin - cmd_str; //��ʼλ��
        //��Ѱ�ҷָ���
        str = cmd_para_get_cur_str_end(str_name_begin); //���ҷָ���
        if ( *str == '=' ) //����ָ���Ϊ ��=�� �ַ�������
        {
            pitem->name_str_len = str - str_name_begin;
            if ( (pitem->name_str_len == 0) || 
                 (pitem->name_str_len >= sizeof(pitem->name)) )
            { //���Ƴ����쳣 ����0
                cmdReslut->item_cnt = 0;
                return 0;
            }
            memcpy( pitem->name , str_name_begin , pitem->name_str_len );
            pitem->name[pitem->name_str_len] = 0;

            //����ֵ�Ŀ�ʼλ��
            str_name_begin = cmd_para_get_next_str_begin(str);
            if ( CMD_PARA_IS_CHAR_END_CMD(*str_name_begin) )
            {
                cmdReslut->item_cnt = 0;
                return 0;
            }
            str = cmd_para_get_cur_str_end(str_name_begin); //���ҷָ���
            pitem->value_str_len = str - str_name_begin;
            if ( (pitem->value_str_len >= sizeof(pitem->value)) )
            { //���Ƴ����쳣 ����0
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
            //�ҵ���һ���Ŀ�ʼλ��
            str_name_begin = cmd_para_get_next_str_begin(str);
            if ( CMD_PARA_IS_CHAR_END_CMD(*str_name_begin) ) //�Ѿ�������λ���� ֱ�ӷ���
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
*Return:      int ��������Ӧ��ID���� <0Ϊδ�ҵ�
*DESCRIPTION: ���Ҳ����б��У�ĳһ�����ƵĲ���������λ��
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
        if ( cmdInfo->items[i].name_str_len == name_len ) //�����ַ���������ͬ
        {
            if ( 0 == memcmp(cmdInfo->items[i].name , para_name , name_len) ) //ֱ��ʹ��memcmp������Ч�ʸ�
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
*Return:      int �ҵ���int���ݵ�����
*DESCRIPTION: ����ֵ �ַ����� int ������ ��ʽΪ 12,34,56 �м䲻���пո�� �ָ���ֻ���� ,��
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

        //���ҷָ��������Ҳ�����һ�����ֵĿ�ʼλ��
        do 
        {
            para_val++;
            ch_cur = *para_val;
        } while ( CMD_PARA_IS_STRING_NUMBER(ch_cur) );
        if ( ch_cur != ',' ) //������ּ�ָ������� , �ţ���ʾ������
        {
            return nIntCnt;
        }
        //��λ����һ�����ֵĿ�ʼλ��
        para_val++;
        ch_cur = *para_val;
    }
    return nIntCnt;
}

/************************************************* 
*Return:      int �ҵ���float���ݵ�����
*DESCRIPTION: ����ֵ �ַ����� float ������ �ָ���ֻ���� ,��
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

        //���ҷָ��������Ҳ�����һ�����ֵĿ�ʼλ��
        do 
        {
            para_val++;
            ch_cur = *para_val;
        } while ( CMD_PARA_IS_STRING_FLOAT(ch_cur) );
        if ( ch_cur != ',' ) //������ּ�ָ������� , �ţ���ʾ������
        {
            return nIntCnt;
        }
        //��λ����һ�����ֵĿ�ʼλ��
        para_val++;
        ch_cur = *para_val;
    }
    return nIntCnt;
}

/******************************************************
		��������ת�ַ�������
        char *itoa(int value, char *string, int radix)
		radix=10 ��ʾ��10����	��ʮ���ƣ�ת�����Ϊ0;  

	    ����d=-379;
		ִ��	itoa(d, buf, 10); ��
		
		buf="-379"							
        ���ؽ���λ��
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

//�����floatתΪ�ַ���������ֻ֧�ַ�ڤ��ʾ��ʽ ֵ�ķ�Χ��Ҫ ����ֵС�� 10^8
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

    //����ת��С��
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
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1��int ���� name=234\t
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
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ ���int ���� name=234,255\t
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
        str_cmd--; //ɾ������һ�� ,
        *str_cmd++ = '\t' ;
    }
    *str_cmd = 0;
    return str_cmd;

}

/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1��float ���� name=234.2\t
*************************************************/
char* cmd_para_build_para_string_float(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float        val ,
    __IN__ int          point_cnt   //����С��������
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
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ ���float ���� name=234.2,-33.2\t
*************************************************/
char* cmd_para_build_para_string_floats(
    __OUT_ char*        str_cmd ,
    __IN__ const char*  str_name ,
    __IN__ float*       vals ,
    __IN__ int          val_cnt ,
    __IN__ int          point_cnt   //����С��������
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
        str_cmd--; //ɾ������һ�� ,
        *str_cmd++ = '\t' ;
    }
    *str_cmd = 0;
    return str_cmd;
}


/************************************************* 
*Return:      char* ���ؽ���λ��
*DESCRIPTION: ���ɲ����ַ���������ֵΪ 1�� �ַ���
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


