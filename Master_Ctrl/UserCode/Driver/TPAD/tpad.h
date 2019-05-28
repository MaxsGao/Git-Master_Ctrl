/*
*********************************************************************************************************
*
*
*                                    Capacitive touch button's driver
*                                         电容触摸按键驱动程序
* File : tpad.h
* By   : MaxsGao
* Date : 2019/05/15
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __TPAD_H
#define   __TPAD_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //参数作为输入
#endif
#ifndef  __OUT_
#define  __OUT_      //参数作为输出
#endif
/**
 * @brief Peer IP register address
 */
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
/** 
 * @brief	TPAD Driver structure definition
 */
typedef void		(*VoidFuncPtr)			(void);
typedef uint16_t	(*TimGetCapFuncPtr)		(TIM_TypeDef*);
typedef struct
{
    uint8_t             contin_en;                                  ///< 是否支持连按
	uint16_t			default_val;								///< 空载的时候(没有手按下),计数器需要的时间
	
	TIM_TypeDef* 		TIMx;                                       ///< 对应的计时器
	uint16_t 			TIM_FLAG;                                   ///< 中断标志
	
	TimGetCapFuncPtr	tim_getcapture_func;                        ///< 计数捕获函数
	VoidFuncPtr			reset_func;                                 ///< 按键复位函数
} TpadDriverCtrl;
extern TpadDriverCtrl DriverTpad1;
extern TpadDriverCtrl DriverTpad2;
extern TpadDriverCtrl DriverTpad3;
extern TpadDriverCtrl DriverTpad4;
extern TpadDriverCtrl DriverTpad5;
extern TpadDriverCtrl DriverTpad6;

#define TPAD1	&DriverTpad1
#define TPAD2	&DriverTpad2
#define TPAD3	&DriverTpad3
#define TPAD4	&DriverTpad4
#define TPAD5	&DriverTpad5
#define TPAD6	&DriverTpad6


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
void TPAD_Reset(void);
u8   TPAD_Init(u8 psc);
u8   TPAD_Scan(u8 mode);

void	tpad_init	( void );
int 	tpad_scan	( TpadDriverCtrl* TPAD,uint8_t mode );


#ifdef __cplusplus
}
#endif

#endif






















