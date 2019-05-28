#include "bsp_dma.h"

#define RECV_BUF_SIZE 8200
u8 RecvBuff[RECV_BUF_SIZE];

void SPI1_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //ʹ��DMA����

    DMA_DeInit(DMA1_Channel2);                                                  //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI1->DR;               //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;        //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //����������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMAͨ�� xӵ�������ȼ�
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel2, &DMA_InitStructure);                                //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x����ʶ�ļĴ���
    
    DMA_DeInit(DMA1_Channel3);                                                  //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI1->DR;               //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;        //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //����������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMAͨ�� xӵ�������ȼ� 
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel3, &DMA_InitStructure);                                //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x����ʶ�ļĴ���
}
void SPI2_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                          //ʹ��DMA����

    DMA_DeInit(DMA1_Channel4);                                                  //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI2->DR;               //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralSRC;        //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //����������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMAͨ�� xӵ�������ȼ�
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel4, &DMA_InitStructure);                                //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x����ʶ�ļĴ���
    
    DMA_DeInit(DMA1_Channel5);                                                  //��DMA��ͨ��x�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr    = (u32)&SPI2->DR;               //DMA�������ַ
    DMA_InitStructure.DMA_MemoryBaseAddr        = 0;                            //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR                   = DMA_DIR_PeripheralDST;        //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
    DMA_InitStructure.DMA_BufferSize            = 0;                            //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc         = DMA_PeripheralInc_Disable;    //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc             = DMA_MemoryInc_Enable;         //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize    = DMA_PeripheralDataSize_Byte;  //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_MemoryDataSize        = DMA_MemoryDataSize_Byte;      //���ݿ��Ϊ8λ
    DMA_InitStructure.DMA_Mode                  = DMA_Mode_Normal;              //����������ģʽ
    DMA_InitStructure.DMA_Priority              = DMA_Priority_Medium;          //DMAͨ�� xӵ�������ȼ� 
    DMA_InitStructure.DMA_M2M                   = DMA_M2M_Disable;              //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel5, &DMA_InitStructure);                                //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��x����ʶ�ļĴ���
}

/**
  *	@breif  The spi dma trans function.
  * @param  rx_buf -- the point of rx buffer
  * @param  tx_buf -- the point of tx buffer
  * @elngth length -- the size of data.
  * @retval None
  */
void SPI1_Trans_Write(uint8_t *tx_buf,     //����bufferָ��
					uint16_t length)     //�������ݳ���
{
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	/* ���ô����ֽ��� */
	DMA_SetCurrDataCounter(DMA1_Channel2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel3, (uint16_t)length);
    
	/* ���ͽ������ݴ洢��ַ���� */
	DMA1_Channel2->CCR |= (1 << 7);
	DMA1_Channel3->CCR |= (1 << 7);
	
	/* ���ý��պͷ��͵��ڴ��ַ */
	DMA1_Channel2->CMAR = (uint32_t)RecvBuff;
	DMA1_Channel3->CMAR = (uint32_t)tx_buf;

	/* ��ȡһ��DR��ʹ����� */
	SPI1->DR;
	
	/* �ȴ�������Ϊ�� */
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
	
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* ��DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* ������� */
    while( DMA_GetITStatus(DMA1_FLAG_TC2) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC3) == RESET );
	
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
    
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	
	/* ���DMA������ɱ�־ */
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
void SPI2_Trans_Write(uint8_t *tx_buf,     //����bufferָ��
					uint16_t length)     //�������ݳ���
{
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	
	/* ���ô����ֽ��� */
	DMA_SetCurrDataCounter(DMA1_Channel4, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel5, (uint16_t)length);
    
	/* ���ͽ������ݴ洢��ַ���� */
	DMA1_Channel4->CCR |= (1 << 7);
	DMA1_Channel5->CCR |= (1 << 7);
	
	/* ���ý��պͷ��͵��ڴ��ַ */
	DMA1_Channel4->CMAR = (uint32_t)RecvBuff;
	DMA1_Channel5->CMAR = (uint32_t)tx_buf;

	/* ��ȡһ��DR��ʹ����� */
	SPI2->DR;
	
	/* �ȴ�������Ϊ�� */
	while( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET );
	
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* ��DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	/* ������� */
    while( DMA_GetITStatus(DMA1_FLAG_TC4) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC5) == RESET );
	
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE);
    
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);	
	
	/* ���DMA������ɱ�־ */
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
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);
	
	/* ���ô����ֽ��� */
	DMA_SetCurrDataCounter(DMA1_Channel2, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel3, (uint16_t)length);
	
	/* ����DMA���ķ��͵�ַ������ */
    DMA1_Channel3->CCR &= ~(1 << 7);
	
	/* ���ý��պͷ��͵��ڴ��ַ */
    DMA1_Channel2->CMAR = (uint32_t)rx_buf;
    DMA1_Channel3->CMAR = (uint32_t)&tx_data;

	/* ��ȡһ��DR��ʹ����� */
	SPI1->DR;
	
	/* �ȴ�������Ϊ�� */
	while( SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET );
	
    /* ����SPI_DMA����ͨ�� */
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* ��DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, ENABLE);
	DMA_Cmd(DMA1_Channel3, ENABLE);
	
	/* ������� */
    while( DMA_GetITStatus(DMA1_FLAG_TC2) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC3) == RESET );
	
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel2, DISABLE);
	DMA_Cmd(DMA1_Channel3, DISABLE);	
	
    /* ����SPI_DMA����ͨ�� */
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI1,SPI_I2S_DMAReq_Tx,DISABLE);
	
	/* ���DMA������ɱ�־ */
	DMA_ClearFlag(DMA1_FLAG_TC2);
	DMA_ClearFlag(DMA1_FLAG_TC3);
}

/**
  *	@breif  The spi dma trans function.���ı�SPI��ͨ�ŷ�ʽ
  * @param  rx_buf -- the point of rx buffer
  * @param  tx_data -- the spi tx data.
  * @elngth length -- the size of data.
  * @retval None
  */
void SPI2_Trans_Read(uint8_t *rx_buf,
					uint16_t length)
{
    uint8_t tx_data=0xFF;
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);
	
	/* ���ô����ֽ��� */
	DMA_SetCurrDataCounter(DMA1_Channel4, (uint16_t)length);
	DMA_SetCurrDataCounter(DMA1_Channel5, (uint16_t)length);
	
	/* ����DMA���ķ��͵�ַ������ */
    DMA1_Channel5->CCR &= ~(1 << 7);
	
	/* ���ý��պͷ��͵��ڴ��ַ */
    DMA1_Channel4->CMAR = (uint32_t)rx_buf;
    DMA1_Channel5->CMAR = (uint32_t)&tx_data;

	/* ��ȡһ��DR��ʹ����� */
	SPI2->DR;
	
	/* �ȴ�������Ϊ�� */
	while( SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET );
	
    /* ����SPI_DMA����ͨ�� */
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,ENABLE);
    
	/* ��DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);
	
	/* ������� */
    while( DMA_GetITStatus(DMA1_FLAG_TC4) == RESET );
    while( DMA_GetITStatus(DMA1_FLAG_TC5) == RESET );
	
	/* �ر�DMAͨ�� */
	DMA_Cmd(DMA1_Channel4, DISABLE);
	DMA_Cmd(DMA1_Channel5, DISABLE);	
	
    /* ����SPI_DMA����ͨ�� */
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Rx,DISABLE);
    SPI_I2S_DMACmd(SPI2,SPI_I2S_DMAReq_Tx,DISABLE);
	
	/* ���DMA������ɱ�־ */
	DMA_ClearFlag(DMA1_FLAG_TC4);
	DMA_ClearFlag(DMA1_FLAG_TC5);
}



