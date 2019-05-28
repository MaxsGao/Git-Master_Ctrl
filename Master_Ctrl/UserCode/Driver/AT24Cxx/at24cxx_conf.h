/*
*********************************************************************************************************
*
*
*                                     AT24Cxx EEPROM driver configuration
*                                         AT24Cxx EEPROM驱动配置程序
* File : at24cxx_conf.h
* By   : MaxsGao
* Date : 2019/03/18
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __AT24Cxx_CONF_H
#define   __AT24Cxx_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include <stdio.h>
#include <stdint.h>


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
/**
 * @brief Peer IP register address
 */
#ifndef  __IN__
#define  __IN__      //参数作为输入
#endif
#ifndef  __OUT_
#define  __OUT_      //参数作为输出
#endif


/**
 * @brief Define interface mode. \n
 * @todo Should select interface mode as chip. 
 *        - @ref \_WIZCHIP_IO_MODE_IIC_HARD_ \n
 *        - @ref \_WIZCHIP_IO_MODE_IIC_SOFT_ \n
 *        ex> <code> #define \_WIZCHIP_IO_MODE_IIC_HARD_ \_WIZCHIP_IO_MODE_IIC_SOFT_ </code>
 *       
 */
//#define _AT24CXX_IO_MODE_           _WIZCHIP_IO_MODE_IIC_HARD_
#define _AT24CXX_IO_MODE_           _WIZCHIP_IO_MODE_IIC_SOFT_

/**
 * @brief Select WIZCHIP.
 * @todo You should select one,AT24C01,AT24C02,AT24C04,AT24C08,AT24C16,AT24C32,AT24C64,AT24C128,AT24C256,AT24C512 or etc. \n\n
 */
#define _AT24CXX_                   AT24C128   // 128k Bit


/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
uint8_t AT24CXX_Init(void);


#ifdef __cplusplus
}
#endif

#endif
