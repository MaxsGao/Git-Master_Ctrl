/*
*********************************************************************************************************
*
*
*                                    Capacitive touch button's driver
*                                         ���ݴ���������������
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "bsp_sys.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
#ifndef  __IN__
#define  __IN__      //������Ϊ����
#endif
#ifndef  __OUT_
#define  __OUT_      //������Ϊ���
#endif
/**
 * @brief Peer IP register address
 */
  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
/** 
 * @brief	TPAD Driver structure definition
 */
typedef void		(*VoidFuncPtr)			(void);
typedef uint16_t	(*TimGetCapFuncPtr)		(TIM_TypeDef*);
typedef struct
{
    uint8_t             contin_en;                                  ///< �Ƿ�֧������
	uint16_t			default_val;								///< ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
	
	TIM_TypeDef* 		TIMx;                                       ///< ��Ӧ�ļ�ʱ��
	uint16_t 			TIM_FLAG;                                   ///< �жϱ�־
	
	TimGetCapFuncPtr	tim_getcapture_func;                        ///< ����������
	VoidFuncPtr			reset_func;                                 ///< ������λ����
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
*                                       FUNCTION STATEMENT  ��������
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






















