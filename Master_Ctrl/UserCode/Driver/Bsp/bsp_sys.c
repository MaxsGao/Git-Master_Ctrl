#include "bsp_sys.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK Mini STM32������
//ϵͳ�жϷ������û�		   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/10
//�汾��V1.4
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2009-2019
//All rights reserved
//********************************************************************************  
//THUMBָ�֧�ֻ������
//�������·���ʵ��ִ�л��ָ��WFI  
void WFI_SET(void)
{
	__ASM volatile("wfi");		  
}
//�ر������ж�
void INTX_DISABLE(void)
{		  
	__ASM volatile("cpsid i");
}
//���������ж�
void INTX_ENABLE(void)
{
	__ASM volatile("cpsie i");		  
}
//����ջ����ַ
//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}


#ifdef _UART_DEBUG_
    #pragma import(__use_no_semihosting)             
    //��׼����Ҫ��֧�ֺ���                 
    struct __FILE
    { 
        int handle; 
    }; 

    FILE __stdout;       
    //����_sys_exit()�Ա���ʹ�ð�����ģʽ    
    void _sys_exit(int x) 
    { 
        x = x; 
    } 
    //�ض���fputc���� 
    int fputc(int ch, FILE *f)
    { 	
        while((USART3->SR&0X40)==0);//ѭ������,ֱ���������   
        USART3->DR = (u8) ch;      
        return ch;
    }
#endif
