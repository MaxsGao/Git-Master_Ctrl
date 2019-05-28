/*
*********************************************************************************************************
*
*
*                                      Software IIC Driver
*                                          ģ��IIC
* File : bsp_I2Csoft.h
* By   : MaxsGao
* Date : 2019/4/10
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __BSP_I2C_SOFT_H
#define   __BSP_I2C_SOFT_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif

#define RCC_SCL             RCC_APB2Periph_GPIOB
#define GPIO_PORT_SCL	    GPIOB
#define GPIO_Pin_SCL	    GPIO_Pin_8

#define RCC_SDA				RCC_APB2Periph_GPIOB
#define GPIO_PORT_SDA       GPIOB
#define GPIO_Pin_SDA        GPIO_Pin_9

/**
 * @brief IO��������
 */
#define SDA_IN()  {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)8<<4;}
#define SDA_OUT() {GPIOB->CRH&=0XFFFFFF0F;GPIOB->CRH|=(u32)3<<4;}

/**
 * @brief IO��������
 */
#define IIC_SCL    PBout(8) //SCL
#define IIC_SDA    PBout(9) //SDA
#define READ_SDA   PBin(9)  //����SDA 
  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/

/** 
 * @brief  CAN init structure definition
 */
//typedef struct
//{
//  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum. 
//                                 It ranges from 1 to 1024. */
//  
//  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
//                                 This parameter can be a value of 
//                                @ref CAN_operating_mode */
//} CAN_InitTypeDef;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
void    IIC_Soft_Init       (void);
void    IIC_Soft_Start      (void);
void    IIC_Soft_Stop       (void);
uint8_t IIC_Soft_WaitAck    (void);
void    IIC_Soft_Ack        (void);
void    IIC_Soft_NoAck      (void);
void    IIC_Soft_WriteByte  (uint8_t byte);
uint8_t IIC_Soft_ReadByte   (uint8_t ack);


#ifdef __cplusplus
}
#endif

#endif
