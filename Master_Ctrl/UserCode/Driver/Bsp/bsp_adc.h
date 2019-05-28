#ifndef __BSP_ADC_H
#define __BSP_ADC_H	
#include "bsp_sys.h"

#define ACI_ASSERT_VAL_SAME(defName,x,y) typedef char defName ## _assert_size_t[-1+10*(x==y)]  //判断两个整数值是否相等

#define ADC_TYPE_ID_USING_CNT 1

#define ADC_CHANNEL_LIST_TO_ID   ADC_Channel_5

//ADC数据采集缓冲区
typedef union
{
    uint16_t cj16[ADC_TYPE_ID_USING_CNT];
    //uint32_t cj32[ADC_TYPE_ID_USING_CNT/2];
}AdcCjDataDma;
extern AdcCjDataDma gUserAdcTransDatas;

/************************************************* 
*Function:		user_adc_dma_init
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  用户使用ADC 的初始化
*************************************************/
void ADC_DMA_Init(void);

#endif
