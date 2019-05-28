//////////////////////////////////////////////////////////////////////////
//CRC校验
#ifndef  __CRC16_H__
#define  __CRC16_H__

/***********************************************************************************************************************
*                                                   GetCheckAddVal
* Description: 这个功能是获得数据包的校验和
* Arguments  : pData	一个指向数据包首地址的指针
*		     len        数据包的长度
*Returns    : added    所有数据的和 
************************************************************************************************************************/
unsigned char GetCheckAddVal(unsigned char* pData,unsigned short len);

/************************************************* 
*Return:      校验值
*DESCRIPTION: CRC校验函数，凡需要用到校验的，均使用该CRC校验函数
*************************************************/
unsigned short GetCRC16(
    unsigned char *buffer,      //待校验字符串
    unsigned int length         //待校验字符串长度
    );


/************************************************* 
*Return:      unsigned short 校验值
*Description: 基于MODBUS 协议的CRC16程序实现
*************************************************/
unsigned short CRC16 (
    unsigned char* arr_buff,
    unsigned char len
    );
    

void unexist_num_add_to_array(int* Array, int* Array_len, int num);
void exist_num_rmove_from_array(int* Array, int* Array_len, int num);
void sort_array(int* Array, int Array_len);

#endif
