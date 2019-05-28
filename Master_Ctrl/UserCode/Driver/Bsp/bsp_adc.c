#include "bsp_adc.h"

const uint8_t gUserAdcChannelUsing[] = { ADC_CHANNEL_LIST_TO_ID };

//����ĺ궨�������ж� �б������� �� �궨���������ͬ
ACI_ASSERT_VAL_SAME( AdcChannelCnt , sizeof(gUserAdcChannelUsing) , ADC_TYPE_ID_USING_CNT );

AdcCjDataDma gUserAdcTransDatas;

/************************************************* 
*Function:		adc_dma_init_gpio
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ��ʼ�� ADC ��IO�ڣ���Ҫ�û��Լ��޸ĳ�ʼ������
*************************************************/
void adc_dma_init_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

//    //PA0 1 2 3 4 5 6 7��Ϊģ��ͨ����������
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //ģ����������
//    GPIO_Init(GPIOA, &GPIO_InitStructure) ;

//    //PB0 1��Ϊģ��ͨ����������
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //ģ����������
//    GPIO_Init(GPIOB, &GPIO_InitStructure) ;
//    
//    //PC0 1 2 3 4 5��Ϊģ��ͨ����������
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //ģ����������
//    GPIO_Init(GPIOC, &GPIO_InitStructure) ;

    //PF6 7 8 9 10 ��Ϊģ��ͨ����������
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN; //ģ����������
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/************************************************* 
*Function:		adc_dma_init_and_start
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ʹ��DMA���� ��ʼ��ADCת������
*           ��ǰʹ�õ��߼�Ϊ������ת����ADCת����һֱ���С�ת��������
*           ���δ��� ����� ���ݵ�ַ�� ��Ҫ���µ�ֵ��ֱ�Ӵ��ڴ���ȡ����
*************************************************/
void adc_dma_init_and_start(
    u16*        pAdcTansDesData ,   //ADCת��������ݴ洢λ��
    const uint8_t*    pAdcChannleList ,   //��Ҫת���� ͨ���б�
    u8          nAdcChannleCnt  ,   //��Ҫת���� ͨ������
    u8          nAdcSampleTime      //ADC_SampleTime_239Cycles5
    )
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    u8 i;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);    /* Enable DMA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 , ENABLE);    /* Enable ADC1 and GPIOC clock */

    ADC_DeInit(ADC3); //������ ADCx ��ȫ���Ĵ�������Ϊȱʡֵ

    ADC_InitStructure.ADC_Mode                  = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode          = ENABLE; //ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE; //ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_ExternalTrigConv_None; //�ⲿ����ת���ر�
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right; //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel          = nAdcChannleCnt ; //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC3, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADCx,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ nAdcSampleTime ����  ADC_SampleTime_239Cycles5
    for( i=0 ; i<nAdcChannleCnt ; i++ )
    {
        ADC_RegularChannelConfig(ADC3, pAdcChannleList[i] , 1+i , nAdcSampleTime );
    }

    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC3, ENABLE);

    ADC_Cmd(ADC3, ENABLE); //ʹ��ָ����ADCx

    ADC_ResetCalibration(ADC3); //��λָ����ADCx��У׼�Ĵ���
    while(ADC_GetResetCalibrationStatus(ADC3)); //��ȡADCx��λУ׼�Ĵ�����״̬,����״̬��ȴ�
    ADC_StartCalibration(ADC3); //��ʼָ��ADCx��У׼״̬
    while(ADC_GetCalibrationStatus(ADC3)); //��ȡָ��ADCx��У׼����,����״̬��ȴ�

    //////////////////////////////////////////////////////
    //ADC  DMA ����
    DMA_DeInit(DMA2_Channel5); //��DMAx��ͨ��x�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&ADC3->DR       ; //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = (u32)pAdcTansDesData ; //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize            = nAdcChannleCnt ; //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord   ; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular; //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA2_Channel5, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

    /* ����DMA1ͨ��1��������ж� */
    //DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);   //��ǰ��ʹ��DMA1���жϣ���˲�ʹ���ж�

    //ʹ��DMAͨ��
    DMA_Cmd(DMA2_Channel5, ENABLE);
    
    //ADCxת������
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/************************************************* 
*Function:		user_adc_dma_init
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  �û�ʹ��ADC �ĳ�ʼ��
*************************************************/
void ADC_DMA_Init(void)
{
    adc_dma_init_gpio();
    adc_dma_init_and_start( gUserAdcTransDatas.cj16 , gUserAdcChannelUsing , ADC_TYPE_ID_USING_CNT , ADC_SampleTime_13Cycles5 );
}

