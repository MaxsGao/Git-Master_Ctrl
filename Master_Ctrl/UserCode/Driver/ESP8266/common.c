/*
*********************************************************************************************************
*
*
*                                       ESP8266 Module's Generic Function Interface
*                                         ESP8266模块的通用函数接口
* File : common.c
* By   :
* Date : 2019/01/11
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
#include "common.h"
#include "bsp_delay.h"


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

///** @defgroup SPI_Private_Defines
//  * @{
//  */

///* SPI SPE mask */
//#define CR1_SPE_Set          ((uint16_t)0x0040)
//#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

///* I2S I2SE mask */
//#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
//#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

///**
//  * @}
//  */


/*
*********************************************************************************************************
*                                       VARIABLES           变量
*********************************************************************************************************
*/
_ESP8266 ESP8266;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
static __inline ESP8266_Error send_cmd_common()
{
    ESP8266_Error errorstatus=ESP8266_ACK_FAIL;

    ESP8266.result = false;                                         ///< 清除确认结果
    ESP8266.UART._send( ESP8266.command,strlen(ESP8266.command) );  ///< 通过串口发送AT指令

    if( ESP8266.waittime!=0 )                                       ///< 等待响应
    {
        while( --ESP8266.waittime )                                 ///< 等待倒计时
        {
            delay_ms(10);
            if( ESP8266.result == true ) {
                errorstatus = ESP8266_OK;
                break;                                              ///< 得到正确的确认信息
            }
        }
    }
    memset( ESP8266.command,0,sizeof(ESP8266.command) );

    return errorstatus;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
static __inline ESP8266_Error send_data_common( __IN__ const char *pSendBuf,__IN__ const int sendlen )
{
    ESP8266_Error errorstatus=ESP8266_ACK_FAIL;

    ESP8266.result = false;                     ///< 清除确认结果
    ESP8266.UART._send( pSendBuf,sendlen );     ///< 通过串口发送AT指令

    if( ESP8266.waittime!=0 )                   ///< 等待响应
    {
        while( --ESP8266.waittime )             ///< 等待倒计时
        {
            delay_ms(10);
            if( ESP8266.result == true ) {
                errorstatus = ESP8266_OK;
                break;                          ///< 得到正确的确认信息
            }
        }
    }

    return errorstatus;
}



/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
*********************************************************************************************************
*/
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_at_testCmd( __OUT_ short waittime,__IN__ const char *pCmd )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    sprintf( ESP8266.command,"%s=?\r\n",pCmd );
    ESP8266.waittime = waittime;

    errorstatus = send_cmd_common();

    return errorstatus;
}
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_at_queryCmd( __OUT_ short waittime,__IN__ const char *pCmd )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    sprintf( ESP8266.command,"%s?\r\n",pCmd );
    ESP8266.waittime = waittime;

    errorstatus = send_cmd_common();

    return errorstatus;
}
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_at_setupCmd( __OUT_ short waittime,__IN__ const char *pCmd,... )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    char str[100];
    va_list ap;
    __va_start( ap,pCmd );
    vsprintf( str,pCmd,ap );
    __va_end( ap );

    sprintf( ESP8266.command,"%s\r\n",str );
    ESP8266.waittime = waittime;

    errorstatus = send_cmd_common();

    return errorstatus;
}
/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_at_exeCmd( __OUT_ short waittime,__IN__ const char *pCmd )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    sprintf( ESP8266.command,"%s\r\n",pCmd );
    ESP8266.waittime = waittime;

    errorstatus = send_cmd_common();

    return errorstatus;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_check_at(
    __IN__ const char *data,
    __IN__ const char *ack,
    __OUT_ short waittime
)
{
    ESP8266_Error errorstatus=ESP8266_OK;

//    errorstatus = AT();
    if( ESP8266_OK==errorstatus ) {
        ESP8266.status = online;
    } else {
        ESP8266.status = offline;
    }

    return errorstatus;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_ate( __IN__ const bool en )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    if( en==0 )
    {
        errorstatus = esp_8266_at_exeCmd( 30,ATE0 );
    } else
    {
        errorstatus = esp_8266_at_exeCmd( 30,ATE1 );
    }

    return errorstatus;
}


/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_wifimode( __IN__ const WIFIMode mode )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    /* Check the parameters */
    assert_param( IS_WIFIMODE(mode) );

    errorstatus = esp_8266_at_setupCmd( 50,setAT_CWMODE_DEF,mode );

    return errorstatus;
}

/**
  * @brief
  * @param
  * @param
  * @retval
  * @note
  */
ESP8266_Error esp_8266_socket(__IN__ const MUXMode mode)
{
    ESP8266_Error errorstatus=ESP8266_OK;

    /* Check the parameters */
    assert_param( IS_MUXMODE(mode) );

    /* Get MUX Mode */
    if( ESP8266_OK!=esp_8266_at_queryCmd( 30,queAT_CIPMUX ) )
        return ESP8266_ACK_FAIL;

    /* Switch MUX Mode */
    if( ESP8266.mux_mode!=mode )
    {
        if( mode==single )  ///< 切换到单连接模式
        {
            if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPCLOSE,5 ) )      ///< 关闭所有连接，（必须在没有连接建立的情况下，设置连接模式）
                return ESP8266_ACK_FAIL;

            if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPSERVER0,0 ) )    ///< 关闭服务器，（如果建立了TCP服务器，想切换为单链接，必须关闭服务器）
                return ESP8266_ACK_FAIL;

            errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPMUX,0 );           ///< 设置为单连接
        } else              ///< 切换到多连接模式
        {
            if( ESP8266_OK!=esp_8266_at_queryCmd( 30,queAT_CIPMODE ) )
                return errorstatus;

            if( 1==ESP8266.trans_mode ) {
                if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPMODE,0 ) )   ///< 只有在非透传模式（“AT+CIPMODE=0”），才能设置多连接
                    return ESP8266_ACK_FAIL;
            }

            errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPMUX,1 );           ///< 设置为多连接
        }
    }

    return errorstatus;
}

ESP8266_Error esp_8266_bind( __IN__ const short portNum,__IN__ const short stoTime )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPSERVER1,1,portNum );
    if( ESP8266_OK==errorstatus )
    {
        errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPSTO,stoTime );
    }

    return errorstatus;
}

ESP8266_Error esp_8266_TCPconnect( __IN__ const int link_ID,__IN__ const char *pAddr,__IN__ const short portNum )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CIPSTATUS );
    if( errorstatus == ESP8266_OK )
    {
        if( ESP8266.Sn[link_ID].link_status==OFF )
        {
            errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPSTART_TCP,link_ID,"TCP",pAddr,portNum );
        } else
        {
            errorstatus = ESP8266_LINKED;
        }
    }

    return errorstatus;
}

ESP8266_Error esp_8266_UDPtrans( __IN__ const int link_ID,__IN__ const char *pAddr,__IN__ const short remotePort,__IN__ const short loadPort )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    errorstatus = esp_8266_at_exeCmd( 30,exeAT_CIPSTATUS );
    if( errorstatus == ESP8266_OK )
    {
        if( ESP8266.Sn[link_ID].link_status==OFF )
        {
            errorstatus = esp_8266_at_setupCmd( 50,setAT_CIPSTART_UDP,link_ID,"UDP",pAddr,remotePort,loadPort,0 );
        } else
        {
            errorstatus = ESP8266_LINKED;
        }
    }

    return errorstatus;
}

ESP8266_Error esp_8266_close( __IN__ const int link_ID )
{
    ESP8266_Error errorstatus;

    if( ESP8266.Sn[link_ID].link_status==ON ) {
        errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPCLOSE,link_ID );
    } else
    {
        errorstatus = ESP8266_UNLINK;
    }

    return errorstatus;
}

ESP8266_Error esp_8266_send( __IN__ const int link_ID,__IN__ const char *pSendBuf,__IN__ const int sendlen )
{
    ESP8266_Error errorstatus=ESP8266_OK;

    if( ESP8266.Sn[link_ID].link_status==ON )
    {
        errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPSEND1,link_ID,sendlen );
        if( errorstatus==ESP8266_OK )
        {
            errorstatus = send_data_common( pSendBuf,sendlen );
        }
    }

    return errorstatus;
}
void uart_isr()
{
    //loc用来对ret_msg全局变量给偏移，用来组装一个字符串
    static unsigned char i = 0;
    unsigned char temp;

    ES = 0;
    temp = SBUF;//temp不能被改变，因为软复位需要用到

    /*ESP8266截取字符串部分*/
    if(temp == '\n')//开始符
    {
        Rev_status = BEGIN;//设置开始接收
    }
    else if(temp == '\r')//结束符
    {
        Rev_status = END;//设置结束接收
    }
    else//出来\r\n以外的字符
    {

        /************************************************/

        //专门用来接收IPD和CIFSR，结束接收时要把Rev_Str_status置为无效，再次进入下面的循环，检测第一个字符
        if(Rev_Str_status == BEGIN)
        {
            if((str_rev_flag == END) &&(temp != ':') && (str_len_flag == BEGIN))//开始接收字符串长度
            {
                str_len = str_len * 10 + (temp - '0');
            }

            //为了充分保证，只有一种情况，并且进入一次，采用多个flag
            if((str_rev_flag == END) &&(First_dou_flag == OK) && (temp == ',') && (client_num_flag == BEGIN) && (str_len_flag == END))//再一次接收到了逗号，开始接收字符串长度
            {
                str_len_flag = BEGIN;
                client_num_flag = END;
            }

            //第一个逗号来临，进入，以后不再进入
            if((str_rev_flag == END) &&(First_dou_flag == NO) && (temp == ',') && (client_num_flag == END))//逗号来临
            {
                client_num_flag = BEGIN;
                First_dou_flag = OK;//是第一个逗号
            }

            //开始接收用户连接号
            if((str_rev_flag == END) && (temp != ',') && (client_num_flag == BEGIN))
            {
                client_num = client_num * 10 + (temp - '0');
            }

            //开始接收字符串
            if(str_rev_flag == BEGIN)
            {
                //保存字符串到全局变量中，以便后面输出
                Get_str[Get_str_loc] = temp;
                Get_str_loc ++;
                //如果字符串的长度和刚刚接收到的指明字符串长度相同，则不再接收，做收尾工作
                if(Get_str_loc == str_len)
                {
                    Get_str[Get_str_loc] = '\0';
                    Str_Ready = OK;//设置标志位，说明我已经接收到了一个整的字符串了，可以进行操作了。
                    Rev_Str_status = END;//清空接收字符串标志位，使得可以再次进入下面switch循环
                    client_num_flag = END;//清空接收client_num标志位
                    str_len_flag = END;//清空接收str_len字符串长度标志位
                    str_rev_flag = END;//清空接收真正字符串标志位
                    First_dou_flag = NO;//清空区分第几个逗号的标志位
                }
            }

            if(temp == ':')//要是开始了：，那么后面开始接收字符长度为str_len的字符串长度
            {
                str_rev_flag = BEGIN;
            }
        }

        /************************************************/

        /************************************************/

        //要在接收字符串的标志位无效，并且接收状态位有效的时候才做
        if((Rev_Str_status == END) && (Rev_status == BEGIN))
        {
            //检测到第一个以后，立刻置接收标志位无效暂时不接收
            switch(temp)
            {
                case '+'://要么接收到+IPD 要么接收到 +CIFSR
                {
                    Rev_Str_status = BEGIN;//开始接收，暂时不进入这个switch循环
                    Rev_status = END;
                    break;
                }
                case 'E'://发送失败，回显ERROR
                {
                    Send_flag = NO;
                    Rev_status = END;
                    break;
                }
                case 'S'://发送成功，回显SEND OK
                {
                    Send_flag = OK;
                    Rev_status = END;
                    break;
                }
                default:
                {
                    Rev_status = END;
                    break;
                }
            }
        }
    }
    //软复位时使用
    if(0x7f == temp)//special for Doflye
    {
        i ++;
        if(10 == i)
        {
            i = 0;
            ISP_CONTR = 0xe0;
        }
    }
    else
    {
        i = 0;
    }
    RI = 0;
    ES = 1;

}

