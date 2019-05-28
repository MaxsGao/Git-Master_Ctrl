#ifndef __BSP_ADC_H
#define __BSP_ADC_H	
#include "bsp_sys.h"

#define ACI_ASSERT_VAL_SAME(defName,x,y) typedef char defName ## _assert_size_t[-1+10*(x==y)]  //�ж���������ֵ�Ƿ����

#define ADC_TYPE_ID_USING_CNT 1

#define ADC_CHANNEL_LIST_TO_ID   ADC_Channel_5

//ADC���ݲɼ�������
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
*DESCRIPTION:  �û�ʹ��ADC �ĳ�ʼ��
*************************************************/
void ADC_DMA_Init(void);

#endif
