#ifndef __BOX_STR_H
#define __BOX_STR_H

//////////////////////////////////////////////////////////////////////////

//unsigned long 与字符串数据之间的转换
unsigned long box_str_hex_to_int(const char* strHex);
char* box_str_u16_to_hex(char* strHex,unsigned short vInt);
char* box_str_u32_to_hex(char* strHex,unsigned long  vInt);

/*
************************************************************************************************************************
*                                                   box_str_len
*
* Description: 这个功能是计算字符串
*
* Arguments  : psc	指向要计算的数组的首地址的指针
*
*
*Returns    : (psc - psc_in)    返回地址差 即为长度 
************************************************************************************************************************
*/
unsigned short box_str_len(const char* psc);

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
char* box_str_cpy_rt_pos(char* pOut,const char* pIn);

/*
************************************************************************************************************************
*                                                   box_str_cpy
*
* Description: 这个功能是拷贝字符串，并返回数据长度
*
* Arguments  : pOut	一个指向原字符串的尾的指针
*
*		    pIn        一个指向新字符串头的指针
*
*Returns    : str_len    数据长度
************************************************************************************************************************
*/
//wifi_str_cpy  返回数据长度
unsigned short box_str_cpy(char* pOut,const char* pIn);
/*
************************************************************************************************************************
*                                                   box_str_cpy_zhuanyi
*
* Description: 这个功能是拷贝字符串，并且 增加并替换成转义字符 \  ，返回结束位置
*
* Arguments  : pOut	一个指向原字符串的尾的指针
*
*		    pIn        一个指向新字符串头的指针
*
*Returns    : pOut    结束位置
************************************************************************************************************************
*/
//拷贝字符串，并且 增加转义字符 , \ " 返回结束位置
char* box_str_cpy_zhuanyi(char* pOut,const char* pIn);

/*
************************************************************************************************************************
*                                                   box_str_find_last
*
* Description: 这个功能是字符串对比，查看末尾是否某些字符串数据，查找 pSource 最后几个字节是否 是pFind 值 nFindLen为pFind的有效长度
*
* Arguments  : pSource	一个指向原数据尾地址的指针
*
*		    pFind        一个指向要查找的数据尾地址的指针
*           nFindLen      要查找数据的长度
*Returns    : 0
************************************************************************************************************************
*/
//wifi_str_find_last 字符串对比，查看末尾是否某些字符串数据
//查找 pSource 最后几个字节是否 是pFind 值 nFindLen为pFind的有效长度
unsigned char  box_str_find_last(const char* pSource,const char* pFind,unsigned char nFindLen);

/*
************************************************************************************************************************
*                                                   box_str_find_char
*
* Description: 查找字符串中 某个范围内的数据
*
* Arguments  : strd	一个指向原数据首地址的指针
*
*		    valMin        该范围的最小值
*           valMax      该范围的最大值
*Returns    : strd     在该范围内的数据的地址
************************************************************************************************************************
*/
//查找字符串中 某个范围内的数据 
char* box_str_find_char(char* strd,char valMin,char valMax);

/*
************************************************************************************************************************
*                                                   box_str_find_string_begin
*
* Description: 查找字符串中是否有strFind 字符串存在。 并且返回对应的开始位置 如果没有找到，返回 0
*
* Arguments  : pOutBig	一个指向原数据首地址的指针
*
*		    pInToFind        一个指向要查找的数据首地址的指针
*           
*Returns    : 如果存在，返回对应的开始位置 如果没有找到，返回 0
************************************************************************************************************************
*/
//wifi_str_find_string_begin:字符串pOutBig 中是否有 pInToFind
//查找字符串中是否有strFind 字符串存在。 并且返回对应的开始位置 如果没有找到，返回 0
char* box_str_find_string_begin(char* pOutBig,const char* pInToFind);

/*
************************************************************************************************************************
*                                                   box_str_find_string_end
*
* Description: 查找字符串，并且返回末尾的位置 如果没有找到，返回 NULL
*
* Arguments  : pOutBig	一个指向原数据首地址的指针
*
*		    pInToFind        一个指向要查找的数据首地址的指针
*           
*Returns    : 如果存在，返回末尾的位置 如果没有找到，返回 NULL
************************************************************************************************************************
*/
//wifi_str_find_string_end 查找字符串，并且返回末尾的位置 如果没有找到，返回 NULL
char* box_str_find_string_end(char* pOutBig,const char* pInToFind);

/*
************************************************************************************************************************
*                                                 box_str_cmp_left
*
* Description: 对比 某个字符串是否是 另外一个的左边
*
* Arguments  : p1	一个指向原数据首地址的指针
*
*		    p2        一个指向要对比的数据首地址的指针
*           
*Returns    : (*p1) - (*p2)         当(*p1)不等于(*p2)时，返回他俩的差
************************************************************************************************************************
*/
//box_str_cmp_left 对比 某个字符串是否是 另外一个的左边
char box_str_cmp_left(const char* p1,const char* p2);

//对整个字符串进行对比，字符串不同，返回差值
char box_str_cmp(const char* p1,const char* p2);
#define DoubleStringsIsSame(p1,p2)		( 0 == box_str_cmp(p1,p2) )
#define DoubleStringsIsDifferent(p1,p2)	( 0 != box_str_cmp(p1,p2) )

/*
************************************************************************************************************************
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
unsigned short box_str_find_num_end(const char* pInToFind);

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
unsigned short box_str_left_not_tdc(const char* pSource ,char* pDes , unsigned short len , char endCh);

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
signed short box_str_find_num(const char* pInToFind);

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
char* box_str_find_char_pos(char* pstr,char fval);

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
unsigned char box_str_get_int(const char* pstr,signed short* pVal);

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
signed long  box_str_to_long(const char* pstr);

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
char* box_str_s16_to_str(char* pstr,signed short val);

char* box_str_float_to_str(char* pstr,float val,unsigned char XiaoShuWei);
char* float_to_str(char *string,float val,int point_cnt);

//数字转字符串，固定长度，首位补0
void box_str_int_to_str_len(char* pstr,unsigned short val,unsigned char str_cnt);

//通过IP字符串 得到IP值 "192.168.3.4"
char box_str_ip_str_to_ips(const char* pstr,unsigned char* pIpVal);

//wifi_str_find_ip_val 字符串中查找IP地址类似的数据。并且返回找到的位置
//如果没有找到，则返回结束位置。   找到了，会直接进行一次解析地址动作
char* box_str_find_ip_val(char* pStr,unsigned char* pIpVal);

//ip地址转字符串 至少需要 4*3=12字节数据
char* box_str_ips_to_str(unsigned char* pIpVal,char* pStrOut);

//long int数据转字符串
char *int_to_str(signed long value, char *string);



/**
  * @brief  十六进制字符串转成十六进制数组
  * @param  
  * @note   "3F431A1002"-->{0x3F,0x43,0x1A,0x10,0x02} 或 "344310102"-->{0x34,0x43,0x10,0x10,0x02}
  * @retval 返回转化完成后的数组的长度
  */
int HexStrToHexArr(char *hexStr, unsigned int strLen, unsigned char *hexArr);

/**
  * @brief  十六进制数组转成十六进制字符串
  * @param  
  * @note   {0x3F,0x43,0x1A,0x10,0x02}-->"3F431A1002"
  * @retval 返回转化完成后的字符串的长度
  */
int HexArrToHexAtr(unsigned char *hexArr, unsigned int arrLen, char *hexStr);



#endif //WIFI_STRING_H__
