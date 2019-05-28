/*
*********************************************************************************************************
*
*
*                                       SPI Driver Package
*                                         SPI驱动程序包
* File : bsp_spi.c
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
#include "bsp_spi.h"

#if SYSTEM_SUPPORT_OS
    #include "os_cpu.h"
#endif


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
#if SYSTEM_SUPPORT_OS
    OS_CPU_SR   cpu_sr;
#endif


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        SPI1_Config()
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
void SPI1_Config(void)
{
	SPI_InitTypeDef     SPI_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE ); //PORTA时钟使能
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE ); //SPI1时钟使能
    
	//PA5->SCK,PA6->MISO,PA7->MOSI
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5 | GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1, ENABLE);
//    SPI1_ReadWriteByte(0xff);
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
void SPI1_SetSpeed(u8 SpeedSet)
{
    SPI_InitTypeDef  SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
    SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
}

/*
*********************************************************************************************************
*                                        SPI1_WriteByte()
*
* Description : Write 1 byte of data to the SPI bus.    写1字节数据到SPI总线
*
* Arguments   : TxData  The data to write to the bus    要写入到总线的数据
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI1_WriteByte(uint8_t TxData)
{				 
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空		  
	SPI1->DR=TxData;	 	  									//发送一个byte 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	SPI1->DR;
}

/*
*********************************************************************************************************
*                                        SPI1_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  从SPI总线读取1字节数据
*
* Arguments   : void
*
* Return      : uint8_t  Read data  读取到的数据
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI1_ReadByte(void)
{			 
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空			  
	SPI1->DR=0xFF;	 	  										//发送一个空数据产生输入数据的时钟 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	return SPI1->DR;  						    
}

/*
*********************************************************************************************************
*                                        SPI3_Config()
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
void SPI3_Config(void)
{
	SPI_InitTypeDef     SPI_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE ); //PORTB时钟使能
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3,  ENABLE ); //SPI3时钟使能
    
	//PA5->SCK,PA6->MISO,PA7->MOSI
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction         = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode              = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize          = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL              = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA              = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS               = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit          = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial     = 7;
	SPI_Init(SPI3,&SPI_InitStructure);
	
	SPI_Cmd(SPI3, ENABLE);
//    SPI1_ReadWriteByte(0xff);
}

//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8分频   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16分频  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256分频 (SPI 281.25K@sys 72M)
void SPI3_SetSpeed(u8 SpeedSet)
{
    SPI_InitTypeDef  SPI_InitStructure;
    SPI_InitStructure.SPI_BaudRatePrescaler = SpeedSet ;
    SPI_Init(SPI3, &SPI_InitStructure);
	SPI_Cmd(SPI3,ENABLE);
}

/*
*********************************************************************************************************
*                                        SPI3_WriteByte()
*
* Description : Write 1 byte of data to the SPI bus.    写1字节数据到SPI总线
*
* Arguments   : TxData  The data to write to the bus    要写入到总线的数据
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI3_WriteByte(uint8_t TxData)
{				 
	while((SPI3->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空		  
	SPI3->DR=TxData;	 	  									//发送一个byte 
	while((SPI3->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	SPI3->DR;
}

/*
*********************************************************************************************************
*                                        SPI3_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  从SPI总线读取1字节数据
*
* Arguments   : void
*
* Return      : uint8_t  Read data  读取到的数据
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI3_ReadByte(void)
{			 
	while((SPI3->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空			  
	SPI3->DR=0xFF;	 	  										//发送一个空数据产生输入数据的时钟 
	while((SPI3->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	return SPI3->DR;  						    
}

/*
*********************************************************************************************************
*                                        SPI_CrisEnter()
*
* Description : Into the critical area function.    进入临界区函数
*
* Arguments   : void
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI_CrisEnter(void)
{
	//OS_CPU_SR  cpu_sr;
	//__set_PRIMASK(1);
	//OS_ENTER_CRITICAL();
#if SYSTEM_SUPPORT_OS
    OS_ENTER_CRITICAL();
#else
    INTX_DISABLE();
#endif
}

/*
*********************************************************************************************************
*                                        SPI_CrisExit()
*
* Description : Exit the critical section function. 退出临界区函数
*
* Arguments   : void
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI_CrisExit(void)
{
	//OS_CPU_SR  cpu_sr;
	//__set_PRIMASK(0);
	//OS_EXIT_CRITICAL();
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();
#else
    INTX_ENABLE();
#endif
}



void SPI2_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
  	SPI_InitTypeDef  SPI_InitStructure;

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTB时钟使能 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2时钟使能 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15复用推挽输出 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15上拉

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//设置SPI工作模式:设置为主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//设置SPI的数据大小:SPI发送接收8位帧结构
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//串行同步时钟的空闲状态为高电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//串行同步时钟的第二个跳变沿（上升或下降）数据被采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//定义波特率预分频的值:波特率预分频值为256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRC值计算的多项式
	SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
 
	SPI_Cmd(SPI2, ENABLE); //使能SPI外设
	
	SPI2_ReadWriteByte(0xff);//启动传输		 
 

}   
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频   
//SPI_BaudRatePrescaler_8   8分频   
//SPI_BaudRatePrescaler_16  16分频  
//SPI_BaudRatePrescaler_256 256分频 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//设置SPI2速度 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//检查指定的SPI标志位设置与否:接受缓存非空标志位
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据					    
}

/*
*********************************************************************************************************
*                                        SPI3_WriteByte()
*
* Description : Write 1 byte of data to the SPI bus.    写1字节数据到SPI总线
*
* Arguments   : TxData  The data to write to the bus    要写入到总线的数据
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI2_WriteByte(uint8_t TxData)
{				 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空		  
	SPI2->DR=TxData;	 	  									//发送一个byte 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	SPI2->DR;
}

/*
*********************************************************************************************************
*                                        SPI3_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  从SPI总线读取1字节数据
*
* Arguments   : void
*
* Return      : uint8_t  Read data  读取到的数据
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI2_ReadByte(void)
{			 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//等待发送区空			  
	SPI2->DR=0xFF;	 	  										//发送一个空数据产生输入数据的时钟 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //等待接收完一个byte  
	return SPI2->DR;
}



