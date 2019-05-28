#include "bsp_uart.h"

#if SYSTEM_SUPPORT_OS
    #include "includes.h"   //支持OS时，使用	  
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

    NVIC_InitStructure.NVIC_IRQChannel = u8NVIC_IRQChannel ;    //设置串口中断
    NVIC_Init(&NVIC_InitStructure);
}

//串口初始化通用函数
void USART_Config(USART_TypeDef* USARTx,u32 baud)
{
    USART_InitTypeDef USART_InitStructure;
    
    USART_InitStructure.USART_BaudRate              = baud;                             //速率 baud bps
    USART_InitStructure.USART_WordLength            = USART_WordLength_8b;              //数据位8位
    USART_InitStructure.USART_StopBits              = USART_StopBits_1;                 //停止位1位
    USART_InitStructure.USART_Parity                = USART_Parity_No;                  //无校验位
    USART_InitStructure.USART_HardwareFlowControl   = USART_HardwareFlowControl_None;   //无硬件流控
    USART_InitStructure.USART_Mode                  = USART_Mode_Rx | USART_Mode_Tx;    //收发模式
    
    /* Configure USART */
    USART_Init(USARTx, &USART_InitStructure);           //配置串口参数函数
    
    /* Enable USART1 Receive and Transmit interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);      //使能接收中断
	//USART_ITConfig( USARTx , USART_IT_TXE, ENABLE);   //发送中断使能
    
    /* Enable the USART */
    USART_Cmd(USARTx, ENABLE);
}

//串口1 使用默认IO的初始化函数
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    //usart_init----------------------------------------------------
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;    //浮空输入模式	   
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 

    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//串口2 使用复用IO的初始化函数
//void USART2_Config(void)
//{
//    GPIO_InitTypeDef GPIO_InitStructure;

//    //PD5,PD6 复用IO口功能用于配置串口。因此要使能AFIO（复用功能IO）时钟。
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);  //因为串口1已经使能了
//    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    //因为串口1已经使能了
//    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	
//    GPIO_PinRemapConfig(GPIO_Remap_USART2,ENABLE); 

//    //usart_init----------------------------------------------------
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //浮空输入模式  
//    GPIO_Init(GPIOD, &GPIO_InitStructure);					 

//    /* Configure USART2 Tx (PD.2) as alternate function push-pull */
//    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_5;
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    //复用推挽输出
//    GPIO_Init(GPIOD, &GPIO_InitStructure);
//}

//串口2 使用默认IO的初始化函数
void USART2_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

    //usart_init----------------------------------------------------
    /* Configure USART2 Rx (PA.3) as input floating */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;      //浮空输入模式	   
    GPIO_Init(GPIOA, &GPIO_InitStructure);					 

    /* Configure USART2 Tx (PA.2) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;    //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

//串口3 使用默认IO的初始化函数
void USART3_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART3 Rx (PB.11) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //浮空输入模式	   
    GPIO_Init(GPIOB, &GPIO_InitStructure);					 

    /* Configure USART3 Tx (PB.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

//串口4 使用默认IO的初始化函数
void USART4_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART4 Rx (PC.11) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //浮空输入模式	   
    GPIO_Init(GPIOC, &GPIO_InitStructure);					 

    /* Configure USART4 Tx (PC.10) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

//串口5 使用默认IO的初始化函数
void USART5_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //时钟配置
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	

    //usart_init----------------------------------------------------
    /* Configure USART5 Rx (PD.2) as input floating */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_IPU;    //浮空输入模式	   
    GPIO_Init(GPIOD, &GPIO_InitStructure);					 

    /* Configure USART5 Tx (PC.12) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin     = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed   = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode    = GPIO_Mode_AF_PP;  //复用推挽输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/************************************************* 
*Function:		Uart_Config
*Input:			
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  配置串口参数，主要是波特率；注册串口接收字节的回调函数
*************************************************/
void Uart_Config(
    VoidFuncPtr     uart_io_init_fun,   //串口IO口初始化函数,可以使用 USART?_Config 函数
    UartDriverCtrl* uart,               //UartDriverCtrl* uart ，使用 DriverUart?
    UartCommType    commtype,           //串口通信方式，TTL、485、422
    u32             baud,               //u32 baud  串口被配置的波特率
    u8*             pSendBuf,           //发送缓冲区 地址 发送缓冲区由外部提供 必须提供，否则会调用出先问题
    u32             nSendBufLen,        //发送缓冲区 的 大小
    UartRecvHandle  recv_handle         //UartRecvHandle recv_handle  串口接收一个字节的中断中对接收数据的处理函数
    )
{
    (*uart_io_init_fun)();              //the uart IO init call

    if ( (nSendBufLen > (4*1024)) || (0==pSendBuf) || (0==nSendBufLen) ){   //错误的缓冲区大小 和 缓冲区
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
    
    uart_enable_int_inNcv( uart->u8_NVIC_IRQChannel );  //中断使能过程
	USART_Config( uart->pRegs , baud );
}

/************************************************* 
*Function:		uart_set_send_finish_handle
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               VoidParaFuncPtr send_ok_handle 发送完成的回调函数，这里发送完成，指将最后一个字节写入发送缓冲区中
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  配置串口发送完成的回调函数，这里的回调函数只会执行一次，机所有的数据发送完毕 然后会自动清除
*           若有需要，函数执行过程再次自动设置
*************************************************/
void uart_set_send_finish_handle(UartDriverCtrl* uart,VoidParaFuncPtr send_ok_handle)
{
    uart->send_finish_cb = send_ok_handle;
}

/************************************************* 
*Function:		uart_get_need_send_cnt
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  获取当前发送缓冲区中 但是还没有发送出去的数据量
*************************************************/
u32 uart_get_need_send_cnt(UartDriverCtrl* uart)
{
    if ( uart->i_yet_send <= uart->i_to_send )
    {
        return ( uart->i_to_send - uart->i_yet_send );
    }
    return ( uart->tx_buf_len + uart->i_to_send - uart->i_yet_send );
}

//获取可用的缓冲区大小
u32 uart_get_send_buf_useful_cnt(UartDriverCtrl* uart)
{
    return ( uart->tx_buf_len - uart_get_need_send_cnt(uart));
}

static __inline u32 uart_do_send_byte(UartDriverCtrl* pUart,u8 tosend)
{
    u16 i_new_add_pos = pUart->i_to_send + 1;
    //如果位置超出缓冲区的总大小 编号切换为0
    if (i_new_add_pos >= pUart->tx_buf_len )
    {
        i_new_add_pos = 0;
    }

    //下面的判断成立，说明环形缓冲区满了，放不了数据
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
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               u8 tosend     需要发送的数据
*OUTPUT:		void
*Return:		u32          实际发送的数据长度
*DESCRIPTION:  发送一个字节数据
*************************************************/
u32 uart_send_byte(UartDriverCtrl* uart,u8 tosend)    
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;
    //如果当前串口未使用 直接返回0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    rt = uart_do_send_byte(uart,tosend);
//    OS_EXIT_CRITICAL();

    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //发送中断使能
    
    return rt;  
}

/************************************************* 
*Function:		uart_send_bytes
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               const u8* sdata     需要发送的数据的开始地址
*               u32 send_len  需要发送的数据长度
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  发送固定长度的串口数据
*************************************************/
u32 uart_send_bytes(UartDriverCtrl* uart,const u8* sdata,u32 send_len)
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;
    //如果当前串口未使用 直接返回0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    while( send_len && (uart_do_send_byte(uart,*(sdata++))))
    {
        rt++;  //返回值增加1
        send_len--; //已经发送的数据 减去1
    }
//    OS_EXIT_CRITICAL();
	
    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //发送中断使能
		
    return rt;    
}


/************************************************* 
*Function:		uart_send_string
*Input:			u32  uart_id  串口编号 UART_PORT_1~5
*               const char* str   需要发送的字符串数据
*OUTPUT:		void
*Return:		void
*DESCRIPTION:  通过串口发送一个字符串
*************************************************/
u32 uart_send_string(UartDriverCtrl* uart,const char* str)
{
    u32 rt=0;
//    OS_CPU_SR  cpu_sr = 0;

    //如果当前串口未使用 直接返回0
    if ( 0 == uart->tx_buf_len )
    {
        return 0;
    }

//    OS_ENTER_CRITICAL();
    while( (*str) ) //如果字符串存在
    {
        if ( uart_do_send_byte( uart ,*str++) )
        {
            rt++;  //返回值增加1
        }
        else
        {
            break;
        }
    }
//    OS_EXIT_CRITICAL();

    USART_ITConfig( uart->pRegs , USART_IT_TXE, ENABLE);  //发送中断使能

    return rt;
}

//清空需要发送的数据
void uart_clear_sending_data(UartDriverCtrl* uart)
{
//    OS_CPU_SR  cpu_sr = 0;
    //如果当前串口未使用 直接返回0
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
//    while(*Data!=0){				                          //判断是否到达字符串结束符
//        if(*Data=='%'){									  //
//            switch (*++Data){				
//            case 's':										  //字符串
//                s = va_arg(ap, const char *);
//                uart_send_string(uart,s);
//                break;
//            case 'd':										  //十进制
//                d = va_arg(ap, int);
//                int_to_str(d, buf);
//                uart_send_string(uart,buf);
//                break;
//            case 'c':										  //单独输出一个字符
//                //c = va_arg(ap,char);
//                //因为我们无法传递一个char类型参数，如果传递了，它将会被自动转化为int类型。上面的式子应该写成：
//                //    c = va_arg(ap,int);
//                buf[0] = (char)( va_arg(ap, int) );
//                uart_do_send_byte(uart, buf[0] ) ;
//                break;
//            case 'X':	
//            case 'x':										  //输出一个 十六进制数据
//                buf[0] = (char)( va_arg(ap, int) );
//                uart_do_send_byte(uart , box_str_ChangeValToChar( (buf[0]>>4)&0X0F ) ); 
//                uart_do_send_byte(uart , box_str_ChangeValToChar(buf[0]&0X0F) ); 
//                break;
//            case 'H':	
//                d = va_arg(ap, int);  //十六进制 输出一个int型数据
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
//    va_end(ap);  /* 释放ap—— 必须！ 见相关链接*/
}

/*******************************************************************************
* Function Name  : uart_send_int_oper
* Description    : 接收到中断后，处理发送中断相关过程
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
static __inline void uart_send_int_oper(UartDriverCtrl* uart)
{
    if ( uart->i_to_send == uart->i_yet_send)
    {
        //发送数据为空，不需要发送数据
        USART_ITConfig( uart->pRegs , USART_IT_TXE, DISABLE);
		//USART_ClearFlag(USART1,USART_IT_TXE);
    }
    else
    {
        if( uart->CommType == UART_485_EN ){
            while(USART_GetFlagStatus(uart->pRegs, USART_FLAG_TC) == RESET);
            RS485_TX_EN = 1;
        }
        
        //需要发送数据 下面为发送一个字节
        USART_SendData( uart->pRegs , *(uart->p_tx_buf+uart->i_yet_send) );
        
        //已经发送的数据位置的更新
        uart->i_yet_send++;
        
        //环形数组 的相关处理
        if ( uart->i_yet_send >= uart->tx_buf_len )
        {
            uart->i_yet_send = 0;
        }

        //是否发送完毕的判断
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

//串口中断处理函数
static __inline void uart_driver_IRQ_oper(USART_TypeDef* USARTx,UartDriverCtrl* uart)
{
    //接收中断处理
    //if(USART_GetITStatus(USARTx, USART_IT_RXNE) != RESET)	   //判断读寄存器是否非空
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

    //发送中断处理
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
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART1 , UART_PORT_1 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
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
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART2 , UART_PORT_2 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
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
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( USART3 , UART_PORT_3 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
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
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( UART4 , UART_PORT_4 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
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
    OS_ENTER_CRITICAL();                //进入临界区(关闭中断)
#else
    INTX_DISABLE();
#endif
    
    uart_driver_IRQ_oper( UART5 , UART_PORT_5 );
    
#if SYSTEM_SUPPORT_OS
    OS_EXIT_CRITICAL();                 //退出临界区(开中断)
#else
    INTX_ENABLE();
#endif
}

