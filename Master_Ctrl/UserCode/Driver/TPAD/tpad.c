/*
*********************************************************************************************************
*
*
*                                    Capacitive touch button's driver
*                                         ���ݴ���������������
* File : tpad.c
* By   : MaxsGao
* Date : 2019/05/15
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
#include "tpad.h"
#include "bsp_delay.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/
void TPAD1_Reset(void);
void TPAD2_Reset(void);
void TPAD3_Reset(void);
void TPAD4_Reset(void);
void TPAD5_Reset(void);
void TPAD6_Reset(void);


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#define TPAD_ARR_MAX_VAL              ((uint16_t)0XFFFF)          ///< ����ARRֵ

/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/
TpadDriverCtrl DriverTpad1 = { 0, 0, TIM2, TIM_IT_CC1, TIM_GetCapture1, TPAD1_Reset };
TpadDriverCtrl DriverTpad2 = { 0, 0, TIM2, TIM_IT_CC2, TIM_GetCapture2, TPAD2_Reset };
TpadDriverCtrl DriverTpad3 = { 0, 0, TIM3, TIM_IT_CC1, TIM_GetCapture1, TPAD3_Reset };
TpadDriverCtrl DriverTpad4 = { 0, 0, TIM3, TIM_IT_CC2, TIM_GetCapture2, TPAD4_Reset };
TpadDriverCtrl DriverTpad5 = { 0, 0, TIM4, TIM_IT_CC1, TIM_GetCapture1, TPAD5_Reset };
TpadDriverCtrl DriverTpad6 = { 0, 0, TIM4, TIM_IT_CC2, TIM_GetCapture2, TPAD6_Reset };


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**
  * @brief  Clears the SPIx CRC Error (CRCERR) flag.
  * @param  SPIx: where x can be
  *   - 1, 2 or 3 in SPI mode 
  * @param  SPI_I2S_FLAG: specifies the SPI flag to clear. 
  *   This function clears only CRCERR flag.
  * @note
  *   - OVR (OverRun error) flag is cleared by software sequence: a read 
  *     operation to SPI_DR register (SPI_I2S_ReceiveData()) followed by a read
  * @retval None
  */
//��ʱ��2ͨ��2���벶������
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);            //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //ʹ��PA�˿�ʱ��
        
	//����GPIOA.1Ϊ��������    
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;                   //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM5
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM5_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM5_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM5_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM5,ENABLE ); 	//ʹ�ܶ�ʱ��5
}


//��ʱ��2ͨ��2���벶������
void TIM4_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM4_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PA�˿�ʱ��
        
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;                   //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
}

/**
  * @brief  ��������1�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM2_CH1,PA15
  * @retval	None
  */
void TPAD1_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //ʹ��PA�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //ʹ�ܸ���ʱ��
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);            //ʹ�ܲ�����ӳ��
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	    //��ֹJTAG���ܣ���PA15��PB3��Ϊ��ͨIO��ʹ��
        
	//����GPIOA.15Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_15;                  //PA15 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM2_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM2_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM2_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
}

/**
  * @brief  ��������2�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM2_CH2,PB3
  * @retval	None
  */
void TPAD2_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM2_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PB�˿�ʱ��
        
	//����GPIOB.3Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;                   //PB3 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM2
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM2_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM2_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM2_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM2_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
}
    
/**
  * @brief  ��������3�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM3_CH1,PB4
  * @retval	None
  */
void TPAD3_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            //ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PB�˿�ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //ʹ�ܸ���ʱ��
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);             //ʹ��TIM3�Ĳ�����ӳ��
        
	//����GPIOB.4Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;                   //PB4 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��1 
  	TIM3_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM3_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI;
  	TIM3_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��2
}
    
/**
  * @brief  ��������4�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM3_CH2,PB5
  * @retval	None
  */
void TPAD4_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM3_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            //ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PB�˿�ʱ��
        
	//����GPIOB.5Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;                   //PB5 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��1 
  	TIM3_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM3_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM3_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI;
  	TIM3_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM3_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��2
}
    
/**
  * @brief  ��������5�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM4_CH1,PB6
  * @retval	None
  */
void TPAD5_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM4_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PA�˿�ʱ��
        
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;                   //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
}
    
/**
  * @brief  ��������6�����ú���
  * @param	arr:
  * @param	psc:
  * @note   TIM4_CH2,PB7
  * @retval	None
  */
void TPAD6_Config(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM4_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //ʹ��TIM5ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //ʹ��PA�˿�ʱ��
        
	//����GPIOA.1Ϊ��������
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;                   //PA1 �˿�����
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //�ٶ�50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //����Ϊ��������

   	//��ʼ��TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //�趨�������Զ���װֵ   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //Ԥ��Ƶ�� 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    
	//��ʼ��ͨ��2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	ѡ������� IC2ӳ�䵽TI5��
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//�����ز���
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //���������Ƶ,����Ƶ 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 ���������˲��� 8����ʱ��ʱ�������˲�
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //��ʼ��I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//ʹ�ܶ�ʱ��4
}

/**
  * @brief  ��������1�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD1_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //ʹ��PA�˿�ʱ��
	
	//����GPIOA.15Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_15;              //PA15 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_15);						    //PA.15���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM2,0);		                                //��0
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOA.15Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  ��������2�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD2_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	
	//����GPIOB.3Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;               //PB3 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_3);						    //PB.3���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM2,0);		                                //��0
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOB.3Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  ��������3�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD3_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	
	//����GPIOB.4Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;               //PB4 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_4);						    //PB.4���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM3,0);		                                //��0
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOB.4Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  ��������4�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD4_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	
	//����GPIOB.5Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;               //PB5 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						    //PB.5���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM3,0);		                                //��0
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOB.5Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  ��������5�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD5_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	
	//����GPIOB.6Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;               //PB6 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_6);						    //PB.6���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM4,0);		                                //��0
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOB.6Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  ��������6�ĸ�λ����
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD6_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //ʹ��PB�˿�ʱ��
	
	//����GPIOB.7Ϊ����ʹ��
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;               //PB7 �˿�����
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //�������
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_7);						    //PB.7���0,�ŵ�

	delay_ms(1);

	TIM_SetCounter(TIM4,0);		                                //��0
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update);      //����жϱ�־
    
	//����GPIOB.7Ϊ��������
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  �õ���ʱ������ֵ,�����ʱ,��ֱ�ӷ��ض�ʱ���ļ���ֵ.
  * @param  n��������ȡ�Ĵ���
  * @note   
  * @retval n�ζ������������������ֵ
  */
uint16_t tpad_get_val(
    TpadDriverCtrl* TPAD
)
{
    TPAD->reset_func();
    
    /* �ȴ����������� */
    while ( TIM_GetFlagStatus( TPAD->TIMx,TPAD->TIM_FLAG ) ) {
        if ( TIM_GetCounter(TPAD->TIMx) > (TPAD_ARR_MAX_VAL-500) )      ///< ��ʱ���
            return TIM_GetCounter(TPAD->TIMx);                          ///< ����ʱ��ֱ�ӷ���CNTֵ
    }
    
    /* �õ�һ��ֵ */
    return TPAD->tim_getcapture_func( TPAD->TIMx );
}

/**
  * @brief  ��ȡn���е����ֵ
  * @param  n��������ȡ�Ĵ���
  * @note   
  * @retval n�ζ������������������ֵ
  */
uint16_t tpad_get_maxval(
    TpadDriverCtrl* TPAD,
    uint8_t n
)
{
	uint16_t temp=0;
	uint16_t res=0;
    
    /* ѭ����ȡn��ʱ��ֵ */
	while(n--)
	{
        /* �õ�һ��ֵ */
        temp = tpad_get_val(TPAD);
        
        /* ȡ���ֵ */
		if ( temp>res ) res=temp;
	};
	return res;
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/**
  * @brief  ��������ͨ�����ã���ÿ���ʱ����������ȡֵ
  * @param  TPAD
  *     - TPAD1��TPAD2��TPAD3��TPAD4��TPAD5��TPAD6
  * @note   None
  * @retval 0,��ʼ���ɹ�;1,��ʼ��ʧ��
  */
int TPAD_Config(
    TpadDriverCtrl* TPAD
)
{
    uint16_t arry[10];
    uint16_t temp;
    uint8_t i,j;
    
    /* ������ȡ10�� */
	for ( i=0;i<10;i++ ) {
		arry[i] = tpad_get_val(TPAD);
		delay_ms(10);
	}
    
    /* ��10�����ݽ����������,��������. */
	for ( i=0;i<9;i++ )	{
		for ( j=i+1;j<10;j++ ) {
			if ( arry[i]>arry[j] ) {
				temp=arry[i];
				arry[i]=arry[j];
				arry[j]=temp;
			}
		}
	}
    
    /* ȡ�м��6�����ݽ���ƽ�� */
	temp=0;
	for ( i=2;i<8;i++ )
        temp+=arry[i];
    TPAD->default_val = temp/6;
    
    /* ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������! */
	if ( TPAD->default_val>TPAD_ARR_MAX_VAL/2 )
        return 1;
    
	return 0;
}

/**
  * @brief  ��ʼ����������
  * @param	None
  * @note   None
  * @retval	0,��ʼ���ɹ�;1,��ʼ��ʧ��
  */
void tpad_init(void)
{
    /* ����Ӳ����ʼ�� */
    TPAD1_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD2_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD3_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD4_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD5_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD6_Config( TPAD_ARR_MAX_VAL,6-1 );
    
    /* ��������ͨ�����ã���ÿ���ʱ����������ȡֵ */
    TPAD_Config( TPAD1 );
    TPAD_Config( TPAD2 );
    TPAD_Config( TPAD3 );
    TPAD_Config( TPAD4 );
    TPAD_Config( TPAD5 );
    TPAD_Config( TPAD6 );
}

/**
  * @brief  ����������ɨ�蹦��
  * @param	TPAD:
  * @note   None
  * @retval	None
  */
#define TPAD_GATE_VAL 	100	//����������ֵ,Ҳ���Ǳ������tpad_default_val+TPAD_GATE_VAL,����Ϊ����Ч����.
int tpad_scan(
    TpadDriverCtrl* TPAD,
    uint8_t mode
)
{ 
	uint8_t     res=0;          ///< ����һ������ֵ����
	uint8_t     sample=3;		///< Ĭ�ϲ�������Ϊ3��	 
	uint16_t    rval;
    
    /* �ж��Ƿ�֧������ */
	if ( mode ) {
		sample=6;		        ///< ֧��������ʱ�����ò�������Ϊ6��
		TPAD->contin_en=0;      ///< ֧������	  
	}
    
    rval = tpad_get_maxval( TPAD,sample );
    if ( rval>(TPAD->default_val+TPAD_GATE_VAL) ) {
        if ( TPAD->contin_en==0 ) res=1;
        TPAD->contin_en=3;
    }
    if ( TPAD->contin_en ) TPAD->contin_en--;
    return res;
}







