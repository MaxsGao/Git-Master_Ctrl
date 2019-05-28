/*
*********************************************************************************************************
*
*
*                                     EEPROM file system configuration
*                                         EEPROM文件系统配置
* File : efconf.h
* By   : MaxsGao
* Date : 2019/03/22
* version : V1.0
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __EFCONF_H
#define   __EFCONF_H

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
     
#include "at24cxx.h"


     
/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
#define _E2P_VOLUMES	        2
     
#define _E2P_TYPE_      _AT24CXX_
#define _E2P_TOAL_SIZE_ _AT24Cxx_Toal_SIZE_
#define _E2P_PAGE_SIZE_ _AT24Cxx_Page_SIZE_
#define _E2P_PAGE_NUM_  _AT24Cxx_Page_NUM_

#define _E2P_MBR_BASE_PAGE_     (uint16_t)0   /* 文件引导区起始页面 */
#define _E2P_MBR_PAGE_NUM_      2   /* 文件引导区总页数 */

#define _E2P_DIR_BASE_PAGE_     (uint16_t)(_E2P_MBR_BASE_PAGE_+_E2P_MBR_PAGE_NUM_)  /* 文件目录区起始页面 */
#define _E2P_DIR_PAGE_NUM_      16                                      /* 文件目录区总页数 */
#define _E2P_DIR_UNIT_SIZE_     8

#define _E2P_FIL_BASE_PAGE_     (uint16_t)(_E2P_DIR_BASE_PAGE_+_E2P_DIR_PAGE_NUM_)  /* 文件页面分配区起始页面 */
#define _E2P_FIL_PAGE_NUM_      7                                       /* 文件页面分配区总页数 */
#define _E2P_FIL_UNIT_SIZE_     2

#define _E2P_DAT_BASE_PAGE_     (uint16_t)(_E2P_FIL_BASE_PAGE_+_E2P_FIL_PAGE_NUM_)  /* 文件数据区起始页面 */
#define _E2P_DAT_PAGE_NUM_      224                                     /* 文件数据区总页数 */


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



#ifdef __cplusplus
}
#endif

#endif
