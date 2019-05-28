/*
*********************************************************************************************************
*
*
*                                    WirelessDataTransmission
*                                         ��������
* File : nrf24l01.h
* By   : 
* Date : 2018/12/6
* version : V0.1
* NOTE(s) : 
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __NRF24L01_H
#define   __NRF24L01_H


/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "queue.h"


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

#define _Si24R1  

/**
 * @brief NRF24L01 Command Operation
 */
#define R_REGISTER              0x00        ///< �����üĴ���,��5λΪ�������ļĴ�����ַ
#define W_REGISTER              0x20        ///< д���üĴ���,��5λΪ�Ĵ�����ַ
#define R_RX_PAYLOAD            0x61        ///< ��RX��Ч����,1~32�ֽ�
#define W_TX_PAYLOAD            0xA0        ///< дTX��Ч����,1~32�ֽ�
#define FLUSH_TX                0xE1        ///< ���TX FIFO�Ĵ���,����ģʽ����
#define FLUSH_RX                0xE2        ///< ���RX FIFO�Ĵ���,����ģʽ����
#define REUSE_TX_PL             0xE3        ///< ����ʹ����һ������,CEΪ��,���ݰ������Ϸ���
#ifdef _Si24R1  ///< {
    #define R_RX_PL_WID         0x60        ///< ��ȡ�յ��������ֽ���
    #define W_ACK_PAYLOAD       0xA8        ///< �����ڽ��շ���ͨ��PIPE PPP������ͨ��ACK����ʽ����ȥ�����������֡���ݴ���FIFO��
    #define W_TX_PAYLOAD_NOACK  0xB0        ///< �����ڷ���ģʽ��ʹ���������ͬʱ��Ҫ��AUTOACKλ��1  
#endif  ///< }
#define NOP                     0xFF        ///< �ղ���,����������״̬�Ĵ���	

//--------------------------------------Registers ADDR----------------------------------------------------//
/**
 * @brief ���üĴ���
 */
#define CONFIG          0x00 
typedef enum
{
    CONF_MASK_RX_DR     = (1<<6),   ///< �����ж����ο���
    CONF_MASK_TX_DS     = (1<<5),   ///< �����ж����ο���
    CONF_MASK_MAX_RT    = (1<<4),   ///< ����ط������ж����ο���
    CONF_EN_CRC         = (1<<3),   ///< ʹ��CRC�����EN_AA��ȫΪ��ʱ��EN_CRC����Ϊ1
    CONF_CRCO           = (1<<2),   ///< CRC�������ã�0��1byte��1��2byte
    CONF_PWR_UP         = (1<<1),   ///< �ض�/����ģʽ����
    CONF_PRIM_RX        = (1<<0),   ///< ����/�������ã�ֻ����Shutdown��Standby�¸��ģ�0������ģʽ��1������ģʽ
}config_type;

/**
 * @brief ʹ���Զ�ȷ��
 */
#define EN_AA           0x01
typedef enum
{
    ENAA_P5             = (1<<5),   ///< ʹ�����ݹܵ�5�Զ�ȷ��
    ENAA_P4             = (1<<4),   ///< ʹ�����ݹܵ�4�Զ�ȷ��
    ENAA_P3             = (1<<3),   ///< ʹ�����ݹܵ�3�Զ�ȷ��
    ENAA_P2             = (1<<2),   ///< ʹ�����ݹܵ�2�Զ�ȷ��
    ENAA_P1             = (1<<1),   ///< ʹ�����ݹܵ�1�Զ�ȷ��
    ENAA_P0             = (1<<0),   ///< ʹ�����ݹܵ�0�Զ�ȷ��
}en_aa_type;

/**
 * @brief ʹ�ܽ������ݹܵ���ַ
 */
#define EN_RXADDR       0x02  //���յ�ַ����,bit0~5,��Ӧͨ��0~5
typedef enum
{
    ERX_P5              = (1<<5),   ///< ʹ�����ݹܵ�5
    ERX_P4              = (1<<4),   ///< ʹ�����ݹܵ�4
    ERX_P3              = (1<<3),   ///< ʹ�����ݹܵ�3
    ERX_P2              = (1<<2),   ///< ʹ�����ݹܵ�2
    ERX_P1              = (1<<1),   ///< ʹ�����ݹܵ�1
    ERX_P0              = (1<<0),   ///< ʹ�����ݹܵ�0
}en_rxaddr_type;

/**
 * @brief ��ַ�������(��������ͨ��)
 */
#define SETUP_AW        0x03
typedef enum
{
    AW                  = (11<<0),   ///< ���䷽/���շ���ַ��ȣ�00������ֵ��01��3bytes��10��4bytes��11:5bytes
}setup_aw_type;

/**
 * @brief �Զ��ط�����
 */
#define SETUP_RETR      0x04
typedef enum
{  
    RETR_ARD            = (1111<<4),   ///< �Զ��ط���ʱ����
    RETR_ARC            = (1111<<0),   ///< ����Զ��ط�����
}setup_retr_type;

/**
 * @brief ��Ƶ�ŵ�������оƬ����ʱ���ŵ����ֱ��Ӧ1~125���ŵ����ŵ����Ϊ1MHz��Ĭ��Ϊ02��2402MHz
 */
#define RF_CH           0x05

/**
 * @brief ��Ƶ����
 */
#define RF_SETUP        0x06
typedef enum
{  
    RF_CONT_WAVE        = (1<<7),   ///< Ϊ��1��ʱ��ʹ�ܺ��ز�����ģʽ
    RF_DR_LOW           = (1<<5),   ///< ������Ƶ�����ʣ���RF_DR_HIGH��ͬ����
    RF_PLL_LOCK         = (1<<4),   ///< �����֣�����Ϊ0
    RF_DR_HIGH          = (1<<3),   ///< ������Ƶ�����ʣ�[LOW��HIGH]��00-1Mbps��01-2Mbps��10-250kbps��11-����
    RF_PWR              = (110<<0), ///< ����TX���书�ʣ�111:7dBm��110:4dBm��101:3dBm��100:1dBm��011:0dBm��010:-4dBm��001:-6dBm��000:-12dBm��
}rf_setup_type;

/**
 * @brief ״̬�Ĵ�����SPI������ʼ��״̬�Ĵ���ֵͨ��MISO���������
 */
#define STATUS          0x07
typedef enum
{  
    STA_RX_DR           = (1<<6),   ///< ���������ж�
    STA_TX_DS           = (1<<5),   ///< ���ݷ�������ж�
    STA_MAX_RT          = (1<<4),   ///< �ﵽ�����ط�
    STA_RX_P_NO         = (111<<3), ///< ��������ͨ����(���:6)
    STA_TX_FULL         = (1<<0),   ///< TX FIFO����־
}status_type;

/**
 * @brief ������ͳ��
 */
#define OBSERVE_TX      0x08
typedef enum
{
    TX_PLOS_CNT         = (1111<<4),    ///< ����������������Ϊ15���ı�RF_CH��PLOS_CNT��0��ʼ����
    TX_ARC_CNT          = (1111<<0),    ///< �ط�����������һ���°�ʱ��ARC_CNT��0��ʼ����
}observe_tx_type;

/**
 * @brief �ز���⣬�����ź�ǿ�ȼ��
 */
#ifdef _Si24R1  ///< {
    #define RSSI
    typedef enum
    {
        RF_RSSI         = (1<<0),   ///< �����ź�ǿ�ȣ�0�������ź�С��-60dBm
    }rssi_type;
#else
    #define CD              0x09
    typedef enum
    {  
        RF_CD           = (1<<0),   ///< �ز����
    }cd_type;
#endif  ///< }

/**
 * @brief ���ݹܵ�0�Ľ��յ�ַ�������Ϊ5bytes��LSByte����д�룬ͨ��SETUP_AW���õ�ַ��ȣ�
 */
#define RX_ADDR_P0      0x0A

/**
 * @brief ���ݹܵ�1�Ľ��յ�ַ�������Ϊ5bytes��LSByte����д�룬ͨ��SETUP_AW���õ�ַ��ȣ�
 */
#define RX_ADDR_P1      0x0B

/**
 * @brief ���ݹܵ�2�Ľ��յ�ַ������ֽڣ����յ�ַ���ֽ���RX_ADDR_P1[39:8]��ͬ
 */
#define RX_ADDR_P2      0x0C

/**
 * @brief ���ݹܵ�3�Ľ��յ�ַ������ֽڣ����յ�ַ���ֽ���RX_ADDR_P1[39:8]��ͬ
 */
#define RX_ADDR_P3      0x0D

/**
 * @brief ���ݹܵ�4�Ľ��յ�ַ������ֽڣ����յ�ַ���ֽ���RX_ADDR_P1[39:8]��ͬ
 */
#define RX_ADDR_P4      0x0E

/**
 * @brief ���ݹܵ�5�Ľ��յ�ַ������ֽڣ����յ�ַ���ֽ���RX_ADDR_P1[39:8]��ͬ
 */
#define RX_ADDR_P5      0x0F

/**
 * @brief ���䷽�ķ����ַ��LSByte����д�룩��������䷽��Ҫ��ACKȷ���źţ�����Ҫ����RX_ADDR_P0��ֵ����TX_ADDR����ʹ��ARQ
 */
#define TX_ADDR         0x10

/**
 * @brief �������ݹܵ�0�����ֽ�����1-32Bytes��
 */
#define RX_PW_P0        0x11

/**
 * @brief �������ݹܵ�1�����ֽ�����1-32Bytes��
 */
#define RX_PW_P1        0x12

/**
 * @brief �������ݹܵ�2�����ֽ�����1-32Bytes��
 */
#define RX_PW_P2        0x13

/**
 * @brief �������ݹܵ�3�����ֽ�����1-32Bytes��
 */
#define RX_PW_P3        0x14

/**
 * @brief �������ݹܵ�4�����ֽ�����1-32Bytes��
 */
#define RX_PW_P4        0x15

/**
 * @brief �������ݹܵ�5�����ֽ�����1-32Bytes��
 */
#define RX_PW_P5        0x16

/**
 * @brief FIFO״̬
 */
#define FIFO_STATUS     0x17
typedef enum
{  
    FIFO_TX_REUSE       = (1<<6),   ///< ֻ���ڷ���ˣ�FIFO�����������ã�����RESUSE_TX_PL����󣬷����ϴ��ѳɹ���������ݣ�ͨ��W_TX_PAYLOAD��FLUSH_TX����رոù���
    FIFO_TX_FULL        = (1<<5),   ///< TX FIFO����־��1��TX FIFO����0��TX FIFO��д
    FIFO_TX_EMPTY       = (1<<4),   ///< TX FIFO�ձ�־��1��TX FIFOΪ�գ�0��TX FIFO������
    FIFO_RX_FULL        = (1<<1),   ///< RX FIFO����־��1��RX FIFO����0��RX FIFO��д
    FIFO_RX_EMPTY       = (1<<0),   ///< RX FIFO�ձ�־��1��RX FIFOΪ�գ�0��RX FIFO������
}fifo_type;

#ifdef _Si24R1  ///< {
    /**
     * @brief ʹ�ܶ�̬���س���
     */
        #define DYNPD       0x1C
    typedef enum
    {  
        DPL_P5              = (1<<5),   ///< ʹ�ܽ��չܵ�5��̬���س��ȣ���EN_DPL��ENAA_P5��
        DPL_P4              = (1<<4),   ///< ʹ�ܽ��չܵ�4��̬���س��ȣ���EN_DPL��ENAA_P4��
        DPL_P3              = (1<<3),   ///< ʹ�ܽ��չܵ�3��̬���س��ȣ���EN_DPL��ENAA_P3��
        DPL_P2              = (1<<2),   ///< ʹ�ܽ��չܵ�2��̬���س��ȣ���EN_DPL��ENAA_P2��
        DPL_P1              = (1<<1),   ///< ʹ�ܽ��չܵ�1��̬���س��ȣ���EN_DPL��ENAA_P1��
        DPL_P0              = (1<<0),   ///< ʹ�ܽ��չܵ�0��̬���س��ȣ���EN_DPL��ENAA_P0��
    }dynpd_type;

    /**
     * @brief �����Ĵ���
     */
        #define FEATURE     0x1D
        typedef enum
        {  
            EN_DPL          = (1<<6),   ///< ʹ�ܶ�̬���س���
            EN_ACK_PAYD     = (1<<5),   ///< ʹ��ACK���أ����������ݵ�ACK����
            EN_DYN_ACK      = (1<<4),   ///< ʹ������W_TX_PAYLOAD_NOACK
        }feature_type;
#endif  ///< }

//--------------------------------------NRF24L01 Error----------------------------------------------------//
/**
 * @brief ����ö�ٶ���
 */
typedef enum
{
    NRF_NONRF           = 0x00,     ///< δ��⵽ģ��ʱ��----ģ�鲻����ģʽ
    NRF_CHECKED         = 0x01,     ///< �ϵ���ģ��ͨ���������----������ģʽ
    NRF_SHUTDOWN        = 0x02,     ///< ���ͨ���������----�ض�ģʽ
    NRF_STANDBY         = 0x03,     ///< ��ʼ����ɺ��----����ģʽ
    NRF_IDLETX          = 0x04,     ///< �������ģʽ
    NRF_TX              = 0x05,     ///< ����ģʽ
    NRF_RX              = 0x06,     ///< ����ģʽ
}NRFMode;
#define IS_NRFMODE(MODE) (((MODE) == NRF_NONRF) || \
                          ((MODE) == NRF_CHECKED) || \
                          ((MODE) == NRF_SHUTDOWN) || \
                          ((MODE) == NRF_STANDBY) || \
                          ((MODE) == NRF_IDLETX) || \
                          ((MODE) == NRF_TX) || \
                          ((MODE) == NRF_RX))

typedef enum
{
    NOACK   = 0x00,
    ACK     = 0x01,
}AckMode;
#define IS_ACKMODE(MODE) ( ((MODE) == ACK) || ((MODE) == NOACK) )

typedef enum {STA_DISABLE = 0, STA_ENABLE = !STA_DISABLE} StartState;
#define IS_START_STATE(STATE) (((STATE) == STA_DISABLE) || ((STATE) == STA_ENABLE))
        
typedef enum
{
    //��׼������
    NRF24L01_OK                         = (0x00), /*!<  */
    NRF24L01_ERROR                      = (0x01), /*!<  */
	//��������� 
    NRF24L01_CHECK_FAIL                 = (0x02), /*!<  */
    NRF24L01_CHECK_NOTALLOW             = (0x03), /*!<  */
    NRF24L01_TX_MAX                     = (0x04), /*!<  */
    NRF24L01_RX_OK                      = (0x05), /*!<  */
    NRF24L01_WAIT_TX_DS                 = (0x06), /*!<  */
    NRF24L01_ADDR_WIDTH_ERROR           = (0x07), /*!<  */
}NRF24L01_Error;

//24L01 ��ȶ���
#define ADDR_WIDTH      5       ///< 5�ֽڵĵ�ַ���
#define PLOAD_WIDTH     32      ///< 32�ֽڵ��û����ݿ��


/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/
typedef void (*VoidFuncPtr)     (void);
//typedef void (*VoidParaFuncPtr) (void*);
/************************************************* 
*������:		UartRecvHandle
*������   ���ڽ������ݺ�Ĵ�����ָ�룬ÿ���ֽڵĴ�����
*       �漰���ĺ�����ͨ��uart_configע�ᵽ���ڽ����жϴ��������
*       �漰���ĺ�����ִ��ʱ�䲻��δ֪���Ҿ�������
*************************************************/
typedef void (*NrfRxHandle) (uint8_t* pRx_buf,uint8_t rx_len);

/** 
 * @brief   NRF24L01 ������е�Ԫ�Ľṹ�嶨��
 */
#define UNIT_BUF_MAX_SIZE     64
typedef struct __NRF_TX_QUEUE_UNIT
{
    AckMode ack_mode;                   ///< ��Ӧģʽ
    uint8_t addr[5];                    ///< Զ�˵�ַ
    uint8_t buf[UNIT_BUF_MAX_SIZE];     ///< ���仺����
    uint8_t iTx;                        ///< ������λ��
    uint8_t nLast;                      ///< ʣ������
}_NRF_TX_QUEUE_UNIT;

/** 
 * @brief   NRF24L01 init structure definition
 */
typedef struct __NRF24L01
{
    NRFMode     mode;                               ///< NRF24L01's Operating mode
    uint8_t     rx_addr[5];                         ///< Local Address
    
    /**
    * The set of chip enable callback func.
    */
    struct
    {
        void    (*_en)              (void);         ///< chip enable
        void    (*_dis)             (void);         ///< chip disable
    }CE;
    /**
    * The set of critical section callback func.
    */
    struct
    {
        void    (*_enter)           (void);         ///< critical section enter 
        void    (*_exit)            (void);         ///< critical section exit  
    }CRIS;
    /**
    * The set of chip select callback func.
    */
    struct
    {
        void    (*_select)          (void);         ///< chip select
        void    (*_deselect)        (void);         ///< chip deselect
    }CS;
    struct
    {
        void    (*_set_speed)       (void);
        uint8_t (*_read_byte)       (void);
        void    (*_write_byte)      (uint8_t wb);
    }SPI;
    struct
    {
        VoidFuncPtr _trigger;                       ///< trigger interrupt
    }IRQ;        
    
    /**
    * �뷢����صı���
    */
    Queue               tx_queue;                       ///< TX Queue
    _NRF_TX_QUEUE_UNIT  tx_unit;
    uint8_t             tx_addr[5];                     ///< Trans Address
    AckMode             ackmode;                        ///< Ack Mode
    StartState          sta_state;
    
    /**
    * �������صı���
    */
    uint8_t         rx_len;
    uint8_t         rx_buf[PLOAD_WIDTH];            ///< RX Buffer
    NrfRxHandle     _rx_handle;                     ///< �����жϺ���
}_NRF24L01;
extern _NRF24L01 NRF24L01;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
void reg_nrf_ce_cbfunc      ( VoidFuncPtr ce_en,    VoidFuncPtr ce_dis );
void reg_nrf_cris_cbfunc    ( VoidFuncPtr cris_en,  VoidFuncPtr cris_ex );
void reg_nrf_cs_cbfunc      ( VoidFuncPtr cs_sel,   VoidFuncPtr cs_desel );
void reg_nrf_spi_cbfunc     ( VoidFuncPtr spi_ss,   uint8_t (*spi_rb)(void), void (*spi_wb)(uint8_t wb) );
void reg_nrf_irq_cbfunc     ( VoidFuncPtr irq_trigger );
void reg_nrf_rx_handle_cbfunc( NrfRxHandle rx_handle );

/* ������� */
NRF24L01_Error NRF24L01_RX          ( __IN__ const AckMode ack_mode );
NRF24L01_Error NRF24L01_SET_RxAddr  ( __IN__ const uint8_t *pRXAddr,__IN__ const uint8_t addr_width );

/* ������� */
NRF24L01_Error NRF24L01_TX_Init     ( __OUT_ _NRF_TX_QUEUE_UNIT* pTXUnitArray,__IN__ const uint16_t array_size );
NRF24L01_Error NRF24L01_TX_Buffer   ( __IN__ const AckMode ack_mode,__IN__ const uint8_t* pTXAddr,__OUT_ uint8_t* pTXBuf,__OUT_ uint8_t tx_len );



NRF24L01_Error NRF24L01_IRQ_oper(void);

#endif
