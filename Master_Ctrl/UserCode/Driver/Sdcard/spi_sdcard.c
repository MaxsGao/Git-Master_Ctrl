/*
*********************************************************************************************************
*
*
*                                               SPI SD
*                                         挂在SPI总线上的SD卡
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
*                                       INCLUDES            头文件
*********************************************************************************************************
*/
#include "spi_sdcard.h"


/*
*********************************************************************************************************
*                                       STATEMENT           声明
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
*                                       CONSTANTS           常量
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       VARIABLES           变量
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
*                                       LOCAL FUNCTIONS     静态函数
*********************************************************************************************************
*/
/**
 * @brief   取消对SD卡的片选，并提供额外的8个CLK
 * @param   None
 * @ret     None
 */
void SPI_SD_DisSelect(void)
{
    SPI_SD.CS._deselect();            //取消片选
    SPI_SD.SPI._write_byte(0xff);   //提供额外的8个时钟
}

/**@brief   片选SD卡,并且等待卡准备完毕
    *@param     None
    *@ret       SPI_SD_Error:SPI_SD_OK,成功;SPI_SD_WAIT_READY_TIMEOUT,失败;
    */
SPI_SD_Error SPI_SD_Select(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    SPI_SD.CS._select();

    //等待SD卡准备就绪
    errorstatus = SPI_SD_WaitReady();
    if( errorstatus!=SPI_SD_OK ) {
        SPI_SD_DisSelect();//SD卡未准备就绪，取消片选
    }

    return errorstatus;
}

/**@brief   等待卡响应
	*@param     Response:要得到的响应值
	*@ret       SPI_SD_Error:0,成功得到了该回应值;其他,得到回应值失败;
	*/
SPI_SD_Error SPI_SD_GetResponse(uint8_t Response)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    uint16_t Retry=0xFFFF;//等待次数

    while ((SPI_SD.SPI._read_byte()!=Response)&&Retry) {
        Retry--;//等待得到准确的回应
    }

    if (Retry==0) {
        errorstatus = SPI_SD_CMD_RSP_TIMEOUT;//回应失败
    }

    return errorstatus;
}

/**@brief   读取一个数据包
	*@param     buf:数据缓存区
                len:要读取的数据长度.
	*@ret       SPI_SD_Error:0,成功;其他,失败;
	*/
SPI_SD_Error SPI_SD_RecvData(uint8_t* buf,uint16_t len)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;

    //等待SD卡发回数据起始令牌0xFE
    errorstatus = SPI_SD_GetResponse(0xFE);
    if( errorstatus==SPI_SD_OK ) {
        //开始接收数据
        while(len--)
        {
            *buf=SPI_SD.SPI._read_byte();
            buf++;
        }

        //下面是2个伪CRC（dummy CRC）
        SPI_SD.SPI._read_byte();
        SPI_SD.SPI._read_byte();
    }
    else {
        errorstatus = SPI_SD_NOTGET_START_TOKEN;
    }

    return errorstatus;
}

/**@brief   向sd卡写入一个数据包的内容 512字节
	*@param     buf:数据缓存区
                cmd:指令
	*@ret       SPI_SD_Error:0,成功;其他,失败;
	*/
SPI_SD_Error SPI_SD_SendBlock(uint8_t* buf,uint8_t cmd)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint16_t t;

    //等待准备失效
    errorstatus = SPI_SD_WaitReady();
    if( errorstatus==SPI_SD_OK ) {
        //发送写数据起始令牌0XFE
        SPI_SD.SPI._write_byte(cmd);

        //不是结束指令
        if( cmd!=0XFD )
        {
            for(t=0; t<512; t++) {
                SPI_SD.SPI._write_byte( buf[t] );//提高速度,减少函数传参时间
            }

            SPI_SD.SPI._write_byte( 0xFF );
            SPI_SD.SPI._write_byte( 0xFF );

            if( (SPI_SD.SPI._read_byte()&0x1F)!=0x05 ) {
                errorstatus = SPI_SD_TX_RSP_ERROR;//响应错误
            }
        }
    }
    return errorstatus;//写入成功
}

/**@brief   向SD卡发送一个CMD命令
	*@param     cmd：命令索引
                arg：命令参数
	*@ret       uint8_t：SD卡返回的响应
	*/
uint8_t SPI_SD_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t Rsp;
    uint8_t Retry=0;

    SPI_SD_DisSelect();//取消上次片选
    if( SPI_SD_Select()!=SPI_SD_OK ) {
        return 0xFF;//片选失效响应
    }

    //发送
    SPI_SD.SPI._write_byte(cmd | 0x40);//分别写入命令
    SPI_SD.SPI._write_byte(arg >> 24);
    SPI_SD.SPI._write_byte(arg >> 16);
    SPI_SD.SPI._write_byte(arg >> 8);
    SPI_SD.SPI._write_byte(arg);
    SPI_SD.SPI._write_byte(crc);

    if(cmd==CMD12) {
        SPI_SD.SPI._read_byte();//Skip a stuff byte when stop reading
    }

    //等待响应，或超时退出
    Retry=0X1F;
    do
    {
        Rsp = SPI_SD.SPI._read_byte();
    } while( (Rsp&0X80) && Retry-- );

    //返回状态值
    return Rsp;
}

/**@brief   获取SD卡的CID信息，包括制造商信息
	*@param     None
	*@ret       SPI_SD_Error：0,成功;其他,失败
	*/
SPI_SD_Error SD_GetCID(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint8_t Rsp;

    //发CMD10命令，读CID
    Rsp = SPI_SD_SendCmd(CMD10,0,0x01);
    if( Rsp==0x00 )
    {
        errorstatus = SPI_SD_RecvData( SPI_SD.cid,sizeof(SPI_SD.cid) );//接收16个字节的数据
    } else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }

    SPI_SD_DisSelect();//取消片选
    return errorstatus;
}

/**@brief   获取SD卡的CSD信息，包括容量和速度信息
	*@param     None
	*@ret       SPI_SD_Error：0,成功;其他,失败
	*/
SPI_SD_Error SPI_SD_GetCSD(void)
{
    SPI_SD_Error errorstatus=SPI_SD_OK;
    uint8_t Rsp;

    //发CMD9命令，读CSD
    Rsp = SPI_SD_SendCmd( CMD9,0,0x01 );
    if( Rsp == 0x00 ) {
        errorstatus = SPI_SD_RecvData( SPI_SD.csd,sizeof(SPI_SD.csd) );//接收16个字节的数据
    } else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }


    SPI_SD_DisSelect();//取消片选

    return errorstatus;
}


/*
*********************************************************************************************************
*                                       API FUNCTIONS       API接口函数
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

    uint8_t     Rsp;        // 存放SD卡的返回值
    uint16_t    Retry;      // 用来进行超时计数
    uint8_t     buf[4];
    uint8_t     i;


    ///> 1、初始化与sd卡连接的硬件条件（MCU的SPI配置，IO配置）
    SPI_SD.SPI._set_low_speed();

    ///> 2、上电延时（>74个CLK）
    for(i=0; i<10; i++) {
        SPI_SD.SPI._write_byte(0xFF);//发送最少74个脉冲
    }

    ///> 3、复位卡（CMD0），进入IDLE状态
    Retry=20;
    do
    {
        Rsp=SPI_SD_SendCmd( CMD0,0,0x95 );//进入IDLE状态
    } while( (Rsp!=0X01) && Retry-- );

    SPI_SD.type = 0;//默认无卡
    if( Rsp==0X01 )
    {
        ///> 4、发送CMD8，检查是否支持2.0协议
        Rsp = SPI_SD_SendCmd(CMD8,0x1AA,0x87);
        if( Rsp==1 )//SD V2.0
        {
            ///> 5、根据不同协议检查SD卡（命令包括：CMD55、CMD41、CMD58和CMD1等）
            for( i=0; i<4; i++ )
                buf[i]=SPI_SD.SPI._read_byte();	//Get trailing return value of R7 resp

            if( buf[2]==0X01&&buf[3]==0XAA )//卡是否支持2.7~3.6V
            {
                Retry = 0XFFFE;
                do
                {
                    SPI_SD_SendCmd( CMD55,0,0X01 );	//发送CMD55
                    Rsp = SPI_SD_SendCmd( CMD41,0x40000000,0X01 );//发送CMD41
                } while( Rsp&&Retry-- );

                Rsp = SPI_SD_SendCmd( CMD58,0,0X01);
                if( Retry&&Rsp==0 )//鉴别SD2.0卡版本开始
                {
                    for( i=0; i<4; i++ )
                        buf[i]=SPI_SD.SPI._read_byte();//得到OCR值

                    if( buf[0]&0x40 )
                        SPI_SD.type = SD_TYPE_V2HC;    //检查CCS

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
            ///> 5、根据不同协议检查SD卡（命令包括：CMD55、CMD41、CMD58和CMD1等）
            SPI_SD_SendCmd( CMD55,0,0X01 );		//发送CMD55
            Rsp = SPI_SD_SendCmd( CMD41,0,0X01 );	//发送CMD41
            if( Rsp<=1 )
            {
                SPI_SD.type = SD_TYPE_V1;
                Retry = 0XFFFE;
                do //等待退出IDLE模式
                {
                    SPI_SD_SendCmd( CMD55,0,0X01 );	//发送CMD55
                    Rsp = SPI_SD_SendCmd( CMD41,0,0X01 );//发送CMD41
                } while( Rsp&&Retry--);
            }
            else//MMC卡不支持CMD55+CMD41识别
            {
                SPI_SD.type = SD_TYPE_MMC;//MMC V3
                Retry=0XFFFE;
                do //等待退出IDLE模式
                {
                    Rsp = SPI_SD_SendCmd( CMD1,0,0X01 );//发送CMD1
                } while( Rsp&&Retry-- );
            }

            Rsp = SPI_SD_SendCmd(CMD16,512,0X01);
            if( Retry==0||Rsp!=0 )
            {
                SPI_SD.type = SD_TYPE_ERR;//错误的卡
                errorstatus = SPI_SD_CMD_RSP_ERROR;
            }
        }
    }
    else
    {
        errorstatus = SPI_SD_CMD_RSP_ERROR;
    }

    ///> 6、取消片选，发多8个CLK，结束初始化
    SPI_SD_DisSelect();

    SPI_SD.SPI._set_high_speed();//高速

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
    } while(Retry<0XFFFFFF);//等待

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
        sector <<= 9;//转换为字节地址

    if( cnt==1 )
    {
        ///> 1、发送CMD17
        Rsp = SPI_SD_SendCmd( CMD17,sector,0X01 );//读命令

        ///> 2、接收卡响应R1
        if( Rsp==0 )//指令发送成功
        {
            ///> 3、接收数据起始令牌0XFE
            ///> 4、接收数据
            ///> 5、接收2个字节的CRC，如果不使用CRC，这两个字节在读取后可以丢掉
            errorstatus = SPI_SD_RecvData( buf,512 );   //接收512个字节
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    } else
    {
        Rsp = SPI_SD_SendCmd( CMD18,sector,0X01 );//连续读命令
        if( Rsp==0 )
        {
            do
            {
                errorstatus = SPI_SD_RecvData( buf,512 );//接收512个字节
                buf += 512;
            } while( --cnt && errorstatus==SPI_SD_OK );
            SPI_SD_SendCmd( CMD12,0,0X01 );	//发送停止命令
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    }

    ///> 6、禁止片选之后，发多8个CLK
    SPI_SD_DisSelect();//取消片选

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
        sector *= 512;//转换为字节地址

    if(cnt==1)
    {
        ///> 1、发送CMD24
        Rsp = SPI_SD_SendCmd( CMD24,sector,0X01 );//读命令

        ///> 2、接收卡响应R1
        if( Rsp==0 )//指令发送成功
        {
            ///> 3、发送写数据起始令牌0XFE
            ///> 4、发送数据
            ///> 5、发送2字节的伪CRC
            errorstatus = SPI_SD_SendBlock( buf,0xFE );//写512个字节
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }
    } else
    {
        if( SPI_SD.type!=SD_TYPE_MMC )
        {
            SPI_SD_SendCmd( CMD55,0,0X01 );
            SPI_SD_SendCmd( CMD23,cnt,0X01 );//发送指令
        }
        Rsp = SPI_SD_SendCmd(CMD25,sector,0X01);//连续读命令
        if( Rsp==0 )
        {
            do
            {
                errorstatus = SPI_SD_SendBlock( buf,0xFC );//接收512个字节
                buf+=512;
            } while( --cnt && errorstatus==SPI_SD_OK );
            errorstatus = SPI_SD_SendBlock( 0,0xFD );//接收512个字节
        } else
        {
            errorstatus = SPI_SD_CMD_RSP_ERROR;
        }

    }

    SPI_SD_DisSelect();//取消片选

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

    //取CSD信息，如果期间出错，返回0
    errorstatus = SPI_SD_GetCSD();
    if( errorstatus ==SPI_SD_OK )
    {
        //如果为SDHC卡，按照下面方式计算
        if( (SPI_SD.csd[0]&0xC0)==0x40 )	 //V2.00的卡
        {
            csize = SPI_SD.csd[9] + ((uint16_t)SPI_SD.csd[8] << 8) + 1;
            SPI_SD.capacity = (uint32_t)csize << 10;//得到扇区数
        }
        else//V1.XX的卡
        {
            n = (SPI_SD.csd[5] & 15) + ((SPI_SD.csd[10] & 128) >> 7) + ((SPI_SD.csd[9] & 3) << 1) + 2;
            csize = (SPI_SD.csd[8] >> 6) + ((uint16_t)SPI_SD.csd[7] << 2) + ((uint16_t)(SPI_SD.csd[6] & 3) << 10) + 1;
            SPI_SD.capacity = (uint32_t)csize << (n - 9);//得到扇区数
        }
    }

    return errorstatus;
}

