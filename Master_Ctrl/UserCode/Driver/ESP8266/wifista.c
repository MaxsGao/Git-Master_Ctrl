/*
*********************************************************************************************************
*
*
*                                       Terminal mode configuration
*                                             �ն�ģʽ������
* File : wifista.h
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
#include "wifista.h"


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
  * @brief  �г���ǰ���õ�APs
  * @param  None
  * @retval 
  * @note   +CWLAP:(4,"ChinaNet-5ced",-85,"04:f0:21:15:60:f2",1,-11)
  *         +CWLAP:(4,"SH_ICMS",-67,"e8:65:d4:02:11:a8",3,-9)
  *
  *         OK
  */
ESP8266_Error esp_wifista_getAvailableAPs( __OUT_ char *pAPsMsg )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CWLAP );
    if( errorstatus==ESP8266_OK )
    {
        /* ���յ�OK��Ӧ����APs�б�Ӵ��ڽ��ջ������п������� */
    }
    
    return errorstatus;
}

/**
  * @brief  ��ѯ�����ӵ�AP��Ϣ
  * @param  None
  * @retval 
  * @note   +CWJAP:"chanpinkaifabu","20:6b:e7:d3:c1:f2",13,-41          No AP

  *         OK                                                          OK
  */
ESP8266_Error esp_wifista_getConnedAP( __OUT_ char *pAPMsg )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CWJAP );
    if( errorstatus == ESP8266_OK )
    {
        /* ���յ�OK��Ӧ���������ӵ�AP��Ϣ�Ӵ��ڻ������п������� */
    }
    
    return errorstatus;
}

/**
  * @brief  ���� ESP8266 station �����ӵ� AP
  * @param  None
  * @retval 
  * @note   WIFI CONNECTED          +CWJAP:1
  *         WIFI GOT IP
  *                                 FAIL
  *         OK
  */
ESP8266_Error esp_wifista_setConnAP( __IN__ const char *pSSID,__IN__ const char *pPWD )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 1000,setAT_CWJAP_DEF,pSSID,pPWD );
    if( errorstatus==ESP8266_OK )
    {
        /*  */
    }
    
    return errorstatus;
}

ESP8266_Error esp_wifista_setDisconnAP(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CWQAP );
    
    return errorstatus;
}

ESP8266_Error esp_wifista_setAutoconnAP( __IN__ const bool en )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CWAUTOCONN,en );
    
    return errorstatus;
}

/**
  * @brief  ��ѯ DHCP �Ƿ�ʹ��
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_getDHCP(void)
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
ESP8266_Error esp_wifista_setDHCP( __IN__ const bool en )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CWDHCP_DEF,1,en );
    
    return errorstatus;
}

/**
  * @brief  ��ѯ ESP8266 station �ӿڵ� MAC ��ַ
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_getMAC(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CIPSTAMAC );
    
    return errorstatus;
}

/**
  * @brief  ���� ESP8266 station �ӿ�Ĭ�� MAC ��ַ
  * @param  pMAC
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_setMAC( __IN__ const char *pMAC )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPSTAMAC_DEF,pMAC );
    
    return errorstatus;
}

/**
  * @brief  ��ѯ ESP8266 station �ӿڵ� IP ��ַ
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_getIP(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_queryCmd( 30,queAT_CIPSTA );
    
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
ESP8266_Error esp_wifista_setIP( 
    __IN__ const char *pIP,
    __IN__ const char *pGateway,
    __IN__ const char *pNetmask
    )
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPSTA_DEF,pIP,pGateway,pNetmask );
    
    return errorstatus;
}

/**
  * @brief  ����SmartConfig����
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_startSMART(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CWSTARTSMART );
    
    return errorstatus;
}

/**
  * @brief  ֹͣSmartConfig����
  * @param  None
  * @retval 
  * @note   
  */
ESP8266_Error esp_wifista_stopSMART(void)
{
    ESP8266_Error errorstatus=ESP8266_OK;
    
    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CWSTOPSMART );
    
    return errorstatus;
}

