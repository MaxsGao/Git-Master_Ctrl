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


/*
*********************************************************************************************************
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "wifiap.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  查询 ESP8266 softAP 参数
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_getCONF(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CWSAP );
    
    return errorstatus;
}

/**
  * @brief  配置 ESP8266 softAP 参数，保存到 flash
  * @param  pSSID
  * @param  pPWD
  * @param  chl
  * @param  ecn
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_setCONF( 
    __IN__ const char *pSSID,
    __IN__ const char*pPWD,
    __IN__ const int chl,
    __IN__ const ECNMode ecn
    )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CWSAP_DEF,pSSID,pPWD,chl,ecn,4 );
    
    return errorstatus;
}

/**
  * @brief  查询连接到 ESP8266 softAP 的 stations 信息
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_getSTA(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CWLIF );
    
    return errorstatus;
}

/**
  * @brief  查询 DHCP 是否使能
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_getDHCP(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CWDHCP );
    
    return errorstatus;
}

/**
  * @brief  设置 DHCP 并保存到 Flash
  * @param  en
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_setDHCP( __IN__ const bool en )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CWDHCP_DEF,0,en );
    
    return errorstatus;
}

/**
  * @brief  查询 ESP8266 softAP 的 MAC 地址
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_getMAC(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CIPAPMAC );
    
    return errorstatus;
}

/**
  * @brief  设置 ESP8266 softAP 默认 MAC 地址
  * @param  pMAC
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_setMAC( __IN__ const char *pMAC )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPAPMAC_DEF,pMAC );
    
    return errorstatus;
}

/**
  * @brief  查询 ESP8266 softAP 的 IP 地址
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_getIP(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CIPAP );
    
    return errorstatus;
}

/**
  * @brief  设置 ESP8266 softAP 默认 IP 地址
  * @param  pIP
  * @param  pGateway
  * @param  pNetmask
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifiap_setIP( 
    __IN__ const char *pIP,
    __IN__ const char *pGateway,
    __IN__ const char *pNetmask
    )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPAP_DEF,pIP,pGateway,pNetmask );
    
    return errorstatus;
}

