/*
*********************************************************************************************************
*
*
*                                       ESP8266 Module's Generic Function Interface
*                                         ESP8266ģ���ͨ�ú����ӿ�
* File : common.h
* By   : 
* Date : 2019/01/11
* version : V0.1 
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __COMMON_H
#define   __COMMON_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>


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

#ifndef ON
    #define ON  1
#endif

#ifndef OFF
    #define OFF 0
#endif

/* Uncomment the line below to expanse the "assert_param" macro in the 
   Standard Peripheral Library drivers code */
/* #define USE_FULL_ASSERT    1 */
/* Exported macro ------------------------------------------------------------*/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  The assert_param macro is used for function's parameters check.
  * @param  expr: If expr is false, it calls assert_failed function which reports 
  *         the name of the source file and the source line number of the call 
  *         that failed. If expr is true, it returns no value.
  * @retval None
  */
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
/* Exported functions ------------------------------------------------------- */
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

/**
 * @brief ����ATָ��
 */
#define AT                          "AT"
#define ATE0                        "ATE0"
  
#define ATE1                        "ATE1"
  
#define exeAT_RST                   "AT+RST"
#define exeAT_GMR                   "AT+GMR"
#define setAT_GSLP                  "AT+GSLP=%d"
#define exeAT_RESTORE               "AT+RESTORE"
  
#define setAT_UART                  "AT+UART=%d,%d,%d,%d,%d"
#define setAT_UART_CUR              "AT+UART=%d,%d,%d,%d,%d"
#define setAT_UART_DEF              "AT+UART=%d,%d,%d,%d,%d"
  
#define queAT_SLEEP                 "AT+SLEEP"
#define setAT_SLEEP                 "AT+SLEEP"

#define setAT_RFPOWER               "AT+RFPOWER=%d"

#define queAT_RFVDD                 "AT+RFVDD"
#define setAT_RFVDD                 "AT+RFVDD=%d"
#define exeAT_RFVDD                 "AT+RFVDD"

/**
 * @brief WiFi����ATָ��
 */
#define tesAT_CWMODE                "AT+CWMODE"
#define queAT_CWMODE                "AT+CWMODE"
#define setAT_CWMODE                "AT+CWMODE=%d"
#define tesAT_CWMODE_CUR            "AT+CWMODE_CUR"
#define queAT_CWMODE_CUR            "AT+CWMODE_CUR"
#define setAT_CWMODE_CUR            "AT+CWMODE_CUR=%d"
#define tesAT_CWMODE_DEF            "AT+CWMODE_DEF"
#define queAT_CWMODE_DEF            "AT+CWMODE_DEF"
#define setAT_CWMODE_DEF            "AT+CWMODE_DEF=%d"

#define queAT_CWJAP                 "AT+CWJAP"
#define setAT_CWJAP                 "AT+CWJAP=\"%s\",\"%s\""
#define queAT_CWJAP_CUR             "AT+CWJAP_CUR"
#define setAT_CWJAP_CUR             "AT+CWJAP_CUR=\"%s\",\"%s\""
#define queAT_CWJAP_DEF             "AT+CWJAP_DEF"
#define setAT_CWJAP_DEF             "AT+CWJAP_DEF=\"%s\",\"%s\""

#define exeAT_CWLAP                 "AT+CWLAP"
#define exeAT_CWQAP                 "AT+CWQAP"
#define queAT_CWSAP                 "AT+CWSAP"
#define setAT_CWSAP                 "AT+CWSAP=\"%s\",\"%s\",%d,%d,%d"
#define queAT_CWSAP_CUR             "AT+CWSAP_CUR"
#define setAT_CWSAP_CUR             "AT+CWSAP_CUR=\"%s\",\"%s\",%d,%d,%d"
#define queAT_CWSAP_DEF             "AT+CWSAP_DEF"
#define setAT_CWSAP_DEF             "AT+CWSAP_DEF=\"%s\",\"%s\",%d,%d,%d"

#define exeAT_CWLIF                 "AT+CWLIF"

#define queAT_CWDHCP                "AT+CWDHCP"
#define setAT_CWDHCP                "AT+CWDHCP=%d,%d"
#define queAT_CWDHCP_CUR            "AT+CWDHCP_CUR"
#define setAT_CWDHCP_CUR            "AT+CWDHCP_CUR=%d,%d"
#define queAT_CWDHCP_DEF            "AT+CWDHCP_DEF"
#define setAT_CWDHCP_DEF            "AT+CWDHCP_DEF=%d,%d"

#define setAT_CWAUTOCONN            "AT+CWAUTOCONN=%d"

#define queAT_CIPSTAMAC             "AT+CIPSTAMAC"
#define setAT_CIPSTAMAC             "AT+CIPSTAMAC=\"%s\""
#define queAT_CIPSTAMAC_CUR         "AT+CIPSTAMAC_CUR"
#define setAT_CIPSTAMAC_CUR         "AT+CIPSTAMAC_CUR=\"%s\""
#define queAT_CIPSTAMAC_DEF         "AT+CIPSTAMAC_DEF"
#define setAT_CIPSTAMAC_DEF         "AT+CIPSTAMAC_DEF=\"%s\""

#define queAT_CIPAPMAC              "AT+CIPAPMAC"
#define setAT_CIPAPMAC              "AT+CIPAPMAC=\"%s\""
#define queAT_CIPAPMAC_CUR          "AT+CIPAPMAC_CUR"
#define setAT_CIPAPMAC_CUR          "AT+CIPAPMAC_CUR=\"%s\""
#define queAT_CIPAPMAC_DEF          "AT+CIPAPMAC_DEF"
#define setAT_CIPAPMAC_DEF          "AT+CIPAPMAC_DEF=\"%s\""

#define queAT_CIPSTA                "AT+CIPSTA"
#define setAT_CIPSTA                "AT+CIPST\"%s\""
#define queAT_CIPSTA_CUR            "AT+CIPSTA_CUR"
#define setAT_CIPSTA_CUR            "AT+CIPSTA_CUR=\"%s\""
#define queAT_CIPSTA_DEF            "AT+CIPSTA_DEF"
#define setAT_CIPSTA_DEF            "AT+CIPSTA_DEF=\"%s\""

#define queAT_CIPAP                 "AT+CIPAP"
#define setAT_CIPAP                 "AT+CIPAP=\"%s\""
#define queAT_CIPAP_CUR             "AT+CIPAP_CUR"
#define setAT_CIPAP_CUR             "AT+CIPAP_CUR=\"%s\""
#define queAT_CIPAP_DEF             "AT+CIPAP_DEF"
#define setAT_CIPAP_DEF             "AT+CIPAP_DEF=\"%s\""

#define exeAT_CWSTARTSMART          "AT+CWSTARTSMART"
#define exeAT_CWSTOPSMART           "AT+CWSTOPSMART"

/**
 * @brief TCP/IP���ATָ��
 */
#define exeAT_CIPSTATUS             "AT+CIPSTATUS"

#define setAT_CIPSTART0             "AT+CIPSTART=\"%s\",\"%s\",%d"
#define setAT_CIPSTART_TCP          "AT+CIPSTART=%d,\"%s\",\"%s\",%d"
#define setAT_CIPSTART_UDP          "AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d"

#define setAT_CIPSEND0              "AT+CIPSEND=%d"
#define setAT_CIPSEND1              "AT+CIPSEND=%d,%d"
#define setAT_CIPSEND_UDP           "AT+CIPSEND=%d,%d,\"%s\",%d"
#define exeAT_CIPSEND               "AT+CIPSEND"

#define setAT_CIPSENDEX0            "AT+CIPSENDEX=%d"
#define setAT_CIPSENDEX1            "AT+CIPSENDEX=%d,%d"
#define setAT_CIPSENDEX_UDP         "AT+CIPSENDEX=%d,%d,\"%s\",%d"

#define setAT_CIPSENDBUF0           "AT+CIPSENDBUF=%d"
#define setAT_CIPSENDBUF1           "AT+CIPSENDBUF=%d,%d"

#define exeAT_CIPBUFRESET0          "AT+CIPBUFRESET"
#define setAT_CIPBUFRESET1          "AT+CIPBUFRESET=%d"

#define exeAT_CIPBUFSTATUS          "AT+CIPBUFSTATUS"
#define setAT_CIPBUFSTATUS          "AT+CIPBUFSTATUS=%d"

#define setAT_CIPCHECKSEQ0          "AT+CIPCHECKSEQ=%d"
#define setAT_CIPCHECKSEQ1          "AT+CIPCHECKSEQ=%d,%d"

#define setAT_CIPCLOSE              "AT+CIPCLOSE=%d"
#define exeAT_CIPCLOSE              "AT+CIPCLOSE"

#define exeAT_CIFSR                 "AT+CIFSR"

#define queAT_CIPMUX                "AT+CIPMUX"
#define setAT_CIPMUX                "AT+CIPMUX=%d"

#define setAT_CIPSERVER0            "AT+CIPSERVER=%d"
#define setAT_CIPSERVER1            "AT+CIPSERVER=%d,%d"

#define queAT_CIPMODE               "AT+CIPMODE"
#define setAT_CIPMODE               "AT+CIPMODE=%d"

#define setAT_SAVETRANSLINK         "AT+SAVETRANSLINK=%d,\"%s\",%d,\"%s\""

#define queAT_CIPSTO                "AT+CIPSTO"
#define setAT_CIPSTO                "AT+CIPSTO=%d"

#define exeAT_CIUPDATE              "AT+CIUPDATE"

#define setAT_PING                  "AT+PING=\"%s\""

#define setAT_CIPDINFO              "AT+CIPDINFO=%d"

  

/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
typedef enum
{
    //��׼������
    ESP8266_OK                          = (0x00), /*!<  */
    ESP8266_ERROR                       = (0x01), /*!<  */
	//��������� 
    ESP8266_ACK_FAIL                    = (0x02), /*!<  */
    ESP8266_UNLINK                      = (0x03), /*!<  */
    ESP8266_LINKED                      = (0x04), /*!<  */
}ESP8266_Error;


typedef enum
{
    test    = 0x00,
    query   = 0x01,
    setup   = 0x02,
    exe     = 0x03,
}CMDType;
#define IS_CMDTYPE(TYPE) ( ((TYPE) == test) || ((TYPE) == query) || ((TYPE) == setup) || ((TYPE) == exe) )

typedef enum
{
    online  = 0x01,
    offline = 0x02,
    busy    = 0x03,
    sleep   = 0x04,
}ESPStatus;
#define IS_ESPSTATUS(STATUS) ( ((MODE) == online) || ((MODE) == offline) || ((MODE) == busy) || ((MODE) == sleep) )

typedef enum
{
    sta     = 0x01,
    ap      = 0x02,
    apsta   = 0x03,
}WIFIMode;
#define IS_WIFIMODE(MODE) ( ((MODE) == sta) || ((MODE) == ap) || ((MODE) == apsta) )

typedef enum
{
    single  = 0x00, ///< ������ģʽ
    multi   = 0x01, ///< ������ģʽ
}MUXMode;
#define IS_MUXMODE(MODE) ( ((MODE) == single) || ((MODE) == multi) )

typedef enum
{
    TCPC    = 0x00,
    TCPS    = 0x01,
}TCPMode;
#define IS_TCPMODE(MODE) ( ((MODE) == TCPC) || ((MODE) == TCPS) )

typedef struct __SocketFD
{
    uint8_t link_id;
    TCPMode link_type;
    uint8_t link_status;
}_SocketFD;

/** 
 * @brief   ESP8266 init structure definition
 */
typedef struct __ESP8266
{
    ESPStatus   status;                               ///< ESP8266's Module online status
//    uint8_t     rx_addr[5];                         ///< Local Address
    
    /* ����������� */
    struct _UART
    {
        void    (*_send)           (const char *pData,const int len);
    }UART;
    char    *pRecv;
    
    CMDType     cmd_type;                           ///< cmd type
    char    command[100];                   ///< ���������ָ��
    char    ackinform[20];                  ///< �����ڴ�ȷ����Ϣ
    short   waittime;                       ///< �ȴ���ʱʱ�䣬��λ��10ms
    bool    result;                         ///< ȷ�Ͻ��
    
    /* WiFi��� */
    WIFIMode      wifi_mode;
    
    /* TCP\IP��� */
    _SocketFD   Sn[5];                      ///< 5�������׽��֣����֧�������������
    TCPMode     tcp_mode;
    MUXMode     mux_mode;                   ///< �������ͣ�0��������ģʽ   1��������ģʽ
    uint8_t     trans_mode;                 ///< ͸��ģʽ��0����ͨģʽ     1��͸��ģʽ����֧��TCP�����Ӻ�UDP�̶�ͨ�ŶԶ˵����
}_ESP8266;
extern _ESP8266 ESP8266;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
ESP8266_Error esp_8266_at_testCmd   ( __OUT_ short waittime,__IN__ const char *pCmd );
ESP8266_Error esp_8266_at_queryCmd  ( __OUT_ short waittime,__IN__ const char *pCmd );
ESP8266_Error esp_8266_at_setupCmd  ( __OUT_ short waittime,__IN__ const char *pCmd,... );
ESP8266_Error esp_8266_at_exeCmd    ( __OUT_ short waittime,__IN__ const char *pCmd );
/* WiFiģ����� */
ESP8266_Error esp_8266_ate          ( __IN__ const bool en );

/* WiFi��� */
ESP8266_Error esp_8266_wifimode     ( __IN__ const WIFIMode mode );

/* socket������� */
ESP8266_Error esp_8266_socket       ( __IN__ const MUXMode mode                                                          );
ESP8266_Error esp_8266_bind         ( __IN__ const short portNum, __IN__ const short stoTime                             );
ESP8266_Error esp_8266_TCPconnect   ( __IN__ const int link_ID,__IN__ const char *pAddr,__IN__ const short portNum );
ESP8266_Error esp_8266_UDPtrans     ( __IN__ const int link_ID,__IN__ const char *pAddr,__IN__ const short remotePort,__IN__ const short loadPort );
ESP8266_Error esp_8266_close        ( __IN__ const int link_ID                                                           );
ESP8266_Error esp_8266_send         ( __IN__ const int link_ID,   __IN__ const char *pSendBuf,__IN__ const int sendlen   );


#ifdef __cplusplus
}
#endif

#endif
