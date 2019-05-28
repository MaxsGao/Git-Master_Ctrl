/*
*********************************************************************************************************
*
*
*                                    Capacitive touch button's driver
*                                         电容触摸按键驱动程序
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "tpad.h"
#include "bsp_delay.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
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
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#define TPAD_ARR_MAX_VAL              ((uint16_t)0XFFFF)          ///< 最大的ARR值

/*
*********************************************************************************************************
*                                       VARIABLES           变量
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
*                                       LOCAL FUNCTIONS     静态函数
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
//定时器2通道2输入捕获配置
void TIM5_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM5_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);            //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //使能PA端口时钟
        
	//设置GPIOA.1为浮空输入    
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_1;                   //PA1 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM5
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM5_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM5_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM5_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM5_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM5_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM5, &TIM5_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM5,ENABLE ); 	//使能定时器5
}


//定时器2通道2输入捕获配置
void TIM4_CH2_Cap_Init(u16 arr,u16 psc)
{
	GPIO_InitTypeDef            GPIO_InitStructure; 
   	TIM_TimeBaseInitTypeDef     TIM_TimeBaseStructure;
	TIM_ICInitTypeDef           TIM4_ICInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PA端口时钟
        
	//设置GPIOA.1为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;                   //PA1 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4
}

/**
  * @brief  触摸按键1的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);           //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //使能复用时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2,ENABLE);            //使能部分重映射
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);	    //禁止JTAG功能，把PA15、PB3作为普通IO口使用
        
	//设置GPIOA.15为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_15;                  //PA15 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM2_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM2_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM2_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
}

/**
  * @brief  触摸按键2的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PB端口时钟
        
	//设置GPIOB.3为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;                   //PB3 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM2
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM2_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM2_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM2_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM2_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM2_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM2,ENABLE ); 	//使能定时器2
}
    
/**
  * @brief  触摸按键3的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);            //使能复用时钟
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);             //使能TIM3的部分重映射
        
	//设置GPIOB.4为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;                   //PB4 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM3
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道1 
  	TIM3_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM3_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI;
  	TIM3_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
}
    
/**
  * @brief  触摸按键4的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);            //使能TIM2时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PB端口时钟
        
	//设置GPIOB.5为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;                   //PB5 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM3
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道1 
  	TIM3_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM3_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM3_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI;
  	TIM3_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM3_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM3, &TIM3_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM3,ENABLE ); 	//使能定时器2
}
    
/**
  * @brief  触摸按键5的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PA端口时钟
        
	//设置GPIOA.1为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;                   //PA1 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_1;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4
}
    
/**
  * @brief  触摸按键6的配置函数
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

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);            //使能TIM5时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);           //使能PA端口时钟
        
	//设置GPIOA.1为浮空输入
	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;                   //PA1 端口配置
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;             //速度50MHz
   	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;        //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);                          //设置为浮空输入

   	//初始化TIM4
	TIM_TimeBaseStructure.TIM_Period        = arr;                  //设定计数器自动重装值   
	TIM_TimeBaseStructure.TIM_Prescaler     = psc;                  //预分频器 	   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;         //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode   = TIM_CounterMode_Up;   //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);                 //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
    
	//初始化通道2 
  	TIM4_ICInitStructure.TIM_Channel        = TIM_Channel_2;        //CC1S=01 	选择输入端 IC2映射到TI5上
  	TIM4_ICInitStructure.TIM_ICPolarity     = TIM_ICPolarity_Rising;//上升沿捕获
  	TIM4_ICInitStructure.TIM_ICSelection    = TIM_ICSelection_DirectTI; 
  	TIM4_ICInitStructure.TIM_ICPrescaler    = TIM_ICPSC_DIV1;       //配置输入分频,不分频 
  	TIM4_ICInitStructure.TIM_ICFilter       = 0x03;                 //IC2F=0011 配置输入滤波器 8个定时器时钟周期滤波
  	TIM_ICInit(TIM4, &TIM4_ICInitStructure);                        //初始化I5 IC2

    TIM_Cmd(TIM4,ENABLE ); 	//使能定时器4
}

/**
  * @brief  触摸按键1的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD1_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
	
	//设置GPIOA.15为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_15;              //PA15 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOA, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOA,GPIO_Pin_15);						    //PA.15输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM2,0);		                                //归0
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC1|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOA.15为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
  * @brief  触摸按键2的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD2_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB端口时钟
	
	//设置GPIOB.3为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_3;               //PB3 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_3);						    //PB.3输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM2,0);		                                //归0
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOB.3为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  触摸按键3的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD3_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB端口时钟
	
	//设置GPIOB.4为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_4;               //PB4 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_4);						    //PB.4输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM3,0);		                                //归0
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC1|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOB.4为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  触摸按键4的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD4_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB端口时钟
	
	//设置GPIOB.5为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_5;               //PB5 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_5);						    //PB.5输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM3,0);		                                //归0
	TIM_ClearITPendingBit(TIM3, TIM_IT_CC2|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOB.5为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/**
  * @brief  触摸按键5的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD5_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB端口时钟
	
	//设置GPIOB.6为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_6;               //PB6 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_6);						    //PB.6输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM4,0);		                                //归0
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC1|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOB.6为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  触摸按键6的复位函数
  * @param	None
  * @note   None
  * @retval	None
  */
void TPAD6_Reset(void)
{
  	GPIO_InitTypeDef  GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	    //使能PB端口时钟
	
	//设置GPIOB.7为推挽使出
 	GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_7;               //PB7 端口配置
 	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_Out_PP;         //推挽输出
 	GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);
 	GPIO_ResetBits(GPIOB,GPIO_Pin_7);						    //PB.7输出0,放电

	delay_ms(1);

	TIM_SetCounter(TIM4,0);		                                //归0
	TIM_ClearITPendingBit(TIM4, TIM_IT_CC2|TIM_IT_Update);      //清除中断标志
    
	//设置GPIOB.7为浮空输入
	GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IN_FLOATING;    //浮空输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
  * @brief  得到定时器捕获值,如果超时,则直接返回定时器的计数值.
  * @param  n：连续获取的次数
  * @note   
  * @retval n次读数里面读到的最大读数值
  */
uint16_t tpad_get_val(
    TpadDriverCtrl* TPAD
)
{
    TPAD->reset_func();
    
    /* 等待捕获上升沿 */
    while ( TIM_GetFlagStatus( TPAD->TIMx,TPAD->TIM_FLAG ) ) {
        if ( TIM_GetCounter(TPAD->TIMx) > (TPAD_ARR_MAX_VAL-500) )      ///< 超时检测
            return TIM_GetCounter(TPAD->TIMx);                          ///< 若超时，直接返回CNT值
    }
    
    /* 得到一次值 */
    return TPAD->tim_getcapture_func( TPAD->TIMx );
}

/**
  * @brief  获取n次中的最大值
  * @param  n：连续获取的次数
  * @note   
  * @retval n次读数里面读到的最大读数值
  */
uint16_t tpad_get_maxval(
    TpadDriverCtrl* TPAD,
    uint8_t n
)
{
	uint16_t temp=0;
	uint16_t res=0;
    
    /* 循环获取n次时间值 */
	while(n--)
	{
        /* 得到一次值 */
        temp = tpad_get_val(TPAD);
        
        /* 取最大值 */
		if ( temp>res ) res=temp;
	};
	return res;
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  触摸按键通用配置，获得空载时触摸按键的取值
  * @param  TPAD
  *     - TPAD1，TPAD2，TPAD3，TPAD4，TPAD5，TPAD6
  * @note   None
  * @retval 0,初始化成功;1,初始化失败
  */
int TPAD_Config(
    TpadDriverCtrl* TPAD
)
{
    uint16_t arry[10];
    uint16_t temp;
    uint8_t i,j;
    
    /* 连续读取10次 */
	for ( i=0;i<10;i++ ) {
		arry[i] = tpad_get_val(TPAD);
		delay_ms(10);
	}
    
    /* 对10次数据进行排序操作,升序排列. */
	for ( i=0;i<9;i++ )	{
		for ( j=i+1;j<10;j++ ) {
			if ( arry[i]>arry[j] ) {
				temp=arry[i];
				arry[i]=arry[j];
				arry[j]=temp;
			}
		}
	}
    
    /* 取中间的6个数据进行平均 */
	temp=0;
	for ( i=2;i<8;i++ )
        temp+=arry[i];
    TPAD->default_val = temp/6;
    
    /* 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常! */
	if ( TPAD->default_val>TPAD_ARR_MAX_VAL/2 )
        return 1;
    
	return 0;
}

/**
  * @brief  初始化触摸按键
  * @param	None
  * @note   None
  * @retval	0,初始化成功;1,初始化失败
  */
void tpad_init(void)
{
    /* 按键硬件初始化 */
    TPAD1_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD2_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD3_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD4_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD5_Config( TPAD_ARR_MAX_VAL,6-1 );
    TPAD6_Config( TPAD_ARR_MAX_VAL,6-1 );
    
    /* 触摸按键通用配置，获得空载时触摸按键的取值 */
    TPAD_Config( TPAD1 );
    TPAD_Config( TPAD2 );
    TPAD_Config( TPAD3 );
    TPAD_Config( TPAD4 );
    TPAD_Config( TPAD5 );
    TPAD_Config( TPAD6 );
}

/**
  * @brief  触摸按键的扫描功能
  * @param	TPAD:
  * @note   None
  * @retval	None
  */
#define TPAD_GATE_VAL 	100	//触摸的门限值,也就是必须大于tpad_default_val+TPAD_GATE_VAL,才认为是有效触摸.
int tpad_scan(
    TpadDriverCtrl* TPAD,
    uint8_t mode
)
{ 
	uint8_t     res=0;          ///< 定义一个返回值变量
	uint8_t     sample=3;		///< 默认采样次数为3次	 
	uint16_t    rval;
    
    /* 判断是否支持连按 */
	if ( mode ) {
		sample=6;		        ///< 支持连按的时候，设置采样次数为6次
		TPAD->contin_en=0;      ///< 支持连按	  
	}
    
    rval = tpad_get_maxval( TPAD,sample );
    if ( rval>(TPAD->default_val+TPAD_GATE_VAL) ) {
        if ( TPAD->contin_en==0 ) res=1;
        TPAD->contin_en=3;
    }
    if ( TPAD->contin_en ) TPAD->contin_en--;
    return res;
}







