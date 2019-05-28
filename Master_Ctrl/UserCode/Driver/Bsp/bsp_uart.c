#include "bsp_uart.h"

#if SYSTEM_SUPPORT_OS
    #include "includes.h"   //֧��OSʱ��ʹ��	  
#endif

UartDriverCtrl DriverUart1 = {USART1,USART1_IRQn ,0,UART_TTL_EN,0,0,0,0,0,};
UartDriverCtrl DriverUart2 = {USART2,USART2_IRQn ,0,UART_TTL_EN,0,0,0,0,0,};
UartDriverCtrl DriverUart3 = {USART3,USART3_IRQn ,0,UART_TTL_EN,0,0,0,0,0,};
UartDriverCtrl DriverUart4 = {UART4 ,USART2_IRQn  ,0,UART_TTL_EN,0,0,0,0,0,};//UART4_IRQn
UartDriverCtrl DriverUart5 = {UART5 ,USART3_IRQn  ,0,UART_TTL_EN,0,0,0,0,0,};//UART5_IRQn

void uart_enable_int_inNcv(u8 u8NVIC_IRQChannel)
{
    NVIC_InitTypeDef NVIC_InitStructure;  

    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd         = ENABLE;

    NVIC_InitStructure.NVIC_IRQChannel = u8NVIC_IRQChannel ;    //���ô����ж�
    NVIC_Init(&NVIC_InitStructure);
}

//���ڳ�ʼ��ͨ�ú���
void USART_Config(USART_TypeDef* USARTx,u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate              = baud;                             //���� baud bps
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;              //����λ8λ
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;                 //ֹͣλ1λ
    USART_InitStructure.USART_Parity                = USART_Parity_No;                  //��У��λ
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //��Ӳ������
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;    //�շ�ģʽ
    
    /* Configure USART */
    USART_Init(USARTx, &USART_InitStructure);           //���ô��ڲ�������
    
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);      //ʹ�ܽ����ж�
	//USART_ITConfig( USARTx , USART_IT_TXE, ENABLE);   //�����ж�ʹ��
    
    /* Enable the USART */
    USART_Cmd(USARTx, ENABLE);
}

//����1 ʹ��Ĭ��IO�ĳ�ʼ������
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʱ������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //usart_init----------------------------------------------------
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;    //��������ģʽ	   
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 

    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//����2 ʹ�ø���IO�ĳ�ʼ������
//void USART2_Config(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;

//    //PD5,PD6 ����IO�ڹ����������ô��ڡ����Ҫʹ��AFIO�����ù���IO��ʱ�ӡ�
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);  //��Ϊ����1�Ѿ�ʹ����
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //��Ϊ����1�Ѿ�ʹ����
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); 

//    //usart_init----------------------------------------------------
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //��������ģʽ  
//    GPIO_Init(GPIOD, &GPIO_InitStructure);					 

//    /* Configure USART2 Tx (PD.2) as alternate function push-pull */
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    //�����������
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//}

//����2 ʹ��Ĭ��IO�ĳ�ʼ������
void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʱ������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //usart_init----------------------------------------------------
    /* Configure USART2 Rx (PA.3) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //��������ģʽ	   
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 

    /* Configure USART2 Tx (PA.2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//����3 ʹ��Ĭ��IO�ĳ�ʼ������
void USART3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʱ������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART3 Rx (PB.11) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //��������ģʽ	   
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 

    /* Configure USART3 Tx (PB.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //�����������
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//����4 ʹ��Ĭ��IO�ĳ�ʼ������
void USART4_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʱ������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART4 Rx (PC.11) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //��������ģʽ	   
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 

    /* Configure USART4 Tx (PC.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//����5 ʹ��Ĭ��IO�ĳ�ʼ������
void USART5_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //ʱ������
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART5 Rx (PD.2) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //��������ģʽ	   
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 

    /* Configure USART5 Tx (PC.12) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //�����������
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/************************************************* 
*Function:		Uart_Config
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ���ô��ڲ�������Ҫ�ǲ����ʣ�ע�ᴮ�ڽ����ֽڵĻص�����
*************************************************/
void Uart_Config(
    VoidFuncPtr     uart_io_init_fun,   //����IO�ڳ�ʼ������,����ʹ�� USART?_Config ����
    UartDriverCtrl* uart,               //UartDriverCtrl* uart ��ʹ�� DriverUart?
    UartCommType    commtype,           //����ͨ�ŷ�ʽ��TTL��485��422
    u32             baud,               //u32 baud  ���ڱ����õĲ�����
    u8*             pSendBuf,           //���ͻ����� ��ַ ���ͻ��������ⲿ�ṩ �����ṩ���������ó�������
    u32             nSendBufLen,        //���ͻ����� �� ��С
    UartRecvHandle  recv_handle         //UartRecvHandle recv_handle  ���ڽ���һ���ֽڵ��ж��жԽ������ݵĴ�����
    )
{
    (*uart_io_init_fun)();              //the uart IO init call

    if ( (nSendBufLen > (4*1024)) || (0==pSendBuf) || (0==nSendBufLen) ){   //����Ļ�������С �� ������
        nSendBufLen      = 0;
        pSendBuf         = 0;
    }
    uart->CommType       = commtype;
    
    uart->p_tx_buf       = pSendBuf;
    uart->tx_buf_len     = nSendBufLen;
    uart->i_to_send      = 0;
    uart->i_yet_send     = 0;
    uart->send_finish_cb = 0;
    uart->recv_handle    = recv_handle;
    
    uart_enable_int_inNcv( uart->u8_NVIC_IRQChannel );  //�ж�ʹ�ܹ���
	USART_Config( uart->pRegs , baud );
}

/************************************************* 
*Function:		uart_set_send_finish_handle
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               VoidParaFuncPtr send_ok_handle ������ɵĻص����������﷢����ɣ�ָ�����һ���ֽ�д�뷢�ͻ�������
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ���ô��ڷ�����ɵĻص�����������Ļص�����ֻ��ִ��һ�Σ������е����ݷ������ Ȼ����Զ����
*           ������Ҫ������ִ�й����ٴ��Զ�����
*************************************************/
void uart_set_send_finish_handle(UartDriverCtrl* uart,VoidParaFuncPtr send_ok_handle)
{
    uart->send_finish_cb = send_ok_handle;
}

/************************************************* 
*Function:		uart_get_need_send_cnt
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ��ȡ��ǰ���ͻ������� ���ǻ�û�з��ͳ�ȥ��������
*************************************************/
u32 uart_get_need_send_cnt(UartDriverCtrl* uart)
{
    if ( uart->i_yet_send <= uart->i_to_send )
    {
        return ( uart->i_to_send - uart->i_yet_send );
    }
    return ( uart->tx_buf_len + uart->i_to_send - uart->i_yet_send );
}

//��ȡ���õĻ�������С
u32 uart_get_send_buf_useful_cnt(UartDriverCtrl* uart)
{
    return ( uart->tx_buf_len - uart_get_need_send_cnt(uart));
}

static __inline u32 uart_do_send_byte(UartDriverCtrl* pUart,u8 tosend)
{
    u16 i_new_add_pos = pUart->i_to_send + 1;
    //���λ�ó������������ܴ�С ����л�Ϊ0
    if (i_new_add_pos >= pUart->tx_buf_len )
    {
        i_new_add_pos = 0;
    }

    //������жϳ�����˵�����λ��������ˣ��Ų�������
    if ( i_new_add_pos == pUart->i_yet_send )
    {
        return 0;
    }
    *((pUart->p_tx_buf)+(pUart->i_to_send)) = tosend;
	pUart->i_to_send = i_new_add_pos;
    
    return 1;
}

/************************************************* 
*Function:		uart_send_byte
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               u8 tosend     ��Ҫ���͵�����
*OUTPUT:		void
*Return:		u32          ʵ�ʷ��͵����ݳ���
*DESCRIPTION:  ����һ���ֽ�����
*************************************************/
u32 uart_send_byte(UartDriverCtrl* uart,u8 tosend)    
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;
    //�����ǰ����δʹ�� ֱ�ӷ���0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    rt = uart_do_send_byte(uart,tosend);
//    OS_EXIT_CRITICAL();

    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //�����ж�ʹ��
    
    return rt;  
}

/************************************************* 
*Function:		uart_send_bytes
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               const u8* sdata     ��Ҫ���͵����ݵĿ�ʼ��ַ
*               u32 send_len  ��Ҫ���͵����ݳ���
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ���͹̶����ȵĴ�������
*************************************************/
u32 uart_send_bytes(UartDriverCtrl* uart,const u8* sdata,u32 send_len)
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;
    //�����ǰ����δʹ�� ֱ�ӷ���0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    while( send_len && (uart_do_send_byte(uart,*(sdata++))))
    {
        rt++;  //����ֵ����1
        send_len--; //�Ѿ����͵����� ��ȥ1
    }
//    OS_EXIT_CRITICAL();
	
    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //�����ж�ʹ��
		
    return rt;    
}


/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  ���ڱ�� UART_PORT_1~5
*               const char* str   ��Ҫ���͵��ַ�������
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  ͨ�����ڷ���һ���ַ���
*************************************************/
u32 uart_send_string(UartDriverCtrl* uart,const char* str)
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;

    //�����ǰ����δʹ�� ֱ�ӷ���0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    while( (*str) ) //����ַ�������
    {
        if ( uart_do_send_byte( uart ,*str++) )
        {
            rt++;  //����ֵ����1
        }
        else
        {
            break;
        }
    }
//    OS_EXIT_CRITICAL();

    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //�����ж�ʹ��

    return rt;
}

//�����Ҫ���͵�����
void uart_clear_sending_data(UartDriverCtrl* uart)
{
//    OS_CPU_SR  cpu_sr = 0;
    //�����ǰ����δʹ�� ֱ�ӷ���0
    if ( 0 == uart->tx_buf_len )
    {
        return;
    }
//    OS_ENTER_CRITICAL();
    uart->i_to_send  = 0;
    uart->i_yet_send = 0;
//    OS_EXIT_CRITICAL();
}

//extern char *int_to_str(signed long value, char *string);

//static __inline char box_str_ChangeValToChar(char val)
//{
//    return ( ( val < 10 ) ? (val+'0') : (val+'A'-10) ) ;
//}

void uart_send_format(UartDriverCtrl* uart,const char *Data,...)
{ 
//    va_list ap;
////    OS_CPU_SR  cpu_sr = 0;
//    const char *s;
//    int d;
//    char buf[13];
////    va_start(ap, Data);

////    OS_ENTER_CRITICAL();
//    while(*Data!=0){				                          //�ж��Ƿ񵽴��ַ���������
//        if(*Data=='%'){									  //
//            switch (*++Data){				
//            case 's':										  //�ַ���
//                s = va_arg(ap, const char *);
//                uart_send_string(uart,s);
//                break;
//            case 'd':										  //ʮ����
//                d = va_arg(ap, int);
//                int_to_str(d, buf);
//                uart_send_string(uart,buf);
//                break;
//            case 'c':										  //�������һ���ַ�
//                //c = va_arg(ap,char);
//                //��Ϊ�����޷�����һ��char���Ͳ�������������ˣ������ᱻ�Զ�ת��Ϊint���͡������ʽ��Ӧ��д�ɣ�
//                //    c = va_arg(ap,int);
//                buf[0] = (char)( va_arg(ap, int) );
//                uart_do_send_byte(uart, buf[0] ) ;
//                break;
//            case 'X':	
//            case 'x':										  //���һ�� ʮ����������
//                buf[0] = (char)( va_arg(ap, int) );
//                uart_do_send_byte(uart , box_str_ChangeValToChar( (buf[0]>>4)&0X0F ) ); 
//                uart_do_send_byte(uart , box_str_ChangeValToChar(buf[0]&0X0F) ); 
//                break;
//            case 'H':	
//                d = va_arg(ap, int);  //ʮ������ ���һ��int������
//                for( buf[0]=0 ; buf[0]<8 ; buf[0]++ )
//                {
//                    uart_do_send_byte(uart , box_str_ChangeValToChar( (d>>28)&0X0F ) ); 
//                    d <<= 4;
//                }
//                break;
//            default:
//                uart_do_send_byte(uart , *Data );
//                break;
//            }
//            if ( *Data == 0 )
//            {
//                break;
//            }
//            Data++;
//        }
//        else {
//            uart_do_send_byte(uart,*Data++);
//        }
//    }
//    OS_EXIT_CRITICAL();
//    va_end(ap);  /* �ͷ�ap���� ���룡 ���������*/
}

/*******************************************************************************
* Function Name  : uart_send_int_oper
* Description    : ���յ��жϺ󣬴������ж���ع���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static __inline void uart_send_int_oper(UartDriverCtrl* uart)
{
    if ( uart->i_to_send == uart->i_yet_send)
    {
        //��������Ϊ�գ�����Ҫ��������
        USART_ITConfig( uart->pRegs , USART_IT_TXE, DISABLE);
		//USART_ClearFlag(USART1,USART_IT_TXE);
    }
    else
    {
        if( uart->CommType == UART_485_EN ){
            while(USART_GetFlagStatus(uart->pRegs, USART_FLAG_TC) == RESET);
            RS485_TX_EN = 1;
        }
        
        //��Ҫ�������� ����Ϊ����һ���ֽ�
        USART_SendData( uart->pRegs , *(uart->p_tx_buf+uart->i_yet_send) );
        
        //�Ѿ����͵�����λ�õĸ���
        uart->i_yet_send++;
        
        //�������� ����ش���
        if ( uart->i_yet_send >= uart->tx_buf_len )
        {
            uart->i_yet_send = 0;
        }

        //�Ƿ�����ϵ��ж�
        if ( uart->i_to_send == uart->i_yet_send )
        {
            if( uart->CommType == UART_485_EN ){
                while(USART_GetFlagStatus(uart->pRegs, USART_FLAG_TC) == RESET);
                RS485_TX_EN = 0;
            }
            USART_ITConfig( uart->pRegs , USART_IT_TXE, DISABLE);
            
            
            if ( uart->send_finish_cb )
            {
                (*(uart->send_finish_cb))((void*)(uart));
            }
        }
    }
}

//�����жϴ�����
static __inline void uart_driver_IRQ_oper(USART_TypeDef* USARTx,UartDriverCtrl* uart)
{
    //�����жϴ���
    //if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)	   //�ж϶��Ĵ����Ƿ�ǿ�
    if( USARTx->SR & USART_FLAG_RXNE)
    {
        if ( uart->recv_handle )
        {
            (*(uart->recv_handle))( USART_ReceiveData(USARTx) );
            //	 USART_ClearFlag(USART1,USART_IT_RXNE);
        }
        else
        {
            USART_ReceiveData(USARTx);
        }
    }

    //�����жϴ���
    //if( (USARTx->SR & USART_FLAG_TXE) )
    if( USART_GetITStatus(USARTx,USART_IT_TXE) != RESET )
    {
        uart_send_int_oper( uart );
    }
}
	
/*******************************************************************************
* Function Name  : USART1_IRQHandler
* Description    : This function handles USART1 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART1 , UART_PORT_1 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
    
}

/*******************************************************************************
* Function Name  : USART2_IRQHandler
* Description    : This function handles USART2 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART2_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART2 , UART_PORT_2 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}

/*******************************************************************************
* Function Name  : USART3_IRQHandler
* Description    : This function handles USART3 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART3_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART3 , UART_PORT_3 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}


/*******************************************************************************
* Function Name  : UART4_IRQHandler
* Description    : This function handles UART4 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART4_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( UART4 , UART_PORT_4 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}

/*******************************************************************************
* Function Name  : UART5_IRQHandler
* Description    : This function handles UART5 global interrupt request.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UART5_IRQHandler(void)
{
#if SYSTEM_SUPPORT_OS
	OS_CPU_SR cpu_sr=0;
    OS_ENTER_CRITICAL();                //�����ٽ���(�ر��ж�)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( UART5 , UART_PORT_5 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //�˳��ٽ���(���ж�)
#else
    INTX_ENABLE();
#endif
}

