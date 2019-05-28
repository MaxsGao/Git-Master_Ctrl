#include "bsp_adc.h"

const uint8_t gUserAdcChannelUsing[] = { ADC_CHANNEL_LIST_TO_ID };

//下面的宏定义用于判断 列表中数量 和 宏定义的数量相同
ACI_ASSERT_VAL_SAME( AdcChannelCnt , sizeof(gUserAdcChannelUsing) , ADC_TYPE_ID_USING_CNT );

AdcCjDataDma gUserAdcTransDatas;

/************************************************* 
*Function:		adc_dma_init_gpio
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  初始化 ADC 的IO口，需要用户自己修改初始化代码
*************************************************/
void adc_dma_init_gpio(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);     
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF,ENABLE);

//    //PA0 1 2 3 4 5 6 7作为模拟通道输入引脚
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0| GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //模拟输入引脚
//    GPIO_Init(GPIOA, &GPIO_InitStructure) ;

//    //PB0 1作为模拟通道输入引脚
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //模拟输入引脚
//    GPIO_Init(GPIOB, &GPIO_InitStructure) ;
//    
//    //PC0 1 2 3 4 5作为模拟通道输入引脚
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5 ;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN ; //模拟输入引脚
//    GPIO_Init(GPIOC, &GPIO_InitStructure) ;

    //PF6 7 8 9 10 作为模拟通道输入引脚
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AIN; //模拟输入引脚
    GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/************************************************* 
*Function:		adc_dma_init_and_start
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  使用DMA函数 初始化ADC转换功能
*           当前使用的逻辑为，开启转换后，ADC转换会一直运行。转换的数据
*           依次存入 传入的 数据地址中 需要最新的值，直接从内存中取即可
*************************************************/
void adc_dma_init_and_start(
    u16*        pAdcTansDesData ,   //ADC转换后的数据存储位置
    const uint8_t*    pAdcChannleList ,   //需要转换的 通道列表
    u8          nAdcChannleCnt  ,   //需要转换的 通道数量
    u8          nAdcSampleTime      //ADC_SampleTime_239Cycles5
    )
{
    DMA_InitTypeDef DMA_InitStructure;
    ADC_InitTypeDef ADC_InitStructure;
    u8 i;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);    /* Enable DMA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3 , ENABLE);    /* Enable ADC1 and GPIOC clock */

    ADC_DeInit(ADC3); //将外设 ADCx 的全部寄存器重设为缺省值

    ADC_InitStructure.ADC_Mode                  = ADC_Mode_Independent; //ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode          = ENABLE; //模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode    = ENABLE; //模数转换工作在连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv      = ADC_ExternalTrigConv_None; //外部触发转换关闭
    ADC_InitStructure.ADC_DataAlign             = ADC_DataAlign_Right; //ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel          = nAdcChannleCnt ; //顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC3, &ADC_InitStructure); //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADCx,ADC通道x,规则采样顺序值为y,采样时间为 nAdcSampleTime 周期  ADC_SampleTime_239Cycles5
    for( i=0 ; i<nAdcChannleCnt ; i++ )
    {
        ADC_RegularChannelConfig(ADC3, pAdcChannleList[i] , 1+i , nAdcSampleTime );
    }

    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC3, ENABLE);

    ADC_Cmd(ADC3, ENABLE); //使能指定的ADCx

    ADC_ResetCalibration(ADC3); //复位指定的ADCx的校准寄存器
    while(ADC_GetResetCalibrationStatus(ADC3)); //获取ADCx复位校准寄存器的状态,设置状态则等待
    ADC_StartCalibration(ADC3); //开始指定ADCx的校准状态
    while(ADC_GetCalibrationStatus(ADC3)); //获取指定ADCx的校准程序,设置状态则等待

    //////////////////////////////////////////////////////
    //ADC  DMA 设置
    DMA_DeInit(DMA2_Channel5); //将DMAx的通道x寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&ADC3->DR       ; //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = (u32)pAdcTansDesData ; //DMA内存基地址
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC; //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize            = nAdcChannleCnt ; //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable; //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable; //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_HalfWord; //数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_HalfWord   ; //数据宽度为16位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Circular; //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable; //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA2_Channel5, &DMA_InitStructure); //根据DMA_InitStruct中指定的参数初始化DMA的通道

    /* 允许DMA1通道1传输结束中断 */
    //DMA_ITConfig(DMA1_Channel1,DMA_IT_TC, ENABLE);   //当前不使用DMA1的中断，因此不使能中断

    //使能DMA通道
    DMA_Cmd(DMA2_Channel5, ENABLE);
    
    //ADCx转换启动
    ADC_SoftwareStartConvCmd(ADC3, ENABLE);
}

/************************************************* 
*Function:		user_adc_dma_init
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  用户使用ADC 的初始化
*************************************************/
void ADC_DMA_Init(void)
{
    adc_dma_init_gpio();
    adc_dma_init_and_start( gUserAdcTransDatas.cj16 , gUserAdcChannelUsing , ADC_TYPE_ID_USING_CNT , ADC_SampleTime_13Cycles5 );
}

