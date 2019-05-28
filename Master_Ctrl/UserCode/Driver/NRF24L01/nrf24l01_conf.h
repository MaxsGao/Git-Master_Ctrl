/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission ModuleConfig
*                                         无线数传模块配置
* File : nrf24l01_conf.h
* By   : 
* Date : 2018/12/6
* version : V0.1
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __NRF24L01_CONF_H
#define   __NRF24L01_CONF_H


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "nrf24l01.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
//#define   RQ_OPTION_WHEN_FULL_DISCARD_FIRST  0u       // discard the first element when ring buffer is full
#define RCC_NRF24L01_CE             RCC_APB2Periph_GPIOB
#define GPIO_NRF24L01_CE            GPIOB
#define GPIO_Pin_NRF24L01_CE        GPIO_Pin_1
#define Pout_NRF24L01_CE            PBout(1)

#define RCC_NRF24L01_CS             RCC_APB2Periph_GPIOB
#define GPIO_NRF24L01_CS            GPIOB
#define GPIO_Pin_NRF24L01_CS        GPIO_Pin_0
#define Pout_NRF24L01_CS            PBout(0)

#define RCC_NRF24L01_IRQ                RCC_APB2Periph_GPIOA
#define GPIO_NRF24L01_IRQ               GPIOA
#define GPIO_Pin_NRF24L01_IRQ           GPIO_Pin_4
#define GPIO_PortSource_NRF24L01_IRQ    GPIO_PortSourceGPIOA
#define GPIO_PinSource_NRF24L01_IRQ     GPIO_PinSource4
#define EXTI_Line_NRF24L01_IRQ          EXTI_Line4


/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
void NRF24L01_Config(
    _NRF_TX_QUEUE_UNIT* pTXUnitArray,
    uint16_t            array_size,
     NrfRxHandle        rx_handle
    );


#endif
