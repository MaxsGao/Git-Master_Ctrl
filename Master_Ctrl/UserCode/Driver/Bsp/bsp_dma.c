#include "bsp_dma.h"

#define RECV_BUF_SIZE 8200
u8 RecvBuff[RECV_BUF_SIZE];

void SPI1_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //使能DMA传输

    DMA_DeInit(DMA1_Channel2);                                                  //将DMA的通道x寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI1->DR;               //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA内存基地址
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;        //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //数据宽度为8位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //工作在正常模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMA通道 x拥有中优先级
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                //根据DMA_InitStruct中指定的参数初始化DMA的通道x所标识的寄存器
    
    DMA_DeInit(DMA1_Channel3);                                                  //将DMA的通道x寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI1->DR;               //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA内存基地址
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;        //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //数据宽度为8位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //工作在正常模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                //根据DMA_InitStruct中指定的参数初始化DMA的通道x所标识的寄存器
}
void SPI2_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //使能DMA传输

    DMA_DeInit(DMA1_Channel4);                                                  //将DMA的通道x寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI2->DR;               //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA内存基地址
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;        //数据传输方向，从外设读取发送到内存
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //数据宽度为8位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //工作在正常模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMA通道 x拥有中优先级
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);                                //根据DMA_InitStruct中指定的参数初始化DMA的通道x所标识的寄存器
    
    DMA_DeInit(DMA1_Channel5);                                                  //将DMA的通道x寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI2->DR;               //DMA外设基地址
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA内存基地址
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;        //数据传输方向，从内存读取发送到外设
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //数据宽度为8位
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //数据宽度为8位
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //工作在正常模式
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMA通道 x拥有中优先级 
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);                                //根据DMA_InitStruct中指定的参数初始化DMA的通道x所标识的寄存器
}

/**
  *	@breif  The spi dma trans function.
  * @param  rx_buf -- the point of rx buffer
  * @param  tx_buf -- the point of tx buffer
  * @elngth length -- the size of data.
  * @retval None
  */
void SPI1_Trans_Write(uint8_t *tx_buf,     //发送buffer指针
					uint16_t length)     //传输数据长度
{
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	/* 设置传输字节数 */
	DMA_SetCurrDataCounter(DMA1_Channel2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel3, (uint16_t)length);
    
	/* 发送接收数据存储地址自增 */
	DMA1_Channel2->CCR |= (1 << 7);
	DMA1_Channel3->CCR |= (1 << 7);
	
	/* 设置接收和发送的内存地址 */
	DMA1_Channel2->CMAR = (uint32_t)RecvBuff;
	DMA1_Channel3->CMAR = (uint32_t)tx_buf;

	/* 读取一次DR，使其清空 */
	SPI1->DR;
	
	/* 等待发送区为空 */
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
	
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* 打开DMA通道 */
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* 传输完成 */
    while( DMA_GetITStatus(DMA1_FLAG_TC2) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC3) == RESET );
	
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
    
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	
	/* 清除DMA传输完成标志 */
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_ClearFlag(DMA1_FLAG_TC3);
}
/**
  *	@breif  The spi dma trans function.
  * @param  rx_buf -- the point of rx buffer
  * @param  tx_buf -- the point of tx buffer
  * @elngth length -- the size of data.
  * @retval None
  */
void SPI2_Trans_Write(uint8_t *tx_buf,     //发送buffer指针
					uint16_t length)     //传输数据长度
{
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	
	/* 设置传输字节数 */
	DMA_SetCurrDataCounter(DMA1_Channel4, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel5, (uint16_t)length);
    
	/* 发送接收数据存储地址自增 */
	DMA1_Channel4->CCR |= (1 << 7);
	DMA1_Channel5->CCR |= (1 << 7);
	
	/* 设置接收和发送的内存地址 */
	DMA1_Channel4->CMAR = (uint32_t)RecvBuff;
	DMA1_Channel5->CMAR = (uint32_t)tx_buf;

	/* 读取一次DR，使其清空 */
	SPI2->DR;
	
	/* 等待发送区为空 */
	while( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET );
	
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* 打开DMA通道 */
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	/* 传输完成 */
    while( DMA_GetITStatus(DMA1_FLAG_TC4) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC5) == RESET );
	
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE);
    
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);	
	
	/* 清除DMA传输完成标志 */
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}

/**
  *	@breif  The spi dma trans function.
  * @param  rx_buf -- the point of rx buffer
  * @elngth length -- the size of data.
  * @retval None
  */  
void SPI1_Trans_Read(uint8_t *rx_buf,
					uint16_t length)
{
    uint8_t tx_data=0xFF;
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	/* 设置传输字节数 */
	DMA_SetCurrDataCounter(DMA1_Channel2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel3, (uint16_t)length);
	
	/* 发送DMA流的发送地址不自增 */
    DMA1_Channel3->CCR &= ~(1 << 7);
	
	/* 设置接收和发送的内存地址 */
    DMA1_Channel2->CMAR = (uint32_t)rx_buf;
    DMA1_Channel3->CMAR = (uint32_t)&tx_data;

	/* 读取一次DR，使其清空 */
	SPI1->DR;
	
	/* 等待发送区为空 */
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
	
    /* 开启SPI_DMA传输通道 */
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* 打开DMA通道 */
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* 传输完成 */
    while( DMA_GetITStatus(DMA1_FLAG_TC2) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC3) == RESET );
	
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	
    /* 开启SPI_DMA传输通道 */
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
	
	/* 清除DMA传输完成标志 */
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_ClearFlag(DMA1_FLAG_TC3);
}

/**
  *	@breif  The spi dma trans function.不改变SPI的通信方式
  * @param  rx_buf -- the point of rx buffer
  * @param  tx_data -- the spi tx data.
  * @elngth length -- the size of data.
  * @retval None
  */
void SPI2_Trans_Read(uint8_t *rx_buf,
					uint16_t length)
{
    uint8_t tx_data=0xFF;
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	
	/* 设置传输字节数 */
	DMA_SetCurrDataCounter(DMA1_Channel4, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel5, (uint16_t)length);
	
	/* 发送DMA流的发送地址不自增 */
    DMA1_Channel5->CCR &= ~(1 << 7);
	
	/* 设置接收和发送的内存地址 */
    DMA1_Channel4->CMAR = (uint32_t)rx_buf;
    DMA1_Channel5->CMAR = (uint32_t)&tx_data;

	/* 读取一次DR，使其清空 */
	SPI2->DR;
	
	/* 等待发送区为空 */
	while( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET );
	
    /* 开启SPI_DMA传输通道 */
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* 打开DMA通道 */
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	/* 传输完成 */
    while( DMA_GetITStatus(DMA1_FLAG_TC4) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC5) == RESET );
	
	/* 关闭DMA通道 */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);	
	
    /* 开启SPI_DMA传输通道 */
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE);
	
	/* 清除DMA传输完成标志 */
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}



