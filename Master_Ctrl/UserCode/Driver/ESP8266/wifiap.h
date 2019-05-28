/*
*********************************************************************************************************
*
*
*                                       Routing mode configuration
*                                            路由模式的配置
* File : wifiap.h
* By   : 
* Date : 2019/01/15
* version : V0.1 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __WIFIAP_H
#define   __WIFIAP_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "common.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
  

/*
*********************************************************************************************************
*                                       DATA TYPE           数据类型
*********************************************************************************************************
*/
/** 
 * @brief   softAP 模式下的加密方式
 */
typedef enum
{
    OPEN            = 0x00,
    WPA_PSK         = 0x02,
    WPA2_PSK        = 0x03,
    WPA_WPA2_PSK    = 0x04,
}ECNMode;
#define IS_ESPSTATUS(STATUS) ( ((MODE) == online) || ((MODE) == offline) || ((MODE) == busy) || ((MODE) == sleep) )


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  函数声明
*********************************************************************************************************
*/
/* 获取 softAP 相关 */
ESP8266_Error esp_wifiap_getCONF ( void );
ESP8266_Error esp_wifiap_getSTA  ( void );
ESP8266_Error esp_wifiap_getDHCP ( void );
ESP8266_Error esp_wifiap_getMAC  ( void );
ESP8266_Error esp_wifiap_getIP   ( void );

/* 配置 softAP 相关 */
ESP8266_Error esp_wifiap_setCONF ( __IN__ const char *pSSID,__IN__ const char*pPWD,__IN__ const int chl,__IN__ const ECNMode ecn );
ESP8266_Error esp_wifiap_setDHCP ( __IN__ const bool en );
ESP8266_Error esp_wifiap_setMAC  ( __IN__ const char *pMAC );
ESP8266_Error esp_wifiap_setIP   ( __IN__ const char *pIP,__IN__ const char *pGateway,__IN__ const char *pNetmask );


#ifdef __cplusplus
}
#endif

#endif
