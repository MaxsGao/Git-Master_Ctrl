/*
*********************************************************************************************************
*
*
*                                      Software IIC Driver
*                                          ģ��IIC
* File : bsp_I2Csoft.c
* By   : MaxsGao
* Date : 2019/4/10
* version : V1.0
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
#include "bsp_I2Csoft.h"
#include "bsp_delay.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/

///** @defgroup SPI_Private_Defines
//  * @{
//  */

///* SPI SPE mask */
//#define CR1_SPE_Set          ((uint16_t)0x0040)
//#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

///* I2S I2SE mask */
//#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
//#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

///**
//  * @}
//  */


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
void IIC_Soft_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_SCL|RCC_SDA, ENABLE );      ///> ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_SCL;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP ;    ///> �������
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init( GPIO_PORT_SCL, &GPIO_InitStructure );
	GPIO_SetBits( GPIO_PORT_SCL,GPIO_Pin_SCL ); 	        ///> ��������
    
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_SDA;
	GPIO_Init( GPIO_PORT_SDA, &GPIO_InitStructure );
	GPIO_SetBits( GPIO_PORT_SDA,GPIO_Pin_SDA ); 	        ///> ��������
}
void IIC_Soft_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ��������
}
void IIC_Soft_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//����I2C���߽����ź�
	delay_us(4);
}
uint8_t IIC_Soft_WaitAck(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA=1;delay_us(1);	   
	IIC_SCL=1;delay_us(1);
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Soft_Stop();
			return 1;
		}
	}
	IIC_SCL=0;//ʱ�����0 	   
	return 0;
}
void IIC_Soft_Ack(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=0;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
void IIC_Soft_NoAck(void)
{
	IIC_SCL=0;
	SDA_OUT();
	IIC_SDA=1;
	delay_us(2);
	IIC_SCL=1;
	delay_us(2);
	IIC_SCL=0;
}
void IIC_Soft_WriteByte(uint8_t byte)
{
    u8 t;
	SDA_OUT();
    IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
		if((byte&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		byte<<=1;
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
    }
}
uint8_t IIC_Soft_ReadByte(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL=0; 
        delay_us(2);
		IIC_SCL=1;
        receive<<=1;
        if(READ_SDA)receive++;
		delay_us(1); 
    }					 
    if (!ack)
        IIC_Soft_NoAck();//����nACK
    else
        IIC_Soft_Ack(); //����ACK   
    return receive;
}

