/*
*********************************************************************************************************
*
*
*                                       ESP8266 Module's Generic Function Interface
*                                         ESP8266ģ���ͨ�ú����ӿ�
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
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include "common.h"
#include "bsp_delay.h"


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
*                                       VARIABLES           ����
*********************************************************************************************************
*/
_ESP8266 ESP8266;


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
static __inline ESP8266_Error send_cmd_common()
{
    ESP8266_Error errorstatus=ESP8266_ACK_FAIL;

    ESP8266.result = false;                                         ///< ���ȷ�Ͻ��
    ESP8266.UART._send( ESP8266.command,strlen(ESP8266.command) );  ///< ͨ�����ڷ���ATָ��

    if( ESP8266.waittime!=0 )                                       ///< �ȴ���Ӧ
    {
        while( --ESP8266.waittime )                                 ///< �ȴ�����ʱ
        {
            delay_ms(10);
            if( ESP8266.result == true ) {
                errorstatus = ESP8266_OK;
                break;                                              ///< �õ���ȷ��ȷ����Ϣ
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

    ESP8266.result = false;                     ///< ���ȷ�Ͻ��
    ESP8266.UART._send( pSendBuf,sendlen );     ///< ͨ�����ڷ���ATָ��

    if( ESP8266.waittime!=0 )                   ///< �ȴ���Ӧ
    {
        while( --ESP8266.waittime )             ///< �ȴ�����ʱ
        {
            delay_ms(10);
            if( ESP8266.result == true ) {
                errorstatus = ESP8266_OK;
                break;                          ///< �õ���ȷ��ȷ����Ϣ
            }
        }
    }

    return errorstatus;
}



/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
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
        if( mode==single )  ///< �л���������ģʽ
        {
            if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPCLOSE,5 ) )      ///< �ر��������ӣ���������û�����ӽ���������£���������ģʽ��
                return ESP8266_ACK_FAIL;

            if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPSERVER0,0 ) )    ///< �رշ������������������TCP�����������л�Ϊ�����ӣ�����رշ�������
                return ESP8266_ACK_FAIL;

            errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPMUX,0 );           ///< ����Ϊ������
        } else              ///< �л���������ģʽ
        {
            if( ESP8266_OK!=esp_8266_at_queryCmd( 30,queAT_CIPMODE ) )
                return errorstatus;

            if( 1==ESP8266.trans_mode ) {
                if( ESP8266_OK!=esp_8266_at_setupCmd( 30,setAT_CIPMODE,0 ) )   ///< ֻ���ڷ�͸��ģʽ����AT+CIPMODE=0�������������ö�����
                    return ESP8266_ACK_FAIL;
            }

            errorstatus = esp_8266_at_setupCmd( 30,setAT_CIPMUX,1 );           ///< ����Ϊ������
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
    //loc������ret_msgȫ�ֱ�����ƫ�ƣ�������װһ���ַ���
    static unsigned char i = 0;
    unsigned char temp;

    ES = 0;
    temp = SBUF;//temp���ܱ��ı䣬��Ϊ��λ��Ҫ�õ�

    /*ESP8266��ȡ�ַ�������*/
    if(temp == '\n')//��ʼ��
    {
        Rev_status = BEGIN;//���ÿ�ʼ����
    }
    else if(temp == '\r')//������
    {
        Rev_status = END;//���ý�������
    }
    else//����\r\n������ַ�
    {

        /************************************************/

        //ר����������IPD��CIFSR����������ʱҪ��Rev_Str_status��Ϊ��Ч���ٴν��������ѭ��������һ���ַ�
        if(Rev_Str_status == BEGIN)
        {
            if((str_rev_flag == END) &&(temp != ':') && (str_len_flag == BEGIN))//��ʼ�����ַ�������
            {
                str_len = str_len * 10 + (temp - '0');
            }

            //Ϊ�˳�ֱ�֤��ֻ��һ����������ҽ���һ�Σ����ö��flag
            if((str_rev_flag == END) &&(First_dou_flag == OK) && (temp == ',') && (client_num_flag == BEGIN) && (str_len_flag == END))//��һ�ν��յ��˶��ţ���ʼ�����ַ�������
            {
                str_len_flag = BEGIN;
                client_num_flag = END;
            }

            //��һ���������٣����룬�Ժ��ٽ���
            if((str_rev_flag == END) &&(First_dou_flag == NO) && (temp == ',') && (client_num_flag == END))//��������
            {
                client_num_flag = BEGIN;
                First_dou_flag = OK;//�ǵ�һ������
            }

            //��ʼ�����û����Ӻ�
            if((str_rev_flag == END) && (temp != ',') && (client_num_flag == BEGIN))
            {
                client_num = client_num * 10 + (temp - '0');
            }

            //��ʼ�����ַ���
            if(str_rev_flag == BEGIN)
            {
                //�����ַ�����ȫ�ֱ����У��Ա�������
                Get_str[Get_str_loc] = temp;
                Get_str_loc ++;
                //����ַ����ĳ��Ⱥ͸ոս��յ���ָ���ַ���������ͬ�����ٽ��գ�����β����
                if(Get_str_loc == str_len)
                {
                    Get_str[Get_str_loc] = '\0';
                    Str_Ready = OK;//���ñ�־λ��˵�����Ѿ����յ���һ�������ַ����ˣ����Խ��в����ˡ�
                    Rev_Str_status = END;//��ս����ַ�����־λ��ʹ�ÿ����ٴν�������switchѭ��
                    client_num_flag = END;//��ս���client_num��־λ
                    str_len_flag = END;//��ս���str_len�ַ������ȱ�־λ
                    str_rev_flag = END;//��ս��������ַ�����־λ
                    First_dou_flag = NO;//������ֵڼ������ŵı�־λ
                }
            }

            if(temp == ':')//Ҫ�ǿ�ʼ�ˣ�����ô���濪ʼ�����ַ�����Ϊstr_len���ַ�������
            {
                str_rev_flag = BEGIN;
            }
        }

        /************************************************/

        /************************************************/

        //Ҫ�ڽ����ַ����ı�־λ��Ч�����ҽ���״̬λ��Ч��ʱ�����
        if((Rev_Str_status == END) && (Rev_status == BEGIN))
        {
            //��⵽��һ���Ժ������ý��ձ�־λ��Ч��ʱ������
            switch(temp)
            {
                case '+'://Ҫô���յ�+IPD Ҫô���յ� +CIFSR
                {
                    Rev_Str_status = BEGIN;//��ʼ���գ���ʱ���������switchѭ��
                    Rev_status = END;
                    break;
                }
                case 'E'://����ʧ�ܣ�����ERROR
                {
                    Send_flag = NO;
                    Rev_status = END;
                    break;
                }
                case 'S'://���ͳɹ�������SEND OK
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
    //��λʱʹ��
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

