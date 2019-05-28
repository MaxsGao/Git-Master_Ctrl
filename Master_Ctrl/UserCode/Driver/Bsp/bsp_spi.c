/*
*********************************************************************************************************
*
*
*                                       SPI Driver Package
*                                         SPI���������
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "bsp_spi.h"

#if SYSTEM_SUPPORT_OS
    #include "os_cpu.h"
#endif


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
#if SYSTEM_SUPPORT_OS
    OS_CPU_SR   cpu_sr;
#endif


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        SPI1_Config()
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
void SPI1_Config(void)
{
	SPI_InitTypeDef     SPI_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA, ENABLE ); //PORTAʱ��ʹ��
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SPI1,  ENABLE ); //SPI1ʱ��ʹ��
    
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

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
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
* Description : Write 1 byte of data to the SPI bus.    д1�ֽ����ݵ�SPI����
*
* Arguments   : TxData  The data to write to the bus    Ҫд�뵽���ߵ�����
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI1_WriteByte(uint8_t TxData)
{				 
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������		  
	SPI1->DR=TxData;	 	  									//����һ��byte 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	SPI1->DR;
}

/*
*********************************************************************************************************
*                                        SPI1_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  ��SPI���߶�ȡ1�ֽ�����
*
* Arguments   : void
*
* Return      : uint8_t  Read data  ��ȡ��������
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI1_ReadByte(void)
{			 
	while((SPI1->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������			  
	SPI1->DR=0xFF;	 	  										//����һ�������ݲ����������ݵ�ʱ�� 
	while((SPI1->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	return SPI1->DR;  						    
}

/*
*********************************************************************************************************
*                                        SPI3_Config()
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
void SPI3_Config(void)
{
	SPI_InitTypeDef     SPI_InitStructure;
	GPIO_InitTypeDef    GPIO_InitStructure;
    
    RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE ); //PORTBʱ��ʹ��
    RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI3,  ENABLE ); //SPI3ʱ��ʹ��
    
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

//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_BaudRatePrescaler_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_BaudRatePrescaler_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_BaudRatePrescaler_256 256��Ƶ (SPI 281.25K@sys 72M)
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
* Description : Write 1 byte of data to the SPI bus.    д1�ֽ����ݵ�SPI����
*
* Arguments   : TxData  The data to write to the bus    Ҫд�뵽���ߵ�����
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI3_WriteByte(uint8_t TxData)
{				 
	while((SPI3->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������		  
	SPI3->DR=TxData;	 	  									//����һ��byte 
	while((SPI3->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	SPI3->DR;
}

/*
*********************************************************************************************************
*                                        SPI3_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  ��SPI���߶�ȡ1�ֽ�����
*
* Arguments   : void
*
* Return      : uint8_t  Read data  ��ȡ��������
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI3_ReadByte(void)
{			 
	while((SPI3->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������			  
	SPI3->DR=0xFF;	 	  										//����һ�������ݲ����������ݵ�ʱ�� 
	while((SPI3->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	return SPI3->DR;  						    
}

/*
*********************************************************************************************************
*                                        SPI_CrisEnter()
*
* Description : Into the critical area function.    �����ٽ�������
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
* Description : Exit the critical section function. �˳��ٽ�������
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

	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//PORTBʱ��ʹ�� 
	RCC_APB1PeriphClockCmd(	RCC_APB1Periph_SPI2,  ENABLE );//SPI2ʱ��ʹ�� 	
 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //PB13/14/15����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB

 	GPIO_SetBits(GPIOB,GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15);  //PB13/14/15����

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		//����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		//����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		//����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	//����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		//NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	//CRCֵ����Ķ���ʽ
	SPI_Init(SPI2, &SPI_InitStructure);  //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
	
	SPI2_ReadWriteByte(0xff);//��������		 
 

}   
//SPI �ٶ����ú���
//SpeedSet:
//SPI_BaudRatePrescaler_2   2��Ƶ   
//SPI_BaudRatePrescaler_8   8��Ƶ   
//SPI_BaudRatePrescaler_16  16��Ƶ  
//SPI_BaudRatePrescaler_256 256��Ƶ 
  
void SPI2_SetSpeed(u8 SPI_BaudRatePrescaler)
{
  	assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
	SPI2->CR1&=0XFFC7;
	SPI2->CR1|=SPI_BaudRatePrescaler;	//����SPI2�ٶ� 
	SPI_Cmd(SPI2,ENABLE); 

} 

//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI2_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 	
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}			  
	SPI_I2S_SendData(SPI2, TxData); //ͨ������SPIx����һ������
	retry=0;

	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET)//���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
		{
		retry++;
		if(retry>200)return 0;
		}	  						    
	return SPI_I2S_ReceiveData(SPI2); //����ͨ��SPIx������յ�����					    
}

/*
*********************************************************************************************************
*                                        SPI3_WriteByte()
*
* Description : Write 1 byte of data to the SPI bus.    д1�ֽ����ݵ�SPI����
*
* Arguments   : TxData  The data to write to the bus    Ҫд�뵽���ߵ�����
*
* Return      : void
*
*Note(s):
*********************************************************************************************************
*/
void SPI2_WriteByte(uint8_t TxData)
{				 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������		  
	SPI2->DR=TxData;	 	  									//����һ��byte 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	SPI2->DR;
}

/*
*********************************************************************************************************
*                                        SPI3_ReadByte()
*
* Description : Read 1 byte data from SPI bus.  ��SPI���߶�ȡ1�ֽ�����
*
* Arguments   : void
*
* Return      : uint8_t  Read data  ��ȡ��������
*
*Note(s):
*********************************************************************************************************
*/
uint8_t SPI2_ReadByte(void)
{			 
	while((SPI2->SR&SPI_I2S_FLAG_TXE)==0);	//�ȴ���������			  
	SPI2->DR=0xFF;	 	  										//����һ�������ݲ����������ݵ�ʱ�� 
	while((SPI2->SR&SPI_I2S_FLAG_RXNE)==0); //�ȴ�������һ��byte  
	return SPI2->DR;
}



