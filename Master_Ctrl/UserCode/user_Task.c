/**********************************************************************************
 * 文件名  ：user_Task.c
 * 描述    ：系统任务创建、定义任务函数
 * 版本    ：
**********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "user_main.h"
#include "tpad.h"
#include "cJSON.h"

 //任务堆栈            //任务函数
/* Private variables 任务堆栈---------------------------------------------------------*/
static OS_STK	LED0_TASK_STK			[LED0_STK_SIZE];
static OS_STK	TPAD1_TASK_STK			[TPAD1_STK_SIZE];
static OS_STK	TPAD2_TASK_STK			[TPAD2_STK_SIZE];
static OS_STK	TPAD3_TASK_STK			[TPAD3_STK_SIZE];
static OS_STK	TPAD4_TASK_STK			[TPAD4_STK_SIZE];
static OS_STK	TPAD5_TASK_STK			[TPAD5_STK_SIZE];
static OS_STK	TPAD6_TASK_STK			[TPAD6_STK_SIZE];
//static OS_STK	DealDataForNetUart_TASK_STK			[DealDataForNetUart_STK_SIZE];

/* Create a bunch of objects as demonstration. */
char* create_modeadd()
{
    cJSON *jsRoot,*jsData,*jsList;
    cJSON *jsMode0,*jsMode0Ctl,*jsMode0Ctl0,*jsMode0Ctl1,*jsMode0Ctl2;
    cJSON *jsMode1,*jsMode1Ctl,*jsMode1Ctl0,*jsMode1Ctl1,*jsMode1Ctl2;
    cJSON *jsMode2,*jsMode2Ctl,*jsMode2Ctl0,*jsMode2Ctl1,*jsMode2Ctl2;
    
//    char *out;
    
    jsRoot = cJSON_CreateObject();
    
    if( !jsRoot ) {
        printf( "Error before: [%s]\n",cJSON_GetErrorPtr() );
        return NULL;
    }
    
    cJSON_AddItemToObject   ( jsRoot,"+data",jsData = cJSON_CreateObject() );
        cJSON_AddStringToObject ( jsData,"cmd","modeAdd" );
        cJSON_AddItemToObject   ( jsData,"modeList",jsList = cJSON_CreateArray() );
    
            cJSON_AddItemToArray    ( jsList,jsMode0 =cJSON_CreateObject() );
                cJSON_AddStringToObject ( jsMode0,"modeName","rest" );
                cJSON_AddItemToObject   ( jsMode0,"modeCtl",jsMode0Ctl = cJSON_CreateArray() );
                    cJSON_AddItemToArray    ( jsMode0Ctl,jsMode0Ctl0 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode0Ctl0,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode0Ctl0,"nodeID",   "3443101002" );
                        cJSON_AddStringToObject ( jsMode0Ctl0,"devName",  "Pro"     );
                        cJSON_AddStringToObject ( jsMode0Ctl0,"devModel", "barco01" );
                        cJSON_AddNumberToObject ( jsMode0Ctl0,"devModel", 0         );
                    cJSON_AddItemToArray    ( jsMode0Ctl,jsMode0Ctl1 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode0Ctl1,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode0Ctl1,"nodeID",   "3443101004" );
                        cJSON_AddStringToObject ( jsMode0Ctl1,"devName",  "monitor" );
                        cJSON_AddStringToObject ( jsMode0Ctl1,"devModel", "LG00"    );
                        cJSON_AddNumberToObject ( jsMode0Ctl1,"devModel", 0         );
                    cJSON_AddItemToArray    ( jsMode0Ctl,jsMode0Ctl2 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode0Ctl2,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode0Ctl2,"nodeID",   "3443101006" );
                        cJSON_AddStringToObject ( jsMode0Ctl2,"devName",  "lamp"    );
                        cJSON_AddStringToObject ( jsMode0Ctl2,"devModel", "toplamp" );
                        cJSON_AddNumberToObject ( jsMode0Ctl2,"devModel", 0         );
    
            cJSON_AddItemToArray    ( jsList,jsMode1 =cJSON_CreateObject() );
                cJSON_AddStringToObject ( jsMode1,"modeName","read" );
                cJSON_AddItemToObject   ( jsMode1,"modeCtl",jsMode1Ctl = cJSON_CreateArray() );
                    cJSON_AddItemToArray    ( jsMode1Ctl,jsMode1Ctl0 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode1Ctl0,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode1Ctl0,"nodeID",   "3443101002" );
                        cJSON_AddStringToObject ( jsMode1Ctl0,"devName",  "Pro"     );
                        cJSON_AddStringToObject ( jsMode1Ctl0,"devModel", "barco01" );
                        cJSON_AddNumberToObject ( jsMode1Ctl0,"devModel", 1         );
                    cJSON_AddItemToArray    ( jsMode1Ctl,jsMode1Ctl1 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode1Ctl1,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode1Ctl1,"nodeID",   "3443101004" );
                        cJSON_AddStringToObject ( jsMode1Ctl1,"devName",  "monitor" );
                        cJSON_AddStringToObject ( jsMode1Ctl1,"devModel", "LG00"    );
                        cJSON_AddNumberToObject ( jsMode1Ctl1,"devModel", 1         );
                    cJSON_AddItemToArray    ( jsMode1Ctl,jsMode1Ctl2 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode1Ctl2,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode1Ctl2,"nodeID",   "3443101006" );
                        cJSON_AddStringToObject ( jsMode1Ctl2,"devName",  "lamp"    );
                        cJSON_AddStringToObject ( jsMode1Ctl2,"devModel", "toplamp" );
                        cJSON_AddNumberToObject ( jsMode1Ctl2,"devModel", 1         );
    
            cJSON_AddItemToArray    ( jsList,jsMode2 =cJSON_CreateObject() );
                cJSON_AddStringToObject ( jsMode2,"modeName","cons" );
                cJSON_AddItemToObject   ( jsMode2,"modeCtl",jsMode2Ctl = cJSON_CreateArray() );
                    cJSON_AddItemToArray    ( jsMode2Ctl,jsMode2Ctl0 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode2Ctl0,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode2Ctl0,"nodeID",   "3443101002" );
                        cJSON_AddStringToObject ( jsMode2Ctl0,"devName",  "Pro"     );
                        cJSON_AddStringToObject ( jsMode2Ctl0,"devModel", "barco01" );
                        cJSON_AddNumberToObject ( jsMode2Ctl0,"devModel", 1         );
                    cJSON_AddItemToArray    ( jsMode2Ctl,jsMode2Ctl1 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode2Ctl1,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode2Ctl1,"nodeID",   "3443101004" );
                        cJSON_AddStringToObject ( jsMode2Ctl1,"devName",  "monitor" );
                        cJSON_AddStringToObject ( jsMode2Ctl1,"devModel", "LG00"    );
                        cJSON_AddNumberToObject ( jsMode2Ctl1,"devModel", 0         );
                    cJSON_AddItemToArray    ( jsMode2Ctl,jsMode2Ctl2 = cJSON_CreateObject() );
                        cJSON_AddStringToObject ( jsMode2Ctl2,"cmd",      "nodeCtl" );
                        cJSON_AddStringToObject ( jsMode2Ctl2,"nodeID",   "3443101006" );
                        cJSON_AddStringToObject ( jsMode2Ctl2,"devName",  "lamp"    );
                        cJSON_AddStringToObject ( jsMode2Ctl2,"devModel", "toplamp" );
                        cJSON_AddNumberToObject ( jsMode2Ctl2,"devModel", 1         );
            
    char *jsDataStr = cJSON_PrintUnformatted( jsData );
    int calCrcVal = GetCRC16( (unsigned char*)jsDataStr,strlen(jsDataStr) );
    free( jsDataStr );
    cJSON_AddNumberToObject( jsRoot,"crc",calCrcVal );
    
    char *jsRootStr = cJSON_PrintUnformatted( jsRoot );    cJSON_Delete( jsRoot );
    
    if (jsRootStr)
        return jsRootStr;
    
//    printf( ":::::::::::::::::::%d:::::::::::::::::::\n%s\n",strlen(jsRootStr),jsRootStr ); free( jsRootStr );
    
    return NULL;
}

/* Create a bunch of objects as demonstration. */
char* create_modeCtl()
{
    cJSON *jsRoot,*jsData;
    
    jsRoot = cJSON_CreateObject();
    if( !jsRoot ) {
        printf( "Error before: [%s]\n",cJSON_GetErrorPtr() );
        return NULL;
    }
    
    cJSON_AddItemToObject( jsRoot,"+data",jsData=cJSON_CreateObject() );
        cJSON_AddStringToObject( jsData,"cmd","modeCtl" );
        cJSON_AddStringToObject( jsData,"modeName","rest" );
    
    char *jsDataStr = cJSON_PrintUnformatted( jsData );
    if ( jsDataStr ) {
        int calCrcVal = GetCRC16( (unsigned char*)jsDataStr,strlen(jsDataStr) );
        free( jsDataStr );
        cJSON_AddNumberToObject( jsRoot,"crc",calCrcVal );
        
        char *jsRootStr = cJSON_PrintUnformatted( jsRoot ); cJSON_Delete( jsRoot );
        if ( jsRootStr ) return jsRootStr;
        else return NULL;
    }
    else {
        cJSON_Delete( jsRoot );
        return NULL;
    }
}

/* Create a bunch of objects as demonstration. */
char* create_nodeCtl()
{
    cJSON *jsRoot,*jsData;
    
    jsRoot = cJSON_CreateObject();
    if( !jsRoot ) {
        printf( "Error before: [%s]\n",cJSON_GetErrorPtr() );
        return NULL;
    }
    
    cJSON_AddItemToObject( jsRoot,"+data",jsData=cJSON_CreateObject() );
        cJSON_AddStringToObject ( jsData,"cmd",         "nodeCtl"   );
        cJSON_AddStringToObject ( jsData,"nodeID",      "3443101002");
        cJSON_AddStringToObject ( jsData,"devName",     "monitor"   );
        cJSON_AddStringToObject ( jsData,"devModel",    "LG00"      );
        cJSON_AddNumberToObject ( jsData,"devModel",    0           );
    
    char *jsDataStr = cJSON_PrintUnformatted( jsData );
    if ( jsDataStr ) {
        int calCrcVal = GetCRC16( (unsigned char*)jsDataStr,strlen(jsDataStr) );
        free( jsDataStr );
        cJSON_AddNumberToObject( jsRoot,"crc",calCrcVal );
        
        char *jsRootStr = cJSON_PrintUnformatted( jsRoot ); cJSON_Delete( jsRoot );
        if ( jsRootStr ) return jsRootStr;
        else return NULL;
    }
    else {
        cJSON_Delete( jsRoot );
        return NULL;
    }
}


OS_STK_DATA  StackBytes;
//LED0任务
static void led0_task (void)
{
//    static int LCD_Data=0;
    
	while ( 1 ) {
		OSTimeDlyHMSM(0,0,0,500);
        LED_0 = !LED_0;
        
    /* TEST CODE BEGIN 0 */
//        NRF24L01_TX_Buffer( ACK,TX_ADDRESS,test,130 );
    /* TEST CODE END 0 */

        
    /* TEST CODE BEGIN 1 */
//        HT16xx_test(LCD_Data++);
//        if(LCD_Data>0x0F)
//        LCD_Data = 0;
    /* TEST CODE END 1 */

        
    /* TEST CODE BEGIN 2 */
//        if ( AT24CXX_OK==AT24CXX_Init() )
//            LED_1 = !LED_1;
    /* TEST CODE END 2 */
        
    /* TEST CODE BEGIN 3 */
    /* TEST CODE END 3 */
	}
}


extern void execModeCtl         ( const char *modeName );
extern void net_uart_cmd_nodeCtl(void* cmd);
extern void net_uart_cmd_modeAdd(void* cmd);
extern void net_uart_cmd_modeCtl(void* cmd);
extern int CommNetUart_DealCmdBuf( void *cmd );

/**
  * @brief  触摸按键检测任务
  * @param  None
  * @note   None
  * @retval None
  */
static void tpad1_task (void)
{
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD1,0) ) {
            LED_1 = !LED_1;
            HT16xx_test( 0x02 );
            
            /* TEST CODE BEGIN 0 */
            execModeCtl( "rest" );
            /* TEST CODE END 0 */
            
            /* TEST CODE BEGIN 1 */
            
            
            /* TEST CODE END 1 */
        }
    }
}
static void tpad2_task (void)
{
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD2,0) ) {
            LED_1 = !LED_1;
            HT16xx_test( 0x06 );
            
            /* TEST CODE BEGIN 0 */
            execModeCtl( "read" );
            /* TEST CODE END 0 */
        }
    }
}

static void tpad3_task (void)
{    
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD3,0) ) {
            LED_1 = !LED_1;
//            HT16xx_test( 0x0A );
//            
//            /* TEST CODE BEGIN 0 */
//            char * my_text = create_nodeCtl();
//            if ( my_text ) {
//                printf( "Request::%s\n",my_text );
//                CommNetUart_DealCmdBuf( my_text );
//                free( my_text );
//            }
//            /* TEST CODE END 0 */
        }
    }
}

static void tpad4_task (void)
{
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD4,0) ) {
            LED_1 = !LED_1;
            HT16xx_test( 0x0D );
            
            /* TEST CODE BEGIN 0 */
            OSTaskStkChk( START_TASK_PRIO,&StackBytes );
            printf( "Start Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( LED0_TASK_PRIO,&StackBytes );
            printf( "LED Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD1_TASK_PRIO,&StackBytes );
            printf( "TPAD1 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD2_TASK_PRIO,&StackBytes );
            printf( "TPAD2 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD3_TASK_PRIO,&StackBytes );
            printf( "TPAD3 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD4_TASK_PRIO,&StackBytes );
            printf( "TPAD4 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD5_TASK_PRIO,&StackBytes );
            printf( "TPAD5 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( TPAD6_TASK_PRIO,&StackBytes );
            printf( "TPAD6 Task:  %d\n",StackBytes.OSUsed/4 );
            
            OSTaskStkChk( DealDataForNetUart_TASK_PRIO,&StackBytes );
            printf( "DealDataForNetUart Task:  %d\n",StackBytes.OSUsed/4 );
            /* TEST CODE END 0 */
        }
    }
}

static void tpad5_task (void)
{
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD5,0) ) {
            LED_1 = !LED_1;
            HT16xx_test( 0x0F );
            
            /* TEST CODE BEGIN 0 */
//            char *jsDataStr = create_modeadd();
//            if ( jsDataStr ) {
//                CommNetUart_DealData( jsDataStr );
//                free( jsDataStr );
//            }
            /* TEST CODE END 0 */
        }
    }
}

static void tpad6_task (void)
{
	while ( 1 )	{
		OSTimeDlyHMSM(0,0,0,100);
        
        if ( tpad_scan(TPAD6,0) ) {
            LED_1 = !LED_1;
            HT16xx_test( 0x00 );
            
            /* TEST CODE BEGIN 0 */
//            char * my_text = create_modeCtl();
//            if ( my_text ) {
//                printf( "Request::%s\n",my_text );
//                CommNetUart_DealCmdBuf( my_text );
//                free( my_text );
//            }
            /* TEST CODE END 0 */
        }
    }
}

extern void CommNetUart_DealRXQueue(void);
static void DealDataForNetUart_task(void)
{
    while(1) {
		OSTimeDlyHMSM(0,0,0,100);
        
//        CommNetUart_DealRXQueue();
    }
}

/**
  * @brief  任务创建函数
  * @param  None
  * @note   None
  * @retval None
  */
void  User_TaskCreate (void)
{
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) led0_task,
        (void          * ) 0,
        (OS_STK        * ) &LED0_TASK_STK[LED0_STK_SIZE-1],
        (INT8U           ) LED0_TASK_PRIO,
        (INT8U           ) LED0_TASK_PRIO,
        (OS_STK        * ) &LED0_TASK_STK[0],
        (INT32U          ) LED0_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad1_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD1_TASK_STK[TPAD1_STK_SIZE-1],
        (INT8U           ) TPAD1_TASK_PRIO,
        (INT8U           ) TPAD1_TASK_PRIO,
        (OS_STK        * ) &TPAD1_TASK_STK[0],
        (INT32U          ) TPAD1_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad2_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD2_TASK_STK[TPAD2_STK_SIZE-1],
        (INT8U           ) TPAD2_TASK_PRIO,
        (INT8U           ) TPAD2_TASK_PRIO,
        (OS_STK        * ) &TPAD2_TASK_STK[0],
        (INT32U          ) TPAD2_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad3_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD3_TASK_STK[TPAD3_STK_SIZE-1],
        (INT8U           ) TPAD3_TASK_PRIO,
        (INT8U           ) TPAD3_TASK_PRIO,
        (OS_STK        * ) &TPAD3_TASK_STK[0],
        (INT32U          ) TPAD3_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad4_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD4_TASK_STK[TPAD4_STK_SIZE-1],
        (INT8U           ) TPAD4_TASK_PRIO,
        (INT8U           ) TPAD4_TASK_PRIO,
        (OS_STK        * ) &TPAD4_TASK_STK[0],
        (INT32U          ) TPAD4_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad5_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD5_TASK_STK[TPAD5_STK_SIZE-1],
        (INT8U           ) TPAD5_TASK_PRIO,
        (INT8U           ) TPAD5_TASK_PRIO,
        (OS_STK        * ) &TPAD5_TASK_STK[0],
        (INT32U          ) TPAD5_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
    /* Task creation */
    OSTaskCreateExt(
        (void (*)(void *)) tpad6_task,
        (void          * ) 0,
        (OS_STK        * ) &TPAD6_TASK_STK[TPAD6_STK_SIZE-1],
        (INT8U           ) TPAD6_TASK_PRIO,
        (INT8U           ) TPAD6_TASK_PRIO,
        (OS_STK        * ) &TPAD6_TASK_STK[0],
        (INT32U          ) TPAD6_STK_SIZE,
        (void          * ) 0,
        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
//    /* Task creation */
//    OSTaskCreateExt(
//        (void (*)(void *)) DealDataForNetUart_task,
//        (void          * ) 0,
//        (OS_STK        * ) &DealDataForNetUart_TASK_STK[DealDataForNetUart_STK_SIZE-1],
//        (INT8U           ) DealDataForNetUart_TASK_PRIO,
//        (INT8U           ) DealDataForNetUart_TASK_PRIO,
//        (OS_STK        * ) &DealDataForNetUart_TASK_STK[0],
//        (INT32U          ) DealDataForNetUart_STK_SIZE,
//        (void          * ) 0,
//        OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
        
}
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
