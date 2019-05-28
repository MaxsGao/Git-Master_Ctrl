/*
*********************************************************************************************************
*
*
*                             Configure HT1261 segment code LCD driver
*                                         配置HT1261段码液晶驱动
* File : ht1621_conf.h
* By   : MaxsGao
* Date : 2019/03/12
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
#include "ht1621_conf.h"
#include "bsp_sys.h"
#include "bsp_delay.h"

#if SYSTEM_SUPPORT_OS
    #include "includes.h"   //支持OS时，使用	  
#endif


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


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
#define RCC_HT1621_DATA         RCC_APB2Periph_GPIOB
#define GPIO_Pin_HT1621_DATA    GPIO_Pin_12
#define GPIO_HT1621_DATA        GPIOB
#define Pout_HT1621_DATA        PBout(12)
#define Pin_HT1621_DATA         PBin(12)

#define RCC_HT1621_WR           RCC_APB2Periph_GPIOB
#define GPIO_Pin_HT1621_WR      GPIO_Pin_13
#define GPIO_HT1621_WR          GPIOB
#define Pout_HT1621_WR          PBout(13)

#define RCC_HT1621_RD           RCC_APB2Periph_GPIOB
#define GPIO_Pin_HT1621_RD      GPIO_Pin_14
#define GPIO_HT1621_RD          GPIOB
#define Pout_HT1621_RD          PBout(14)
    
#define RCC_HT1621_CS           RCC_APB2Periph_GPIOB
#define GPIO_Pin_HT1621_CS      GPIO_Pin_15
#define GPIO_HT1621_CS          GPIOB
#define Pout_HT1621_CS          PBout(15)


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_GPIO_Init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd( RCC_HT1621_DATA|RCC_HT1621_WR|RCC_HT1621_RD|RCC_HT1621_CS, ENABLE);

    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_HT1621_DATA;
    GPIO_Init(GPIO_HT1621_DATA, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_HT1621_WR;
    GPIO_Init(GPIO_HT1621_WR, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_HT1621_RD;
    GPIO_Init(GPIO_HT1621_RD, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_HT1621_CS;
    GPIO_Init(GPIO_HT1621_CS, &GPIO_InitStructure);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_CS_Select(void)
{
    Pout_HT1621_CS = 0;
//    GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_CS_Deselect(void)
{
    Pout_HT1621_CS = 1;
//    GPIO_SetBits(GPIOB,GPIO_Pin_15);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_WR_High(void)
{
    Pout_HT1621_WR = 1;
//    GPIO_SetBits(GPIOB,GPIO_Pin_13);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_WR_Low(void)
{
    Pout_HT1621_WR = 0;
//    GPIO_ResetBits(GPIOB,GPIO_Pin_13);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_RD_High(void)
{
    Pout_HT1621_RD = 1;
//    GPIO_SetBits(GPIOB,GPIO_Pin_14);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_RD_Low(void)
{
    Pout_HT1621_RD = 0;
//    GPIO_ResetBits(GPIOB,GPIO_Pin_14);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
uint8_t HT1621_DATA_In(void)
{
    return Pin_HT1621_DATA;
//    return PBin(12);
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval None
  */
void HT1621_DATA_Out(uint8_t data)
{
    Pout_HT1621_DATA = data;
//    PBout(12) = data;
}

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
void HT1621_Init(void)
{
    HT1621_GPIO_Init();
    
    reg_ht16xx_cs_cbfunc(HT1621_CS_Select,HT1621_CS_Deselect);
    reg_ht16xx_wr_cbfunc(HT1621_WR_High, HT1621_WR_Low);
    reg_ht16xx_rd_cbfunc(HT1621_RD_High, HT1621_RD_Low);
    reg_ht16xx_data_cbfunc(HT1621_DATA_In, HT1621_DATA_Out);
    reg_ht16xx_delayms_cbfunc(delay_ms);
    
    HT16xx_conf_func( 5, RC_256K, SYS_EN, BIAS_1_3, LCD_ON, TNORMAL );
    
//    HT16xx_test();
}

