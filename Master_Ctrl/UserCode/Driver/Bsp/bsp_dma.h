#ifndef __BSP_DMA_H
#define	__BSP_DMA_H	   
#include "bsp_sys.h"

void SPI1_DMA_Config (void                           );
void SPI1_Trans_Write(uint8_t *tx_buf,uint16_t length);
void SPI1_Trans_Read (uint8_t *rx_buf,uint16_t length);

void SPI2_DMA_Config (void                           );
void SPI2_Trans_Write(uint8_t *tx_buf,uint16_t length);
void SPI2_Trans_Read (uint8_t *rx_buf,uint16_t length);

#endif




