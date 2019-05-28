/*
*********************************************************************************************************
*
*
*                                       Routing mode configuration
*                                            ·��ģʽ������
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "wifiap.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           ����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/**
  * @brief  ��ѯ ESP8266 softAP ����
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
  * @brief  ���� ESP8266 softAP ���������浽 flash
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
  * @brief  ��ѯ���ӵ� ESP8266 softAP �� stations ��Ϣ
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
  * @brief  ��ѯ DHCP �Ƿ�ʹ��
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
  * @brief  ���� DHCP �����浽 Flash
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
  * @brief  ��ѯ ESP8266 softAP �� MAC ��ַ
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
  * @brief  ���� ESP8266 softAP Ĭ�� MAC ��ַ
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
  * @brief  ��ѯ ESP8266 softAP �� IP ��ַ
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
  * @brief  ���� ESP8266 softAP Ĭ�� IP ��ַ
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

