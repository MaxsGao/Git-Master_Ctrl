/*
*********************************************************************************************************
*
*
*                                   Flash emulation EEPROM driver package
*                                         flashģ��EEPROM���������
* File : bsp_flash.h
* By   : 
* Date : 
* version : 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "bsp_flash.h"

#if SYSTEM_SUPPORT_OS
    #include "os_cpu.h"
#endif


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //�ֽ�
#else 
#define STM_SECTOR_SIZE	2048
#endif		 


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
#if SYSTEM_SUPPORT_OS
//    OS_CPU_SR   cpu_sr;
#endif

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//�����2K�ֽ�

Flash_CtrlTypeDef   Flash_CtrlStructure;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
/**
  * @brief  ʹ��SPIʱ��
  * @retval None
  */
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

/**
  * @brief  ʹ��SPIʱ��
  * @retval None
  */
static u16 STMFLASH_FindVaildBlock(void)
{
    u16 flag;
    u32 ReadAddr;
    
    Flash_CtrlStructure.curNum_block    = 0;
    
    while( Flash_CtrlStructure.curNum_block < Flash_CtrlStructure.cnts_block )
    {
        ReadAddr = FLASH_SAVE_ADDR+(0x40*Flash_CtrlStructure.curNum_block);
        
        flag = STMFLASH_ReadHalfWord( ReadAddr );
        if( flag == FLAG_VALID_BLOCK )
        {
            Flash_CtrlStructure.have_valid = 1;
            return Flash_CtrlStructure.have_valid;
        }
        
        Flash_CtrlStructure.curNum_block++;
    }
    
    Flash_CtrlStructure.curNum_block = 0;
    Flash_CtrlStructure.have_valid   = 0;
    return Flash_CtrlStructure.have_valid;
}

/**
  * @brief  ʹ��SPIʱ��
  * @retval None
  */
//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//��ַ����2.
	}  
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        SPI1_Configuration()
*
* Description : SPI1 configuration function.    SPI1���ú���
*
* Arguments   : void
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void STMFLASH_Init(void)
{
    Flash_CtrlStructure.cnts_block      = 2048/32;
    
    if( 0==STMFLASH_FindVaildBlock() )
    {
        FLASH_ErasePage(FLASH_SAVE_ADDR);//������ҳ
    }
}

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void STMFLASH_Write(u16 *pBuffer,u16 NumToWrite)	
{
    u32 WriteAddr;   
 	u16 i;
    
    if( !pBuffer )  //���pBuffer�Ƿ���Ч
    {
        return;
    }
    
    if( NumToWrite > MAX_SIZE_PER_BLOCK-2 ) //����д������ݳ����Ƿ񳬳�ÿһ���洢����������
    {
        return;
    }
    
	FLASH_Unlock();						    //����
    
    if( Flash_CtrlStructure.curNum_block == Flash_CtrlStructure.cnts_block-1 )
    {
        FLASH_ErasePage(FLASH_SAVE_ADDR);//�����������
        
        Flash_CtrlStructure.curNum_block = 0;
    }
    else
    {
        WriteAddr = FLASH_SAVE_ADDR + (0x40*Flash_CtrlStructure.curNum_block);
        FLASH_ProgramHalfWord(WriteAddr,FLAG_INVALID_BLOCK);
        
        Flash_CtrlStructure.curNum_block += 1;
    }
    
    STMFLASH_BUF[0] = FLAG_VALID_BLOCK;
    for( i=0;i<NumToWrite;i++ )
    {
        STMFLASH_BUF[i+1] = pBuffer[i];
    }
    WriteAddr = FLASH_SAVE_ADDR + (0x40*Flash_CtrlStructure.curNum_block);
    STMFLASH_Write_NoCheck( WriteAddr,STMFLASH_BUF,NumToWrite+1 );
    
	FLASH_Lock();//����
}


//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
u8 STMFLASH_Read(u16 *pBuffer,u16 NumToRead)
{
	u16 i;
    u32 ReadAddr= FLASH_SAVE_ADDR;
    
    if( NumToRead > MAX_SIZE_PER_BLOCK-2 ) //����д������ݳ����Ƿ񳬳�ÿһ���洢����������
    {
        return 0;
    }
    
    if( Flash_CtrlStructure.have_valid )
    {
        ReadAddr += 0x40*Flash_CtrlStructure.curNum_block;
        for( i=0;i<NumToRead;i++ )
        {
            ReadAddr += 2;
            pBuffer[i]=STMFLASH_ReadHalfWord(ReadAddr);
        }
        return 1;
    }
    
    return 0;
}




