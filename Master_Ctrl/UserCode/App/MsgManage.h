/*
*********************************************************************************************************
*
*
*                                       Message Management
*                                         �ļ���Ϣ����
* File : MsgManage.h
* By   : MaxsGao
* Date : 2019/05/06
* version : V1.0
* NOTE(s) : ��Ҫ�������Ϣ�ļ������ࣺ�洢����������Ϣ���ļ����洢�����ӽڵ���Ϣ���ļ����洢ģʽ��Ϣ���ļ�
* History : 
*
*
*********************************************************************************************************
*/
#ifndef   __MSGMANGAGE_H
#define   __MSGMANGAGE_H

#ifdef __cplusplus
 extern "C" {
#endif
     

/*
*********************************************************************************************************
*                                       INCLUDES            ͷ�ļ�
*********************************************************************************************************
*/


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
/**
 * @brief Peer IP register address
 */


/*
*********************************************************************************************************
*                                       DATA TYPE           ��������
*********************************************************************************************************
*/

/** 
 * @brief  CAN init structure definition
 */
//typedef struct
//{
//  uint16_t CAN_Prescaler;   /*!< Specifies the length of a time quantum. 
//                                 It ranges from 1 to 1024. */
//  
//  uint8_t CAN_Mode;         /*!< Specifies the CAN operating mode.
//                                 This parameter can be a value of 
//                                @ref CAN_operating_mode */
//} CAN_InitTypeDef;


/*
*********************************************************************************************************
*                                       FUNCTION STATEMENT  ��������
*********************************************************************************************************
*/
void msg_confload(void);
void msg_confupdate(void);
void msg_confselect(void);

void msg_nodeload(void);
void msg_nodeupdata(void);
void msg_nodeselect(void);

void msg_modeload(void);
void msg_modeupdate(void);
void msg_modeselect(void);



#ifdef __cplusplus
}
#endif

#endif
