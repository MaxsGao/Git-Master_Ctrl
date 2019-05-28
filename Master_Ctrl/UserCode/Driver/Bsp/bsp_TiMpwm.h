#ifndef __BSP_TIMPWM_H
#define __BSP_TIMPWM_H
#include "bsp_sys.h"

//////////////////////////////////////////////////////////////////////////
//定时器  PWM输出控制初始化 这部分功能需要用户根据实际情况自定义设计
void PWM_Init_Tim8_CH3_CH4(u16 arr,u16 psc);
void PWM_Init_Tim3_CH1_CH2_CH3(u16 arr,u16 psc);

//使用下面的 函数，可以增加代码执行效率，不需要执行参数是否正确的判断
static __inline void SetPwm_Tim8_CH3(uint16_t Compare3)
{
    //TIM_SetCompare3( TIM8 , Compare3 );
    TIM8->CCR3 = Compare3;
}
static __inline void SetPwm_Tim8_CH4(uint16_t Compare4)
{
    //TIM_SetCompare4( TIM8 , Compare4 );
    TIM8->CCR4 = Compare4;
}
static __inline void SetPwm_Tim3_CH1(uint16_t Compare3)
{
    //TIM_SetCompare3( TIM8 , Compare3 );
    TIM3->CCR1 = Compare3;
}
static __inline void SetPwm_Tim3_CH2(uint16_t Compare4)
{
    //TIM_SetCompare4( TIM8 , Compare4 );
    TIM3->CCR2 = Compare4;
}
static __inline void SetPwm_Tim3_CH3(uint16_t Compare4)
{
    //TIM_SetCompare4( TIM8 , Compare4 );
    TIM3->CCR3 = Compare4;
}
#endif
