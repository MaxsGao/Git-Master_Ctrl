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
#ifndef   __WIFISTA_H
#define   __WIFISTA_H

#ifdef __cplusplus
 extern "C" {
#endif


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "common.h"


/*
*********************************************************************************************************
*                                       CONSTANTS           ����
*********************************************************************************************************
*/
  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
/* ��ȡ stationģʽ������ ��� */
ESP8266_Error esp_wifista_getAvailableAPs ( __OUT_ char *pAPsMsg );
ESP8266_Error esp_wifista_getConnedAP     ( __OUT_ char *pAPMsg );
ESP8266_Error esp_wifista_getDHCP         ( void );
ESP8266_Error esp_wifista_getMAC          ( void );
ESP8266_Error esp_wifista_getIP           ( void );

/* ���� stationģʽ ��� */
ESP8266_Error esp_wifista_setConnAP     ( __IN__ const char *pSSID,__IN__ const char *pPWD );
ESP8266_Error esp_wifista_setDisconnAP  ( void );
ESP8266_Error esp_wifista_setAutoconnAP ( __IN__ const bool en );
ESP8266_Error esp_wifista_setDHCP       ( __IN__ const bool en );
ESP8266_Error esp_wifista_setMAC        ( __IN__ const char *pMAC );
ESP8266_Error esp_wifista_setIP         ( __IN__ const char *pIP,__IN__ const char *pGateway,__IN__ const char *pNetmask );
ESP8266_Error esp_wifista_startSMART    ( void );
ESP8266_Error esp_wifista_stopSMART     ( void );

#ifdef __cplusplus
}
#endif

#endif
