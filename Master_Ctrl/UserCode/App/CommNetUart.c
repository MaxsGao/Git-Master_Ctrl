/*
*********************************************************************************************************
*
*
*                                   Communicate with net uart
*                                        与网络串口通信
* File : CommNetUart.c
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : 此功能模块是通过ESP8266模块，以实现串口转网络通信
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
#include <stdbool.h>
#include "CommNetUart.h"
#include "bsp_uart.h"
#include "2_4GRF.h"
#include "SignalLED.h"

#include "BoxOs_str.h"
#include "CRC16.h"
#include "cJSON.h"
#include "queue.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
*********************************************************************************************************
*/
DECL_NETUART_CMD_FUNC(test);    ///< void net_uart_cmd_test(char* cmd)
DECL_NETUART_CMD_FUNC(nodeCtl); ///< void net_uart_cmd_nodectl(char* cmd)

DECL_NETUART_CMD_FUNC(modeAdd); ///< void net_uart_cmd_modeadd(char* cmd)
DECL_NETUART_CMD_FUNC(modeCtl); ///< void net_uart_cmd_modectl(char* cmd)

#define ADDS_NETUART_CMD_FUNC_ALL   \
    ADDS_NETUART_CMD_FUNC(test),    \
    ADDS_NETUART_CMD_FUNC(nodeCtl), \
    ADDS_NETUART_CMD_FUNC(modeAdd), \
    ADDS_NETUART_CMD_FUNC(modeCtl), \
    


/*
*********************************************************************************************************
*                                       CONSTANTS           常量
*********************************************************************************************************
*/
const net_uart_cmd_ctrl net_uart_ctrls[] = { ADDS_NETUART_CMD_FUNC_ALL };

#define CNT_NET_UART_CMD_TYPE   ( sizeof(net_uart_ctrls)/sizeof(net_uart_cmd_ctrl) )



/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
static uint32_t  UartBaudToNetUart;   ///< 调试串口的通信波特率

static uint8_t gUartSendBufToNetUart[128];


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
cJSON *crcCheck( char *cmd )
{
    cJSON *jsRoot,*jsData,*jsCrc;
    int calCrcVal,getCrcVal;
    
	jsRoot = cJSON_Parse( cmd );
	if (!jsRoot) {
        printf("Error before: [%s]\n",cJSON_GetErrorPtr());
        return NULL;
    }
    
    jsData  = cJSON_GetObjectItem( jsRoot,"+data" );
    jsCrc   = cJSON_GetObjectItem( jsRoot,"crc" );
    
    if ( jsData && jsCrc ) {
        char *jsDataStr = cJSON_PrintUnformatted( jsData );
        if (jsDataStr) {
            calCrcVal = GetCRC16( (unsigned char*)jsDataStr,strlen(jsDataStr) ); 
            free(jsDataStr);
        }
        getCrcVal = jsCrc->valueint;
    }
    
    if ( calCrcVal==getCrcVal ) {
        printf( "check ok!\n" );
        
        cJSON *jsCopy = cJSON_Duplicate( jsData,1 ); cJSON_Delete( jsRoot );
        if ( !jsCopy ) {
            printf( "Error before: [%s]\n",cJSON_GetErrorPtr() );
            return NULL;
        }
        return jsCopy;
    }
    
    cJSON_Delete( jsRoot );
    return NULL;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
void execModeCtl( const char *modeName )
{
    cJSON *js_fdata=NULL,*js_modeList=NULL,*js_mode=NULL,*js_modeCtl=NULL,*js_nodeCtl=NULL;
    
    E2PFIL  fp_mode;
    char    *fdata=NULL;
    int     rcnt=0;
    
    /* 打开配置文件，将模式信息写入到配置文件中去 */
    if ( e2pf_open( &fp_mode,(uint8_t*)"mode",E2PFA_WRITE ) == E2PFR_OK ) {
        fdata = (char*)malloc(fp_mode.fsize+1);
        if ( fdata ) {
            memset( fdata,0,fp_mode.fsize+1 );
            e2pf_seek( &fp_mode,0,E2PFA_SEEK_SET ); e2pf_read( &fp_mode, fdata, fp_mode.fsize, (uint32_t*)&rcnt);
            if ( rcnt==fp_mode.fsize ) {
//                printf( "mode file data::%s\n",fdata );
                printf( "read mode file ok,%d\n",rcnt );
                js_fdata = cJSON_Parse( fdata );
            }
            
            free( fdata );
        }
        e2pf_close( &fp_mode );
    }
    else
        printf( "open mode file fail!\n" );
    
    if ( js_fdata ) {
        js_modeList = cJSON_GetObjectItem( js_fdata,"modeList" );
        if ( js_modeList->type == cJSON_Array ) {
            int listSize = cJSON_GetArraySize( js_modeList );
            for ( int i=0;i<listSize;i++ ) {
                js_mode = cJSON_GetArrayItem( js_modeList,i );
                if ( js_mode ) {
                    if ( strcmp( modeName,cJSON_GetObjectItem(js_mode,"modeName")->valuestring ) == 0 ) {
                        printf( "exec \"%s\" mode\n",modeName );
                        
                        js_modeCtl = cJSON_GetObjectItem( js_mode,"modeCtl" );
                        if ( js_modeCtl->type == cJSON_Array ) {
                            int js_modeCtlArraySize =  cJSON_GetArraySize( js_modeCtl );
                            for( int j=0;j<js_modeCtlArraySize;j++ ) {
                                js_nodeCtl = cJSON_GetArrayItem( js_modeCtl,j );
                                
                                if ( !strcmp( "nodeCtl",cJSON_GetObjectItem(js_nodeCtl,"cmd")->valuestring ) ) {
                                    net_uart_cmd_nodeCtl( js_nodeCtl );
                                }
                            }
                        }
                        
                        break;
                    }
                }
            }
        }
        cJSON_Delete( js_fdata );
    }
    else
        printf( "Error before: [%s]\n",cJSON_GetErrorPtr() );
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
void AddCrc2cJson(
    cJSON *json
)
{
    char *js_str = cJSON_PrintUnformatted( json );
    int cal_crc_val = GetCRC16( (unsigned char*)js_str,strlen(js_str) );;
    free(js_str);
    
    cJSON_AddNumberToObject( json,"crc",cal_crc_val );
}

/**
  * @brief  
  * @param  
  * @param  
  * @note   void net_uart_cmd_test(char* cmd)
  * @retval None
  */
DECL_NETUART_CMD_FUNC(test)
{
    printf( "receive test%s\r\n",cmd );
}

/**
  * @brief  单节点控制命令的处理函数
  * @param  cmd 控制命令
  *     -e.g  {"cmd":"nodectl", "nodeId":"3443101002", "devName":"投影仪", "devModel":"索尼TBxx", "devCtl":1}
  * @note   void net_uart_cmd_node(void* cmd) 
  *     -过程 获取子节点的无线通信信道地址——添加CRC校验——将JSON格式命令转化为字符串格式——通过NRF无线发送数据
  * @retval None
  */
DECL_NETUART_CMD_FUNC(nodeCtl)
{
    cJSON *jsRoot   = NULL;
    cJSON *jsData   = (cJSON *)cmd;
    cJSON *jsNodeId = NULL;
    
    uint8_t TX_ADDRESS[5];
    
    if ( !jsData )
        return ;
    
    /* 获取子节点的信道地址 */
    jsNodeId = cJSON_GetObjectItem( jsData,"nodeId" );
    if ( !(jsNodeId) )
        return ;
    printf( "nodeId %s nodeCtl!\n",jsNodeId->valuestring );
    memset( TX_ADDRESS,0,sizeof(TX_ADDRESS) );
    uint32_t outLen = HexStrToHexArr( jsNodeId->valuestring,strlen(jsNodeId->valuestring),TX_ADDRESS );
    
    /* 添加CRC校验 cJSON *jsCopy = cJSON_Duplicate( jsData,1 );*/
    jsRoot=cJSON_CreateObject();
    if ( !(jsRoot) ) return;
    
    cJSON_AddItemToObject( jsRoot,"+data",cJSON_Duplicate( jsData,1 ) );
    
    char *jsDataStr = cJSON_PrintUnformatted( jsData );
    if ( jsDataStr ) {
        int calCrcVal = GetCRC16( (unsigned char*)jsDataStr,strlen(jsDataStr) ); free( jsDataStr );
        cJSON_AddNumberToObject( jsRoot,"crc",calCrcVal );
    
        /* 将JSON格式命令转化为字符串格式，并通过NRF无线模块发送出去 */
        char *jsRootStr = cJSON_PrintUnformatted( jsRoot );
        if ( jsRootStr ) {
            if ( 0 == NRF24L01_TX_Buffer( ACK,TX_ADDRESS,(unsigned char*)jsRootStr,strlen(jsRootStr) ) )
                printf( "nodeCtl exec accuess!\n" );
            else
                printf( "nodeCtl exec fail!\n" );
            free( jsRootStr );
        }
    }
    
    cJSON_Delete( jsRoot );
}

/**
  * @brief  
  * @param  
  * @param  
  * @note   void net_uart_cmd_mode(char* cmd)
  * @retval None
  */
DECL_NETUART_CMD_FUNC(modeAdd)
{
    E2PFIL  fp_mode;
    int     wcnt=0;
   
    /* 对命令进行解析并校验 */
    cJSON *jsData = (cJSON *)cmd;
    
    if ( !jsData )
        return ;    
    
    /* 打开配置文件，将模式信息写入到配置文件中去 */
    if ( e2pf_open( &fp_mode,(uint8_t*)"mode",E2PFA_CREATE_NEW|E2PFA_WRITE ) == E2PFR_OK ) {
        char *jsDataStr = cJSON_PrintUnformatted( jsData );
        
        if ( jsDataStr ) {
            printf( "jsDataStr::%d\n%s\n",strlen(jsDataStr),jsDataStr );
            
            e2pf_seek( &fp_mode,0,E2PFA_SEEK_SET ); 
            e2pf_write( &fp_mode,jsDataStr,strlen(jsDataStr), (uint32_t*)&wcnt );
            if ( wcnt==strlen(jsDataStr) )
                printf( "mode file size:%d,%d\njsDataStr write ok!\n",wcnt,fp_mode.fsize );
            else
                printf( "mode file size:%d,%d\njsDataStr write fail!\n",wcnt,fp_mode.fsize );
            
            free( jsDataStr );
        }
        e2pf_close( &fp_mode );
    }
}

/**
  * @brief  “模式控制”指令的解析入口函数
  * @param   cmd: void*
  * @param  
  * @note   void net_uart_cmd_mode(char* cmd)
  * @retval None
  */
DECL_NETUART_CMD_FUNC(modeCtl)
{
    cJSON *jsData=(cJSON *)cmd;
    
    /* 对命令进行解析 */
    if ( !jsData )
        return ; 
    
    cJSON *jsModeName = cJSON_GetObjectItem( jsData,"modeName" );
    if ( !jsModeName )
        return;
    
    /* 执行相应的模式 */
    execModeCtl( jsModeName->valuestring );
    
}

/**
  * @brief  串口接收中断的回调函数
  * @param  recvData    待处理数据
  * @note   将接收到的数据添加到待处理缓冲区队列中，由处理线程处理
  * @retval None
  */
Queue CommNetUart_RXQueue;
uint8_t CommNetUart_RXBuf[2048];
static void CommNetUart_RecvHandleCbFunc( uint8_t recvData )
{
    if ( Queue_In( &CommNetUart_RXQueue,&recvData,sizeof(uint8_t),ISFULL_DEL_OLD,NULL ) ) {
        printf( "Queue In Fail!\n" );
    }
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */

int CommNetUart_Init(void)
{
    UartBaudToNetUart = BAUD_RATE_9600;
    
    if ( Queue_Create( &CommNetUart_RXQueue, CommNetUart_RXBuf,2048,sizeof(uint8_t) ) )
        return -1;
    
    /* 初始化网络串口NetUart */
    Uart_Config(
        UartConfigToNetUart,
        UartPortToNetUart,
        UART_TTL_EN,
        UartBaudToNetUart,
        gUartSendBufToNetUart,
        sizeof(gUartSendBufToNetUart),
        CommNetUart_RecvHandleCbFunc
    );
    
    /* 初始化通信相关的变量或标识符 */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNetUart_SendHeartBeat(void)
{
    /* 构建一组心跳包数据，cmd数据格式 */
    /* 将心跳包数据通过串口发送出去 */
    return 0;
}

/**
  * @brief  
  * @param  
  * @note   
  * @retval 
  */
int CommNetUart_DealCmdBuf( void *cmd )
{
    /* 逐字节读取并处理串口缓冲区中的数据 */
    
    
    cJSON *jsData=NULL,*jsCmdType=NULL;
    
    /* 对命令进行解析并校验 */
    jsData = crcCheck( cmd );
    
    if ( !jsData )
        return 1;
    
    jsCmdType = cJSON_GetObjectItem( jsData,"cmd" );
    
    if ( jsCmdType ) {
        for ( int i=0;i<CNT_NET_UART_CMD_TYPE;i++ ) {
            if ( !strcmp( net_uart_ctrls[i].cmd_str,jsCmdType->valuestring ) ) {
                net_uart_ctrls[i].pfun( jsData );
                break;
            }
        }
    }
    
    cJSON_Delete( jsData );
    
    return 0;
}

/**
  * @brief  对接收缓冲区的数据处理操作
  * @param  None
  * @note   此函数为线程函数，需放到线程中进行循环调用，以及时处理缓冲区中的数据
  *   -功能 寻到缓冲区中的完整的数据包，然后调用数据包处理函数进行处理
  * @retval None
  */
void CommNetUart_DealRXQueue(void)
{
    static uint8_t  cmdType = 0;     ///< 0——没有检测到包头   1——JSON格式指令
    static uint16_t cmdLen  = 0;
    static uint8_t  bigFlag = 0, medFlag = 0;
    static uint8_t  cmdBuf[2048];
    uint8_t dataByte;
    
    while( !Queue_Out(&CommNetUart_RXQueue,&dataByte,sizeof(uint8_t)) )
    {
//        printf( "%c",dataByte );
        if ( '+'==dataByte ) {
            if ( cmdLen>=2 ) {
                if ( (cmdBuf[cmdLen-1]=='\"') && (cmdBuf[cmdLen-2]=='{') ) {
                    cmdType = 1;
                    cmdLen  = 2;
                    bigFlag = 1;
                    medFlag = 0;
                    cmdBuf[0] = '{';
                    cmdBuf[1] = '\"';
                    cmdBuf[2] = '+';
                }
                else {
                    cmdType = cmdLen = bigFlag = medFlag =0;
                    memset( cmdBuf,0,2048 );
                }
            }
            else {
                cmdType = cmdLen = bigFlag = medFlag =0;
                memset( cmdBuf,0,2048 );
            }
        }
        
        if ( 0==cmdType ) {
            if ( cmdLen<2 )
                cmdBuf[cmdLen++] = dataByte;
            else
                cmdLen = 0;
        }
        else {
            if ( '{' == dataByte )
                bigFlag +=1;
            
            else if ( '}' == dataByte )
                bigFlag -=1;
            
            else if ( '[' == dataByte )
                medFlag +=1;
            
            else if ( ']' == dataByte )
                medFlag -=1;
            
            cmdBuf[cmdLen++] = dataByte;
            
            if ( (cmdLen>=2048) || ( (bigFlag==0) && (medFlag!=0) ) ) {
                cmdType = cmdLen = bigFlag = medFlag =0;
                memset( cmdBuf,0,2048 );
            }
            else if ( bigFlag==0 ){
                printf( "%s\n",cmdBuf );
                cmdType = cmdLen = bigFlag = medFlag =0;
                memset( cmdBuf,0,2048 );
            }
        }
    }
}

