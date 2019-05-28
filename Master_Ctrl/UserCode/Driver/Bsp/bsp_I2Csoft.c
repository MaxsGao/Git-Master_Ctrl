/*
*********************************************************************************************************
*
*
*                                      Software IIC Driver
*                                          模拟IIC
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "bsp_I2Csoft.h"
#include "bsp_delay.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
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
*                                       VARIABLES           变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
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
*                                       API FUNCTIONS       API接口函数
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
	RCC_APB2PeriphClockCmd(	RCC_SCL|RCC_SDA, ENABLE );      ///> 使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_SCL;
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP ;    ///> 推挽输出
	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
	GPIO_Init( GPIO_PORT_SCL, &GPIO_InitStructure );
	GPIO_SetBits( GPIO_PORT_SCL,GPIO_Pin_SCL ); 	        ///> 引脚拉高
    
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_SDA;
	GPIO_Init( GPIO_PORT_SDA, &GPIO_InitStructure );
	GPIO_SetBits( GPIO_PORT_SDA,GPIO_Pin_SDA ); 	        ///> 引脚拉高
}
void IIC_Soft_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA=1;	  	  
	IIC_SCL=1;
	delay_us(4);
 	IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL=0;//钳住I2C总线，准备发送或接收数据
}
void IIC_Soft_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL=0;
	IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL=1; 
	IIC_SDA=1;//发送I2C总线结束信号
	delay_us(4);
}
uint8_t IIC_Soft_WaitAck(void)
{
	u8 ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
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
	IIC_SCL=0;//时钟输出0 	   
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
    IIC_SCL=0;//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {
        //IIC_SDA=(txd&0x80)>>7;
		if((byte&0x80)>>7)
			IIC_SDA=1;
		else
			IIC_SDA=0;
		byte<<=1;
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL=1;
		delay_us(2);
		IIC_SCL=0;
		delay_us(2);
    }
}
uint8_t IIC_Soft_ReadByte(uint8_t ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
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
        IIC_Soft_NoAck();//发送nACK
    else
        IIC_Soft_Ack(); //发送ACK   
    return receive;
}

