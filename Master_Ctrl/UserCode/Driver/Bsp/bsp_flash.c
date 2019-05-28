/*
*********************************************************************************************************
*
*
*                                   Flash emulation EEPROM driver package
*                                         flash模拟EEPROM驱动程序包
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "bsp_flash.h"

#if SYSTEM_SUPPORT_OS
    #include "os_cpu.h"
#endif


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#if STM32_FLASH_SIZE<256
#define STM_SECTOR_SIZE 1024 //字节
#else 
#define STM_SECTOR_SIZE	2048
#endif		 


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
#if SYSTEM_SUPPORT_OS
//    OS_CPU_SR   cpu_sr;
#endif

u16 STMFLASH_BUF[STM_SECTOR_SIZE/2];//最多是2K字节

Flash_CtrlTypeDef   Flash_CtrlStructure;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
//读取指定地址的半字(16位数据)
//faddr:读地址(此地址必须为2的倍数!!)
//返回值:对应数据.
/**
  * @brief  使能SPI时钟
  * @retval None
  */
u16 STMFLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}

/**
  * @brief  使能SPI时钟
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
  * @brief  使能SPI时钟
  * @retval None
  */
//不检查的写入
//WriteAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数   
void STMFLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
	    WriteAddr+=2;//地址增加2.
	}  
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        SPI1_Configuration()
*
* Description : SPI1 configuration function.    SPI1配置函数
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
        FLASH_ErasePage(FLASH_SAVE_ADDR);//擦除整页
    }
}

//从指定地址开始写入指定长度的数据
//WriteAddr:起始地址(此地址必须为2的倍数!!)
//pBuffer:数据指针
//NumToWrite:半字(16位)数(就是要写入的16位数据的个数.)
void STMFLASH_Write(u16 *pBuffer,u16 NumToWrite)	
{
    u32 WriteAddr;   
 	u16 i;
    
    if( !pBuffer )  //检查pBuffer是否有效
    {
        return;
    }
    
    if( NumToWrite > MAX_SIZE_PER_BLOCK-2 ) //检查待写入的数据长度是否超出每一个存储块的最大数量
    {
        return;
    }
    
	FLASH_Unlock();						    //解锁
    
    if( Flash_CtrlStructure.curNum_block == Flash_CtrlStructure.cnts_block-1 )
    {
        FLASH_ErasePage(FLASH_SAVE_ADDR);//擦除这个扇区
        
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
    
	FLASH_Lock();//上锁
}


//从指定地址开始读出指定长度的数据
//ReadAddr:起始地址
//pBuffer:数据指针
//NumToWrite:半字(16位)数
u8 STMFLASH_Read(u16 *pBuffer,u16 NumToRead)
{
	u16 i;
    u32 ReadAddr= FLASH_SAVE_ADDR;
    
    if( NumToRead > MAX_SIZE_PER_BLOCK-2 ) //检查待写入的数据长度是否超出每一个存储块的最大数量
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




