#include "BoxOs_str.h"

/*
************************************************************************************************************************
*                                                   box_str_len
*
* Description: ��������Ǽ����ַ����ĳ���
*
* Arguments  : psc	ָ��Ҫ�����������׵�ַ��ָ��
*
*
*Returns    : (psc - psc_in)    ���ص�ַ�� ��Ϊ���� 
************************************************************************************************************************
*/
unsigned short box_str_len(const char* psc)
{
    const char* psc_in = psc;
    while(*psc)
    {
        psc++;
    }
    return (psc - psc_in); //���ص�ַ�� ��Ϊ����
}
/*
************************************************************************************************************************
*                                                   box_str_cpy_rt_pos
*
* Description: ��������ǿ����ַ����������ؽ���λ�õ� ָ��
*
* Arguments  : pOut	һ��ָ��ԭ�ַ�����β��ָ��
*
*		    pIn        һ��ָ�����ַ���ͷ��ָ��
*
*Returns    : pOut    Դ�ַ�����β��Ҳ�����¿������ַ�����ͷ
************************************************************************************************************************
*/
//�����ַ����������ؽ���λ�õ� ָ��
///return: Դ�ַ�����β��Ҳ�����¿������ַ�����ͷ
char* box_str_cpy_rt_pos(char* pOut,const char* pIn)
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
/*
************************************************************************************************************************
*                                                  box_str_cpy
*
* Description: ��������ǿ����ַ��������������ݳ���
*
* Arguments  : pOut	һ��ָ��ԭ�ַ�����β��ָ��
*
*		    pIn        һ��ָ�����ַ���ͷ��ָ��
*
*Returns    : str_len    ���ݳ���
*************************************************************************************************************************/
unsigned short box_str_cpy(char* pOut,const char* pIn)
{
    unsigned short str_len = 0;
    while( *pIn )
    {
        *pOut = *pIn ;
        pOut++;
        pIn++;
        str_len++;
    }
    *pOut = 0 ;
    return str_len;
}
/*************************************************************************************************************************
*                                                   box_str_cpy_zhuanyi
*
* Description: ��������ǿ����ַ��������� ���Ӳ��滻��ת���ַ� \  �����ؽ���λ��
*
* Arguments  : pOut	һ��ָ��ԭ�ַ�����β��ָ��
*
*		    pIn        һ��ָ�����ַ���ͷ��ָ��
*
*Returns    : pOut    ����λ��
************************************************************************************************************************/
char* box_str_cpy_zhuanyi(char* pOut,const char* pIn)
{
    while( *pIn )
    {
        if ( (*pIn==',') || (*pIn=='\\') || (*pIn=='\"') )
        {
            *pOut++ = '\\';
        }
        *pOut++ = *pIn++ ;
    }
    *pOut = 0 ;
    return pOut;
}
/*************************************************************************************************************************
*                                                   box_str_find_last
*
* Description: �Ӻ�Ǯ�Ƚ������ַ������ݣ���ͬ�򷵻�pFind���鲻��ͬ��λ��
*
* Arguments  : pSource	һ��ָ��ԭ����β��ַ��ָ��
*
*		    pFind        һ��ָ��Ҫ���ҵ�����β��ַ��ָ��
*           nFindLen      Ҫ�������ݵĳ���
*Returns    : 0
*************************************************************************************************************************/
unsigned char  box_str_find_last(const char* pSource,const char* pFind,unsigned char nFindLen)
{
    while( nFindLen )
    {
        nFindLen--;
        if ( (*pSource) != pFind[nFindLen] )
        {
            return nFindLen+1;  //���ز�ͬ�ĳ���
        }
        pSource--;
    }
    return 0;
}
/*************************************************************************************************************************
*                                                   wifi_str_find_char
*
* Description: �����ַ����� ĳ����Χ�ڵ�����
*
* Arguments  : strd	һ��ָ��ԭ�����׵�ַ��ָ��
*
*		    valMin        �÷�Χ����Сֵ
*           valMax      �÷�Χ�����ֵ
*Returns    : strd     �ڸ÷�Χ�ڵ����ݵĵ�ַ
*************************************************************************************************************************/
char* box_str_find_char(char* strd,char valMin,char valMax)
{
    while( (*strd<valMin) || (*strd>valMax) ){
        strd++;
    }
    return strd;
}
/*************************************************************************************************************************
*                                                   box_str_find_string_begin
*
* Description: �����ַ������Ƿ���strFind �ַ������ڡ� ���ҷ��ض�Ӧ�Ŀ�ʼλ�� ���û���ҵ������� 0
*
* Arguments  : pOutBig	һ��ָ��ԭ�����׵�ַ��ָ��
*
*		    pInToFind        һ��ָ��Ҫ���ҵ������׵�ַ��ָ��
*           
*Returns    : ������ڣ����ض�Ӧ�Ŀ�ʼλ�� ���û���ҵ������� 0
*************************************************************************************************************************/
char* box_str_find_string_begin(char* pOutBig,const char* pInToFind)
{
    const char* pPosCmpFnd ;                                //�Ա�ʹ�õ�ָ��
    const char* pPosCmpSrc;                                 //�Ա�ʹ�õ�ָ��
    if ( (*pInToFind) == 0 )                                //��Ҫ�ҵ��ַ��� Ϊ ��
    {
        return 0;
    }
    while( * pOutBig )
    {
        if ( (*pOutBig) == (*pInToFind) )
        {
            pPosCmpFnd = pInToFind; //С�ַ���
            pPosCmpSrc = pOutBig;
            while( ((*pPosCmpFnd)) && ( (*pPosCmpFnd)==(*pPosCmpSrc) ) )
            {
                pPosCmpFnd++; 
                pPosCmpSrc++;
            }
            if (  (*pPosCmpFnd == 0) ) //���С�ַ���ֵΪ0 ��ʾ�ҵ���
            {
                return pOutBig;
            }
            if ( *pPosCmpSrc == 0 )
            {
                return 0;
            }
        }
        pOutBig ++;  //���� Ѱ�ҵ� ָ���1
    }
    return 0;
}
/*************************************************************************************************************************
*                                                   box_str_find_string_end
*
* Description: �����ַ��������ҷ���ĩβ��λ�� ���û���ҵ������� NULL
*
* Arguments  : pOutBig	һ��ָ��ԭ�����׵�ַ��ָ��
*
*		    pInToFind        һ��ָ��Ҫ���ҵ������׵�ַ��ָ��
*           
*Returns    : ������ڣ�����ĩβ��λ�� ���û���ҵ������� NULL
*************************************************************************************************************************/
char* box_str_find_string_end(char* pOutBig,const char* pInToFind)
{
    const char* pPosCmpFnd ;                                //�Ա�ʹ�õ�ָ��
    char* pPosCmpSrc;                                 //�Ա�ʹ�õ�ָ��
    if ( (*pInToFind) == 0 )                                //��Ҫ�ҵ��ַ��� Ϊ ��
    {
        return 0;
    }
    while( * pOutBig )
    {
        if ( (*pOutBig) == (*pInToFind) )
        {
            pPosCmpFnd = pInToFind; //С�ַ���
            pPosCmpSrc = pOutBig;
            while( ((*pPosCmpFnd)) && ( (*pPosCmpFnd)==(*pPosCmpSrc) ) )
            {
                pPosCmpFnd++; 
                pPosCmpSrc++;
            }
            if (  (*pPosCmpFnd == 0) ) //���С�ַ���ֵΪ0 ��ʾ�ҵ���
            {
                //e.g.  "we1are2here" ->"are"  and return pos->'2' 
                return pPosCmpSrc;
            }
            if ( *pPosCmpSrc == 0 )
            {
                return 0;
            }
        }
        pOutBig ++;  //���� Ѱ�ҵ� ָ���1
    }
    return 0;
}
/*************************************************************************************************************************
*                                                 box_str_cmp_left
*
* Description: �Ա� ĳ���ַ����Ƿ��� ����һ�������
*
* Arguments  : p1	һ��ָ��ԭ�����׵�ַ��ָ��
*
*		    p2        һ��ָ��Ҫ�Աȵ������׵�ַ��ָ��
*           
*Returns    : (*p1) - (*p2)         ��(*p1)������(*p2)ʱ�����������Ĳ�
*************************************************************************************************************************/
char box_str_cmp_left(const char* p1,const char* p2)
{
    while( ((*p1)) && (*p1) == (*p2) ){
        p1++;
        p2++;
    }
    if ( ((*p1) == 0) || ((*p2) == 0) )
    {
        return 0;
    }
    return (*p1) - (*p2);
}

char box_str_cmp(const char* p1,const char* p2)
{
    while( ((*p1)) &&  ( (*p1) == (*p2) )  ){
        p1++;
        p2++;
    }
    if ( ((*p1) == 0) && ((*p2) == 0) )
    {
        return 0;
    }
    return (*p1) - (*p2);
}

/*************************************************************************************************************************
*                                                 box_str_find_num_end
*
* Description: �����ֵ�β
*
* Arguments  : pInToFind	һ��ָ��Ҫ�����ҵ����ݵ��׵�ַ��ָ��
*
*		    
*           
*Returns    : nFindCounts         ���ֵĳ���
************************************************************************************************************************
*/
///�����ֵ�β
///pInToFind: [in]��ʼ�����ֵ��ַ���;[out]ָ������β����һ���ַ�
///ret: ���ֵĳ���
unsigned short box_str_find_num_end(const char* pInToFind)
{
    unsigned short nFindCounts = 0;
    while( ( ( (*pInToFind)>='0' ) && ( (*pInToFind)<='9' ) )  )
    {
        nFindCounts++;
        pInToFind++;
    }
    return nFindCounts;
}
/*
************************************************************************************************************************
*                                                 box_str_left_not_tdc
*
* Description: �����ַ����������ַ�������������Ҫ�ж� ����Ϊ�ض����ַ�
*
* Arguments  : pSource	һ��ָ��Ҫ���������ݵ��׵�ַ��ָ��
*              pDes     һ��ָ������ݵĵ�ַ��ָ��
*		       len      Ҫ���������ݵĳ���
*              endCh    �ַ�������
*Returns    : len1         ���ֵĳ���
************************************************************************************************************************
*/
//�����ַ����������ַ�������������Ҫ�ж� ����Ϊ�ض����ַ�
unsigned short box_str_left_not_tdc(const char* pSource ,char* pDes , unsigned short len , char endCh)
{
    unsigned short len1=0;
    while( (len1<len) && (*pSource) && (*pSource!=endCh) ){
        *pDes = *pSource;
        pDes++ ; pSource++;
        len1++;
    }
    *pDes = 0;
    return len1;
}
/*
************************************************************************************************************************
*                                                 box_str_find_num
*
* Description: ���� �ַ��������ֲ��ֵĳ���
*
* Arguments  : pInToFind	һ��ָ������������׵�ַ��ָ��
*              
*		       
*              
*Returns    : �ַ��������ֲ��ֵĳ��ȣ� ��û�������򷵻�-1
************************************************************************************************************************
*/
//���� �ַ��������ֲ��ֵĳ���
///pInToFind: [in] �����ҵ��ַ���; ��û��������ָ���ַ���β
///ret: �ַ��������ֲ��ֵĳ��ȣ� ��û�������򷵻�-1.
signed short box_str_find_num(const char* pInToFind)
{
    unsigned short nFindCounts = 0;
    while( ( (*pInToFind) ) && (nFindCounts<1000) )
    {
        if ( ( (*pInToFind)>='0' ) && ( (*pInToFind)<='9' ) )
        {
            return nFindCounts;
        }
        nFindCounts++;
        pInToFind++;
    }
    return -1;
}
/*
************************************************************************************************************************
*                                                 box_str_find_char_pos
*
* Description: Ѱ�� ĳ���ַ��� ��ĳλ�ַ�λ��
*
* Arguments  : pstr	һ��ָ����������ݵ�ַ��ָ��
*              fval     Ҫ���ҵ��ַ�
*		       
*              
*Returns    : 0
************************************************************************************************************************
*/
//Ѱ�� ĳ���ַ��� ��ĳλ�ַ�λ��
char* box_str_find_char_pos(char* pstr,char fval)
{
    unsigned short val_find_i = 0;
    while( (val_find_i<500) && (*pstr))
    {
        if ( *pstr == fval )
        {
            return pstr;
        }
        pstr++;
        val_find_i++;
    }
    return 0;
}
/*
************************************************************************************************************************
*                                                 box_str_get_int
*
* Description: �ַ���ת����
*
* Arguments  : pstr	һ��ָ���ַ����׵�ַ��ָ��
*              fval     һ��ָ������ֵ��׵�ַ��ָ��
*		       
*              
*Returns    : (pstr-pstr_begin)�ַ����ĳ���
************************************************************************************************************************
*/
//�ַ���ת���֣����ҷ����ַ��� ���ֳ���
unsigned char box_str_get_int(const char* pstr,signed short* pVal)
{
    const char* pstr_begin = pstr;
    unsigned char bIs_FuShu = 0;
    *pVal = 0;
    if ( *pstr == '-' )
    {
        bIs_FuShu = 1;
        pstr++;
    }
    while( (*pstr>='0') && (*pstr<='9')  )
    {
        *pVal *= 10;
        *pVal += *pstr - '0';
        pstr++;
    }
    if ( bIs_FuShu )
    {
        *pVal = -(*pVal);
    }
    return (pstr-pstr_begin);
}
/*
************************************************************************************************************************
*                                                 box_str_to_long
*
* Description: �ַ���ת����
*
* Arguments  : pstr	һ��ָ���ַ����׵�ַ��ָ��
*              
*		       
*              
*Returns    : pVal
************************************************************************************************************************
*/
//�ַ��� ת����
signed long  box_str_to_long(const char* pstr)
{
    signed long  pVal = 0;
    unsigned char bIs_FuShu = 0;
    if ( *pstr == '-' )
    {
        bIs_FuShu = 1;
        pstr++;
    }
    while( (*pstr>='0') && (*pstr<='9')  )
    {
        pVal *= 10;
        pVal += *pstr - '0';
        pstr++;
    }
    if ( bIs_FuShu )
    {
        pVal = -(pVal);
    }
    return pVal;
}
/*
************************************************************************************************************************
*                                                 box_str_s16_to_str
*
* Description: ����ת�ַ���
*
* Arguments  : pstr	һ��ָ�������׵�ַ��ָ��
*              
*		       val
*              
*Returns    : pstr   ���ؽ���λ��
************************************************************************************************************************
*/
//����ת�ַ��������ؽ���λ��
char* box_str_s16_to_str(char* pstr,signed short val)
{
    char d = 0;
    char flag = 0;
    unsigned short i = 0;
    if ( val < 0 )
    {
        *pstr++ = '-';
        val = -val;
    }
    else if ( val == 0 )
    {
        *pstr++ = '0';
    }
    for (i = 10000; i > 0; i /= 10)
    {
        d = val / i;

        if (d || flag)
        {
            *pstr++ = (char)(d + 0x30);
            val -= (d * i);
            flag = 1;
        }
    }
    *pstr = 0;
    return pstr;
}

/*
************************************************************************************************************************
*                                                 box_str_int_to_str_len
*
* Description: ����ת�ַ������̶����ȣ���λ��0
*
* Arguments  : pStr	һ��ָ�������׵�ַ��ָ��
*              
*		       val  
*              str_cnt   
*Returns    : 4
************************************************************************************************************************
*/
//����ת�ַ������̶����ȣ���λ��0
void box_str_int_to_str_len(char* pstr,unsigned short val,unsigned char str_cnt)
{
    pstr += str_cnt-1;
    while( str_cnt-- ){
        *pstr-- = (val%10) + '0';
        val /= 10;
    }
}
/*
************************************************************************************************************************
*                                                 box_str_ip_str_to_ips
*
* Description: ͨ��IP�ַ��� �õ�IPֵ "192.168.3.4"
*
* Arguments  : pStr	һ��ָ���ַ����׵�ַ��ָ��
*              
*		       pIpVal  һ��ָ����IP��ַ��ָ��
*              
*Returns    : 4
************************************************************************************************************************
*/
//ͨ��IP�ַ��� �õ�IPֵ "192.168.3.4"
char box_str_ip_str_to_ips(const char* pstr,unsigned char* pIpVal)
{
    signed short getVal = 0;
    char i=0;
    for ( ; i<3 ; i++ )
    {
        pstr += box_str_get_int(pstr,&getVal);
        if( *pstr != '.' ){ return i; }
        pIpVal[i] = getVal;
        pstr++;
    }
    box_str_get_int(pstr,&getVal);
    pIpVal[3] = getVal;
    return 4; //���� 4
}
/*
************************************************************************************************************************
*                                                 box_str_find_ip_val
*
* Description: �ַ����в���IP��ַ���Ƶ����ݡ����ҷ����ҵ���λ��
*
* Arguments  : pStr	һ��ָ���ַ����׵�ַ��ָ��
*              
*		       pIpVal  һ��ָ����IP��ַ��ָ��
*              
*Returns    : ���û���ҵ����򷵻ؽ���λ�á�   �ҵ��ˣ���ֱ�ӽ���һ�ν�����ַ����
************************************************************************************************************************
*/
char* box_str_find_ip_val(char* pStr,unsigned char* pIpVal)
{
    signed short getVal = 0;
    char i=0;
    while( *pStr ){
        //��Ѱ�� ���ֿ�ʼλ��
        while( (*pStr<'0') || (*pStr>'9') ){ pStr++; if( *pStr == 0) return 0; }
        for ( i=0; i<3 ; i++ )
        {
            pStr += box_str_get_int(pStr,&getVal);
            if( *pStr != '.' ){ break; }
            pIpVal[i] = getVal;
            pStr++;
        }
        if ( i >= 3 )
        {
            pStr += box_str_get_int(pStr,&getVal);
            pIpVal[3] = getVal;
            return pStr; // ���� ���� λ��
        }
    }
    return 0; //���� 4
}
/*
************************************************************************************************************************
*                                                 box_str_ips_to_str
*
* Description: ip��ַת�ַ���
*
* Arguments  : pIpVal	һ��ָ�������׵�ַ��ָ��
*              
*		       pStrOut  һ��ָ����IP��ַ��ָ��
*              
*Returns    : pStrOut   ���ؽ���λ��
************************************************************************************************************************
*/
//ip��ַת�ַ��� ������Ҫ 4*3=12�ֽ�����

char* box_str_ips_to_str(unsigned char* pIpVal,char* pStrOut)
{
    unsigned char i=0;
    for( i=0 ; i<4 ; i++ )
    {
        pStrOut = box_str_s16_to_str( pStrOut , pIpVal[i] );
        *pStrOut++ = '.';
    }
    pStrOut--; //���һ�� '.' λ��
    *pStrOut = 0;
    return pStrOut;
}

char *int_to_str(signed long value, char *string)
{
    char    buf_fan[13];
    char    bit_Cnt=0;

    if ( value < 0 )
    {
        *string++ = '-';
        value = -value;
    }

    while( value )
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

char* box_str_float_to_str(char* pstr,float val,unsigned char XiaoShuWei)
{
    char xiaos;
    unsigned long nzs = 0;
    if ( val < 0.0f )
    {
        *pstr++ = '-';
        val = -val;
    }
    nzs = (unsigned long) val;
    if ( nzs > 30000 )
    {
        pstr = box_str_s16_to_str(pstr, nzs/10000 );
        box_str_int_to_str_len(pstr, nzs%10000 , 4 );
        pstr += 4;
    }
    else{
        pstr = box_str_s16_to_str(pstr, nzs );
    }
    *pstr++ = '.';
    val -= nzs;
    while( XiaoShuWei-- )
    {
        val *= 10.0f;
        xiaos =  ((char)val) ;
        val -= xiaos;
        *pstr++ = '0' + xiaos;
    }
    *pstr = 0;
    return pstr;
}

char* float_to_str(char *string,float val,int point_cnt)
{
    signed int ZsVal;
    if ( val < 0 )
    {
        *string++ = '-';
        val = -val;
    }

    ZsVal = (signed int)(val);
    val -= ZsVal;
    string = int_to_str(ZsVal,string);

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


//////////////////////////////////////////////////////////////////////////
//unsigned long ���ַ�������֮���ת��;
//FA34 -> 0XFA34==64052
#define HEX_CHAR_TO_INT_VAL(ch) ( (((ch)<='9') && ((ch)>='0'))? ((ch)-'0') : \
    ( (((ch)<='F')&&((ch)>='A'))? ((ch)-('A'-10)) : ( (((ch)<='f')&&((ch)>='a'))? ((ch)-('a'-10)) : 30) ) )
#define INT_VAL_TO_HEX_CHAR(iVal)  ( ((iVal)<10)? ('0'+(iVal)) : ('A'-10+(iVal)) )
unsigned long box_str_hex_to_int(const char* strHex)
{
    unsigned long rt=0;
    char char_hex = *strHex++;
    unsigned char bitHex = HEX_CHAR_TO_INT_VAL(char_hex);
    while( bitHex <= 0X0F )
    {
        rt <<= 4;
        rt |= bitHex;
        char_hex = *strHex++;
        bitHex = HEX_CHAR_TO_INT_VAL(char_hex);
    }
    return rt;
}

char* box_str_u16_to_hex(char* strHex,unsigned short vInt)
{
    unsigned char i;
    unsigned char one_bit ;
    for( i=0 ; i<4 ; i++ )
    {
        one_bit = (vInt>>12)&0X0F ;
        vInt <<= 4;
        *strHex++ = INT_VAL_TO_HEX_CHAR(one_bit);
    }
    *strHex = 0;
    return strHex;
}
char* box_str_u32_to_hex(char* strHex,unsigned long  vInt)
{
    unsigned char i;
    unsigned char one_bit ;
    for( i=0 ; i<8 ; i++ )
    {
        one_bit = (vInt>>28)&0X0F ;
        vInt <<= 4;
        *strHex++ = INT_VAL_TO_HEX_CHAR(one_bit);
    }
    *strHex = 0;
    return strHex;
}

/**
  * @brief  ʮ�������ַ���ת��ʮ����������
  * @param  
  * @note   "3F431A1002"-->{0x3F,0x43,0x1A,0x10,0x02} �� "344310102"-->{0x34,0x43,0x10,0x10,0x02}
  * @retval ����ת����ɺ������ĳ���
  */
#define HEX_CHAR_TO_HEX_VAL(hexChar)  ( ((hexChar > '9') && ((hexChar <= 'F') || (hexChar <= 'f'))) ? (hexChar - '0' - 7) : (hexChar - '0') )
int HexStrToHexArr(char *hexStr, unsigned int strLen, unsigned char *hexArr)
{
    char *p = hexStr;
    char high = 0, low = 0;
    int  cnt = 0;
    
    while(cnt < (strLen / 2))
    {
        high    = HEX_CHAR_TO_HEX_VAL( *p );
        p++;
		low     = HEX_CHAR_TO_HEX_VAL( *p );
        hexArr[cnt] = ((high & 0x0f) << 4 | (low & 0x0f));
        p ++;
        cnt ++;
    }
    if(strLen % 2 != 0) hexArr[cnt] = HEX_CHAR_TO_HEX_VAL( *p );
    
//    if(arrLen != NULL) *arrLen = tmplen / 2 + tmplen % 2;
    return strLen / 2 + strLen % 2;
}

/**
  * @brief  ʮ����������ת��ʮ�������ַ���
  * @param  
  * @note   {0x3F,0x43,0x1A,0x10,0x02}-->"3F431A1002"
  * @retval ����ת����ɺ���ַ����ĳ���
  */
#define HEX_VAL_TO_HEX_CHAR(hexVal)  ( ((hexVal)<10)? ('0'+(hexVal)) : ('A'-10+(hexVal)) )
int HexArrToHexAtr(unsigned char *hexArr, unsigned int arrLen, char *hexStr)
{
    unsigned char *p = hexArr;
    char high = 0, low = 0;
    int  cnt = 0;
    
    while( cnt < (arrLen * 2) ) {
        low     = *p & 0x0F;
        high    = *p>>4 & 0x0F;
        hexStr[cnt++]   = HEX_VAL_TO_HEX_CHAR( high );
        hexStr[cnt++]   = HEX_VAL_TO_HEX_CHAR( low );
        p++;
    }
    
    return cnt;
}








