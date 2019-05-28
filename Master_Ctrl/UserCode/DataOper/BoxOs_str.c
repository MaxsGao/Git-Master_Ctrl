#include "BoxOs_str.h"

/*
************************************************************************************************************************
*                                                   box_str_len
*
* Description: 这个功能是计算字符串的长度
*
* Arguments  : psc	指向要计算的数组的首地址的指针
*
*
*Returns    : (psc - psc_in)    返回地址差 即为长度 
************************************************************************************************************************
*/
unsigned short box_str_len(const char* psc)
{
    const char* psc_in = psc;
    while(*psc)
    {
        psc++;
    }
    return (psc - psc_in); //返回地址差 即为长度
}
/*
************************************************************************************************************************
*                                                   box_str_cpy_rt_pos
*
* Description: 这个功能是拷贝字符串，并返回结束位置的 指针
*
* Arguments  : pOut	一个指向原字符串的尾的指针
*
*		    pIn        一个指向新字符串头的指针
*
*Returns    : pOut    源字符串的尾，也就是新拷贝的字符串的头
************************************************************************************************************************
*/
//拷贝字符串，并返回结束位置的 指针
///return: 源字符串的尾，也就是新拷贝的字符串的头
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
* Description: 这个功能是拷贝字符串，并返回数据长度
*
* Arguments  : pOut	一个指向原字符串的尾的指针
*
*		    pIn        一个指向新字符串头的指针
*
*Returns    : str_len    数据长度
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
* Description: 这个功能是拷贝字符串，并且 增加并替换成转义字符 \  ，返回结束位置
*
* Arguments  : pOut	一个指向原字符串的尾的指针
*
*		    pIn        一个指向新字符串头的指针
*
*Returns    : pOut    结束位置
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
* Description: 从后到钱比较两个字符的数据，不同则返回pFind数组不相同的位置
*
* Arguments  : pSource	一个指向原数据尾地址的指针
*
*		    pFind        一个指向要查找的数据尾地址的指针
*           nFindLen      要查找数据的长度
*Returns    : 0
*************************************************************************************************************************/
unsigned char  box_str_find_last(const char* pSource,const char* pFind,unsigned char nFindLen)
{
    while( nFindLen )
    {
        nFindLen--;
        if ( (*pSource) != pFind[nFindLen] )
        {
            return nFindLen+1;  //返回不同的长度
        }
        pSource--;
    }
    return 0;
}
/*************************************************************************************************************************
*                                                   wifi_str_find_char
*
* Description: 查找字符串中 某个范围内的数据
*
* Arguments  : strd	一个指向原数据首地址的指针
*
*		    valMin        该范围的最小值
*           valMax      该范围的最大值
*Returns    : strd     在该范围内的数据的地址
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
* Description: 查找字符串中是否有strFind 字符串存在。 并且返回对应的开始位置 如果没有找到，返回 0
*
* Arguments  : pOutBig	一个指向原数据首地址的指针
*
*		    pInToFind        一个指向要查找的数据首地址的指针
*           
*Returns    : 如果存在，返回对应的开始位置 如果没有找到，返回 0
*************************************************************************************************************************/
char* box_str_find_string_begin(char* pOutBig,const char* pInToFind)
{
    const char* pPosCmpFnd ;                                //对比使用的指针
    const char* pPosCmpSrc;                                 //对比使用的指针
    if ( (*pInToFind) == 0 )                                //需要找的字符串 为 空
    {
        return 0;
    }
    while( * pOutBig )
    {
        if ( (*pOutBig) == (*pInToFind) )
        {
            pPosCmpFnd = pInToFind; //小字符串
            pPosCmpSrc = pOutBig;
            while( ((*pPosCmpFnd)) && ( (*pPosCmpFnd)==(*pPosCmpSrc) ) )
            {
                pPosCmpFnd++; 
                pPosCmpSrc++;
            }
            if (  (*pPosCmpFnd == 0) ) //如果小字符串值为0 表示找到了
            {
                return pOutBig;
            }
            if ( *pPosCmpSrc == 0 )
            {
                return 0;
            }
        }
        pOutBig ++;  //整体 寻找的 指针加1
    }
    return 0;
}
/*************************************************************************************************************************
*                                                   box_str_find_string_end
*
* Description: 查找字符串，并且返回末尾的位置 如果没有找到，返回 NULL
*
* Arguments  : pOutBig	一个指向原数据首地址的指针
*
*		    pInToFind        一个指向要查找的数据首地址的指针
*           
*Returns    : 如果存在，返回末尾的位置 如果没有找到，返回 NULL
*************************************************************************************************************************/
char* box_str_find_string_end(char* pOutBig,const char* pInToFind)
{
    const char* pPosCmpFnd ;                                //对比使用的指针
    char* pPosCmpSrc;                                 //对比使用的指针
    if ( (*pInToFind) == 0 )                                //需要找的字符串 为 空
    {
        return 0;
    }
    while( * pOutBig )
    {
        if ( (*pOutBig) == (*pInToFind) )
        {
            pPosCmpFnd = pInToFind; //小字符串
            pPosCmpSrc = pOutBig;
            while( ((*pPosCmpFnd)) && ( (*pPosCmpFnd)==(*pPosCmpSrc) ) )
            {
                pPosCmpFnd++; 
                pPosCmpSrc++;
            }
            if (  (*pPosCmpFnd == 0) ) //如果小字符串值为0 表示找到了
            {
                //e.g.  "we1are2here" ->"are"  and return pos->'2' 
                return pPosCmpSrc;
            }
            if ( *pPosCmpSrc == 0 )
            {
                return 0;
            }
        }
        pOutBig ++;  //整体 寻找的 指针加1
    }
    return 0;
}
/*************************************************************************************************************************
*                                                 box_str_cmp_left
*
* Description: 对比 某个字符串是否是 另外一个的左边
*
* Arguments  : p1	一个指向原数据首地址的指针
*
*		    p2        一个指向要对比的数据首地址的指针
*           
*Returns    : (*p1) - (*p2)         当(*p1)不等于(*p2)时，返回他俩的差
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
* Description: 找数字的尾
*
* Arguments  : pInToFind	一个指向要被查找的数据的首地址的指针
*
*		    
*           
*Returns    : nFindCounts         数字的长度
************************************************************************************************************************
*/
///找数字的尾
///pInToFind: [in]起始是数字的字符串;[out]指向数字尾的下一个字符
///ret: 数字的长度
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
* Description: 拷贝字符串，并且字符串拷贝过程需要判断 不能为特定的字符
*
* Arguments  : pSource	一个指向要被拷贝数据的首地址的指针
*              pDes     一个指向放数据的地址的指针
*		       len      要被拷贝数据的长度
*              endCh    字符结束符
*Returns    : len1         数字的长度
************************************************************************************************************************
*/
//拷贝字符串，并且字符串拷贝过程需要判断 不能为特定的字符
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
* Description: 查找 字符串中数字部分的长度
*
* Arguments  : pInToFind	一个指向待查找数据首地址的指针
*              
*		       
*              
*Returns    : 字符串中数字部分的长度， 若没有数字则返回-1
************************************************************************************************************************
*/
//查找 字符串中数字部分的长度
///pInToFind: [in] 待查找的字符串; 若没有数字则指向字符串尾
///ret: 字符串中数字部分的长度， 若没有数字则返回-1.
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
* Description: 寻找 某个字符串 中某位字符位置
*
* Arguments  : pstr	一个指向待查找数据地址的指针
*              fval     要查找的字符
*		       
*              
*Returns    : 0
************************************************************************************************************************
*/
//寻找 某个字符串 中某位字符位置
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
* Description: 字符串转数字
*
* Arguments  : pstr	一个指向字符串首地址的指针
*              fval     一个指向放数字的首地址的指针
*		       
*              
*Returns    : (pstr-pstr_begin)字符串的长度
************************************************************************************************************************
*/
//字符串转数字，并且返回字符串 数字长度
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
* Description: 字符串转数字
*
* Arguments  : pstr	一个指向字符串首地址的指针
*              
*		       
*              
*Returns    : pVal
************************************************************************************************************************
*/
//字符串 转数字
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
* Description: 数字转字符串
*
* Arguments  : pstr	一个指向数字首地址的指针
*              
*		       val
*              
*Returns    : pstr   返回结束位置
************************************************************************************************************************
*/
//数字转字符串，返回结束位置
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
* Description: 数字转字符串，固定长度，首位补0
*
* Arguments  : pStr	一个指向数字首地址的指针
*              
*		       val  
*              str_cnt   
*Returns    : 4
************************************************************************************************************************
*/
//数字转字符串，固定长度，首位补0
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
* Description: 通过IP字符串 得到IP值 "192.168.3.4"
*
* Arguments  : pStr	一个指向字符串首地址的指针
*              
*		       pIpVal  一个指向存放IP地址的指针
*              
*Returns    : 4
************************************************************************************************************************
*/
//通过IP字符串 得到IP值 "192.168.3.4"
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
    return 4; //返回 4
}
/*
************************************************************************************************************************
*                                                 box_str_find_ip_val
*
* Description: 字符串中查找IP地址类似的数据。并且返回找到的位置
*
* Arguments  : pStr	一个指向字符串首地址的指针
*              
*		       pIpVal  一个指向存放IP地址的指针
*              
*Returns    : 如果没有找到，则返回结束位置。   找到了，会直接进行一次解析地址动作
************************************************************************************************************************
*/
char* box_str_find_ip_val(char* pStr,unsigned char* pIpVal)
{
    signed short getVal = 0;
    char i=0;
    while( *pStr ){
        //先寻找 数字开始位置
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
            return pStr; // 返回 结束 位置
        }
    }
    return 0; //返回 4
}
/*
************************************************************************************************************************
*                                                 box_str_ips_to_str
*
* Description: ip地址转字符串
*
* Arguments  : pIpVal	一个指向数字首地址的指针
*              
*		       pStrOut  一个指向存放IP地址的指针
*              
*Returns    : pStrOut   返回结束位置
************************************************************************************************************************
*/
//ip地址转字符串 至少需要 4*3=12字节数据

char* box_str_ips_to_str(unsigned char* pIpVal,char* pStrOut)
{
    unsigned char i=0;
    for( i=0 ; i<4 ; i++ )
    {
        pStrOut = box_str_s16_to_str( pStrOut , pIpVal[i] );
        *pStrOut++ = '.';
    }
    pStrOut--; //最后一个 '.' 位置
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
//unsigned long 与字符串数据之间的转换;
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
  * @brief  十六进制字符串转成十六进制数组
  * @param  
  * @note   "3F431A1002"-->{0x3F,0x43,0x1A,0x10,0x02} 或 "344310102"-->{0x34,0x43,0x10,0x10,0x02}
  * @retval 返回转化完成后的数组的长度
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
  * @brief  十六进制数组转成十六进制字符串
  * @param  
  * @note   {0x3F,0x43,0x1A,0x10,0x02}-->"3F431A1002"
  * @retval 返回转化完成后的字符串的长度
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








