//////////////////////////////////////////////////////////////////////////
//CRCУ��
#ifndef  __CRC16_H__
#define  __CRC16_H__

/***********************************************************************************************************************
*                                                   GetCheckAddVal
* Description: ��������ǻ�����ݰ���У���
* Arguments  : pData	һ��ָ�����ݰ��׵�ַ��ָ��
*		     len        ���ݰ��ĳ���
*Returns    : added    �������ݵĺ� 
************************************************************************************************************************/
unsigned char GetCheckAddVal(unsigned char* pData,unsigned short len);

/************************************************* 
*Return:      У��ֵ
*DESCRIPTION: CRCУ�麯��������Ҫ�õ�У��ģ���ʹ�ø�CRCУ�麯��
*************************************************/
unsigned short GetCRC16(
    unsigned char *buffer,      //��У���ַ���
    unsigned int length         //��У���ַ�������
    );


/************************************************* 
*Return:      unsigned short У��ֵ
*Description: ����MODBUS Э���CRC16����ʵ��
*************************************************/
unsigned short CRC16 (
    unsigned char* arr_buff,
    unsigned char len
    );
    

void unexist_num_add_to_array(int* Array, int* Array_len, int num);
void exist_num_rmove_from_array(int* Array, int* Array_len, int num);
void sort_array(int* Array, int Array_len);

#endif
