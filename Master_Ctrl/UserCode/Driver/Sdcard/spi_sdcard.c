/*
*********************************************************************************************************
*
*
*                                               SPI SD
*                                         ����SPI�����ϵ�SD��
* File : SPI_SD.h
* By   : Bear
* Date : 2018/11/15
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
#include "spi_sdcard.h"


/*
*********************************************************************************************************
*                                       STATEMENT           ����
*********************************************************************************************************
*/
void    spi_sdcard_cris_enter(void)         {};
void    spi_sdcard_cris_exit(void)          {};
void    spi_sdcard_cs_select(void)          {};
void    spi_sdcard_cs_deselect(void)        {};
void    spi_sdcard_spi_setlowspeed(void)    {};
void    spi_sdcard_spi_sethighspeed(void)   {};
uint8_t spi_sdcard_spi_readbyte(void)       {
    return 0;
};
void    spi_sdcard_spi_writebyte(uint8_t wb) {};


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
/**
 * @\ref _SPI_SD instance
 */
_SPI_SD SPI_SD =
{
    .type                   = 0,
    .cid                    = 0,
    .csd                    = 0,
    .CRIS._enter            = spi_sdcard_cris_enter,
    .CRIS._exit             = spi_sdcard_cris_exit,
    .CS._select             = spi_sdcard_cs_select,
    .CS._deselect           = spi_sdcard_cs_deselect,
    .SPI._set_low_speed     = spi_sdcard_spi_setlowspeed,
    .SPI._set_high_speed    = spi_sdcard_spi_sethighspeed,
    .SPI._read_byte         = spi_sdcard_spi_readbyte,
    .SPI._write_byte        = spi_sdcard_spi_writebyte
};


/*
*********************************************************************************************************
*                                       LOCAL FUNCTIONS     ��̬����
*********************************************************************************************************
*/
/**
 * @brief   ȡ����SD����Ƭѡ�����ṩ�����8��CLK
 * @param   None
 * @ret     None
 */
void SPI_SD_DisSelect(void)
{
    SPI_SD.CS._deselect();            //ȡ��Ƭѡ
    SPI_SD.SPI._write_byte(0xff);   //�ṩ�����8��ʱ��
}

/**@brief   ƬѡSD��,���ҵȴ���׼�����
    *@param     None
    *@ret       SPI_SD_Error:SPI_SD_OK,�ɹ�;SPI_SD_WAIT_READY_TIMEOUT,ʧ��;
    */
SPI_SD_Error SPI_SD_Select(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    SPI_SD.CS._select();

    //�ȴ�SD��׼������
    errorstatus = SPI_SD_WaitReady();
    if( errorstatus!=SPI_SD_OK ) {
        SPI_SD_DisSelect();//SD��δ׼��������ȡ��Ƭѡ
    }

    return errorstatus;
}

/**@brief   �ȴ�����Ӧ
	*@param     Response:Ҫ�õ�����Ӧֵ
	*@ret       SPI_SD_Error:0,�ɹ��õ��˸û�Ӧֵ;����,�õ���Ӧֵʧ��;
	*/
SPI_SD_Error SPI_SD_GetResponse(uint8_t Response)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    uint16_t Retry=0xFFFF;//�ȴ�����

    while ((SPI_SD.SPI._read_byte()!=Response)&&Retry) {
        Retry--;//�ȴ��õ�׼ȷ�Ļ�Ӧ
    }

    if (Retry==0) {
        errorstatus = SPI_SD_CMD_RSP_TIMEOUT;//��Ӧʧ��
    }

    return errorstatus;
}

/**@brief   ��ȡһ�����ݰ�
	*@param     buf:���ݻ�����
                len:Ҫ��ȡ�����ݳ���.
	*@ret       SPI_SD_Error:0,�ɹ�;����,ʧ��;
	*/
SPI_SD_Error SPI_SD_RecvData(uint8_t* buf,uint16_t len)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    //�ȴ�SD������������ʼ����0xFE
    errorstatus = SPI_SD_GetResponse(0xFE);
    if( errorstatus==SPI_SD_OK ) {
        //��ʼ��������
        while(len--)
        {
            *buf=SPI_SD.SPI._read_byte();
            buf++;
        }

        //������2��αCRC��dummy CRC��
        SPI_SD.SPI._read_byte();
        SPI_SD.SPI._read_byte();
    }
    else {
        errorstatus = SPI_SD_NOTGET_START_TOKEN;
    }

    return errorstatus;
}

/**@brief   ��sd��д��һ�����ݰ������� 512�ֽ�
	*@param     buf:���ݻ�����
                cmd:ָ��
	*@ret       SPI_SD_Error:0,�ɹ�;����,ʧ��;
	*/
SPI_SD_Error SPI_SD_SendBlock(uint8_t* buf,uint8_t cmd)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint16_t t;

    //�ȴ�׼��ʧЧ
    errorstatus = SPI_SD_WaitReady();
    if( errorstatus==SPI_SD_OK ) {
        //����д������ʼ����0XFE
        SPI_SD.SPI._write_byte(cmd);

        //���ǽ���ָ��
        if( cmd!=0XFD )
        {
            for(t=0; t<512; t++) {
                SPI_SD.SPI._write_byte( buf[t] );//����ٶ�,���ٺ�������ʱ��
            }

            SPI_SD.SPI._write_byte( 0xFF );
            SPI_SD.SPI._write_byte( 0xFF );

            if( (SPI_SD.SPI._read_byte()&0x1F)!=0x05 ) {
                errorstatus = SPI_SD_TX_RSP_ERROR;//��Ӧ����
            }
        }
    }
    return errorstatus;//д��ɹ�
}

/**@brief   ��SD������һ��CMD����
	*@param     cmd����������
                arg���������
	*@ret       uint8_t��SD�����ص���Ӧ
	*/
uint8_t SPI_SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t Rsp;
    uint8_t Retry=0;

    SPI_SD_DisSelect();//ȡ���ϴ�Ƭѡ
    if( SPI_SD_Select()!=SPI_SD_OK ) {
        return 0xFF;//ƬѡʧЧ��Ӧ
    }

    //����
    SPI_SD.SPI._write_byte(cmd | 0x40);//�ֱ�д������
    SPI_SD.SPI._write_byte(arg >> 24);
    SPI_SD.SPI._write_byte(arg >> 16);
    SPI_SD.SPI._write_byte(arg >> 8);
    SPI_SD.SPI._write_byte(arg);
    SPI_SD.SPI._write_byte(crc);

    if(cmd==CMD12) {
        SPI_SD.SPI._read_byte();//Skip a stuff byte when stop reading
    }

    //�ȴ���Ӧ����ʱ�˳�
    Retry=0X1F;
    do
    {
        Rsp = SPI_SD.SPI._read_byte();
    } while( (Rsp&0X80) && Retry-- );

    //����״ֵ̬
    return Rsp;
}

/**@brief   ��ȡSD����CID��Ϣ��������������Ϣ
	*@param     None
	*@ret       SPI_SD_Error��0,�ɹ�;����,ʧ��
	*/
SPI_SD_Error SD_GetCID(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint8_t Rsp;

    //��CMD10�����CID
    Rsp = SPI_SD_SendCmd(CMD10,0,0x01);
    if( Rsp==0x00 )
    {
        errorstatus = SPI_SD_RecvData( SPI_SD.cid,sizeof(SPI_SD.cid) );//����16���ֽڵ�����
    } else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }

    SPI_SD_DisSelect();//ȡ��Ƭѡ
    return errorstatus;
}

/**@brief   ��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
	*@param     None
	*@ret       SPI_SD_Error��0,�ɹ�;����,ʧ��
	*/
SPI_SD_Error SPI_SD_GetCSD(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint8_t Rsp;

    //��CMD9�����CSD
    Rsp = SPI_SD_SendCmd( CMD9,0,0x01 );
    if( Rsp == 0x00 ) {
        errorstatus = SPI_SD_RecvData( SPI_SD.csd,sizeof(SPI_SD.csd) );//����16���ֽڵ�����
    } else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }


    SPI_SD_DisSelect();//ȡ��Ƭѡ

    return errorstatus;
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API�ӿں���
*********************************************************************************************************
*/
/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
void reg_spi_sdcard_cris_cbfunc (void(*cris_en)(void), void(*cris_ex)(void))
{
    if( !cris_en || !cris_ex ) {
        SPI_SD.CRIS._enter  = spi_sdcard_cris_enter;
        SPI_SD.CRIS._exit   = spi_sdcard_cris_exit;
    }
    else {
        SPI_SD.CRIS._enter  = cris_en;
        SPI_SD.CRIS._exit   = cris_ex;
    }
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
void reg_spi_sdcard_cs_cbfunc   (void(*cs_sel)(void), void(*cs_desel)(void))
{
    if( !cs_sel || !cs_desel ) {
        SPI_SD.CS._select   = spi_sdcard_cs_select;
        SPI_SD.CS._deselect = spi_sdcard_cs_deselect;
    }
    else {
        SPI_SD.CS._select   = cs_sel;
        SPI_SD.CS._deselect = cs_desel;
    }
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
void reg_spi_sdcard_spi_cbfunc  (void(*spi_sls)(void), void(*spi_shs)(void), uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb))
{
    if( !spi_sls || !spi_shs || !spi_rb || !spi_wb ) {
        SPI_SD.SPI._set_low_speed   = spi_sdcard_spi_setlowspeed;
        SPI_SD.SPI._set_high_speed  = spi_sdcard_spi_sethighspeed;
        SPI_SD.SPI._read_byte       = spi_sdcard_spi_readbyte;
        SPI_SD.SPI._write_byte      = spi_sdcard_spi_writebyte;
    }
    else {
        SPI_SD.SPI._set_low_speed   = spi_sls;
        SPI_SD.SPI._set_high_speed  = spi_shs;
        SPI_SD.SPI._read_byte       = spi_rb;
        SPI_SD.SPI._write_byte      = spi_wb;
    }
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
SPI_SD_Error SPI_SD_Init(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    uint8_t     Rsp;        // ���SD���ķ���ֵ
    uint16_t    Retry;      // �������г�ʱ����
    uint8_t     buf[4];
    uint8_t     i;


    ///> 1����ʼ����sd�����ӵ�Ӳ��������MCU��SPI���ã�IO���ã�
    SPI_SD.SPI._set_low_speed();

    ///> 2���ϵ���ʱ��>74��CLK��
    for(i=0; i<10; i++) {
        SPI_SD.SPI._write_byte(0xFF);//��������74������
    }

    ///> 3����λ����CMD0��������IDLE״̬
    Retry=20;
    do
    {
        Rsp=SPI_SD_SendCmd( CMD0,0,0x95 );//����IDLE״̬
    } while( (Rsp!=0X01) && Retry-- );

    SPI_SD.type = 0;//Ĭ���޿�
    if( Rsp==0X01 )
    {
        ///> 4������CMD8������Ƿ�֧��2.0Э��
        Rsp = SPI_SD_SendCmd(CMD8,0x1AA,0x87);
        if( Rsp==1 )//SD V2.0
        {
            ///> 5�����ݲ�ͬЭ����SD�������������CMD55��CMD41��CMD58��CMD1�ȣ�
            for( i=0; i<4; i++ )
                buf[i]=SPI_SD.SPI._read_byte();	//Get trailing return value of R7 resp

            if( buf[2]==0X01&&buf[3]==0XAA )//���Ƿ�֧��2.7~3.6V
            {
                Retry = 0XFFFE;
                do
                {
                    SPI_SD_SendCmd( CMD55,0,0X01 );	//����CMD55
                    Rsp = SPI_SD_SendCmd( CMD41,0x40000000,0X01 );//����CMD41
                } while( Rsp&&Retry-- );

                Rsp = SPI_SD_SendCmd( CMD58,0,0X01);
                if( Retry&&Rsp==0 )//����SD2.0���汾��ʼ
                {
                    for( i=0; i<4; i++ )
                        buf[i]=SPI_SD.SPI._read_byte();//�õ�OCRֵ

                    if( buf[0]&0x40 )
                        SPI_SD.type = SD_TYPE_V2HC;    //���CCS

                    else
                        SPI_SD.type = SD_TYPE_V2;

                }
                else
                {
                    errorstatus = SPI_SD_CMD_RSP_ERROR;
                }
            }
            else
            {
                errorstatus = SPI_SD_ERROR;
            }
        }
        else//SD V1.x/ MMC	V3
        {
            ///> 5�����ݲ�ͬЭ����SD�������������CMD55��CMD41��CMD58��CMD1�ȣ�
            SPI_SD_SendCmd( CMD55,0,0X01 );		//����CMD55
            Rsp = SPI_SD_SendCmd( CMD41,0,0X01 );	//����CMD41
            if( Rsp<=1 )
            {
                SPI_SD.type = SD_TYPE_V1;
                Retry = 0XFFFE;
                do //�ȴ��˳�IDLEģʽ
                {
                    SPI_SD_SendCmd( CMD55,0,0X01 );	//����CMD55
                    Rsp = SPI_SD_SendCmd( CMD41,0,0X01 );//����CMD41
                } while( Rsp&&Retry--);
            }
            else//MMC����֧��CMD55+CMD41ʶ��
            {
                SPI_SD.type = SD_TYPE_MMC;//MMC V3
                Retry=0XFFFE;
                do //�ȴ��˳�IDLEģʽ
                {
                    Rsp = SPI_SD_SendCmd( CMD1,0,0X01 );//����CMD1
                } while( Rsp&&Retry-- );
            }

            Rsp = SPI_SD_SendCmd(CMD16,512,0X01);
            if( Retry==0||Rsp!=0 )
            {
                SPI_SD.type = SD_TYPE_ERR;//����Ŀ�
                errorstatus = SPI_SD_CMD_RSP_ERROR;
            }
        }
    }
    else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }

    ///> 6��ȡ��Ƭѡ������8��CLK��������ʼ��
    SPI_SD_DisSelect();

    SPI_SD.SPI._set_high_speed();//����

    if( SPI_SD.type ) {
        errorstatus = SPI_SD_OK;
    }

    return errorstatus;
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
SPI_SD_Error SPI_SD_WaitReady(void)
{
    uint32_t Retry=0;

    do
    {
        if( SPI_SD.SPI._read_byte()==0XFF ) {
            return SPI_SD_OK;//OK
        }
        Retry++;
    } while(Retry<0XFFFFFF);//�ȴ�

    return SPI_SD_WAIT_READY_TIMEOUT;
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
SPI_SD_Error SPI_SD_ReadDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint8_t Rsp;

    if( SPI_SD.type!=SD_TYPE_V2HC )
        sector <<= 9;//ת��Ϊ�ֽڵ�ַ

    if( cnt==1 )
    {
        ///> 1������CMD17
        Rsp = SPI_SD_SendCmd( CMD17,sector,0X01 );//������

        ///> 2�����տ���ӦR1
        if( Rsp==0 )//ָ��ͳɹ�
        {
            ///> 3������������ʼ����0XFE
            ///> 4����������
            ///> 5������2���ֽڵ�CRC�������ʹ��CRC���������ֽ��ڶ�ȡ����Զ���
            errorstatus = SPI_SD_RecvData( buf,512 );   //����512���ֽ�
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    } else
    {
        Rsp = SPI_SD_SendCmd( CMD18,sector,0X01 );//����������
        if( Rsp==0 )
        {
            do
            {
                errorstatus = SPI_SD_RecvData( buf,512 );//����512���ֽ�
                buf += 512;
            } while( --cnt && errorstatus==SPI_SD_OK );
            SPI_SD_SendCmd( CMD12,0,0X01 );	//����ֹͣ����
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    }

    ///> 6����ֹƬѡ֮�󣬷���8��CLK
    SPI_SD_DisSelect();//ȡ��Ƭѡ

    return errorstatus;
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
SPI_SD_Error SPI_SD_WriteDisk(uint8_t*buf,uint32_t sector,uint8_t cnt)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    uint8_t Rsp;

    if( SPI_SD.type!=SD_TYPE_V2HC )
        sector *= 512;//ת��Ϊ�ֽڵ�ַ

    if(cnt==1)
    {
        ///> 1������CMD24
        Rsp = SPI_SD_SendCmd( CMD24,sector,0X01 );//������

        ///> 2�����տ���ӦR1
        if( Rsp==0 )//ָ��ͳɹ�
        {
            ///> 3������д������ʼ����0XFE
            ///> 4����������
            ///> 5������2�ֽڵ�αCRC
            errorstatus = SPI_SD_SendBlock( buf,0xFE );//д512���ֽ�
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    } else
    {
        if( SPI_SD.type!=SD_TYPE_MMC )
        {
            SPI_SD_SendCmd( CMD55,0,0X01 );
            SPI_SD_SendCmd( CMD23,cnt,0X01 );//����ָ��
        }
        Rsp = SPI_SD_SendCmd(CMD25,sector,0X01);//����������
        if( Rsp==0 )
        {
            do
            {
                errorstatus = SPI_SD_SendBlock( buf,0xFC );//����512���ֽ�
                buf+=512;
            } while( --cnt && errorstatus==SPI_SD_OK );
            errorstatus = SPI_SD_SendBlock( 0,0xFD );//����512���ֽ�
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }

    }

    SPI_SD_DisSelect();//ȡ��Ƭѡ

    return errorstatus;
}

/*
*********************************************************************************************************
*                                        reg_spi_sdcard_cris_cbfunc()
*
* Description :
*
* Arguments   :
*
* Return      :
*
*Note(s):
*********************************************************************************************************
*/
SPI_SD_Error SPI_SD_GetSectorCount(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    uint8_t n;
    uint16_t csize;

    //ȡCSD��Ϣ������ڼ��������0
    errorstatus = SPI_SD_GetCSD();
    if( errorstatus ==SPI_SD_OK )
    {
        //���ΪSDHC�����������淽ʽ����
        if( (SPI_SD.csd[0]&0xC0)==0x40 )	 //V2.00�Ŀ�
        {
            csize = SPI_SD.csd[9] + ((uint16_t)SPI_SD.csd[8] << 8) + 1;
            SPI_SD.capacity = (uint32_t)csize << 10;//�õ�������
        }
        else//V1.XX�Ŀ�
        {
            n = (SPI_SD.csd[5] & 15) + ((SPI_SD.csd[10] & 128) >> 7) + ((SPI_SD.csd[9] & 3) << 1) + 2;
            csize = (SPI_SD.csd[8] >> 6) + ((uint16_t)SPI_SD.csd[7] << 2) + ((uint16_t)(SPI_SD.csd[6] & 3) << 10) + 1;
            SPI_SD.capacity = (uint32_t)csize << (n - 9);//�õ�������
        }
    }

    return errorstatus;
}

