//�ĵ�˵��
//��������ĳ��������Ҫ�ȴ�һ����CPUʱ�ӣ���ĳ���ж��в�����ɲ���
//          ���ⲿ������Ҫ��ĳһ������δ���ʱ�ͼ�����������������Ҫ
//          һ������������������ӵ������У����ò�����ɻص�����
//          �����������������Ӻ��Ƴ���ʹӦ�ò���ø���

//��������ͻ�������Ӧ��������Ҫ�ǲ����������Ƿ���Ҫһֱ��CPU�����У�
//����󲿷�ʱ�䲻��ҪCPU��Ԥ����ѡ�� OperationList �� ����ѡ�� BoxOs_MutexLock

#ifndef  __LIST__H
#define  __LIST__H

#define LIST_OPTION_WHEN_MATCH_USERID	0u
#define LIST_OPTION_WHEN_MATCH_USERIP	1u
#define LIST_OPTION_WHEN_MATCH_USERNAME	2u

typedef struct ListData_tag
{
    int     User_ID;        //�û�ID
    char    User_IP[4];     //�û�IP
    short   User_Port;      //�û��˿�
    char    User_Name[50];  //�û���
    
    /* ����״̬ User_State0 != User_State1 */
    /* ����״̬ User_State0 == User_State1 */
    /* ������������ʼ��Ϊ��ͬ״̬���жϵ���״̬��ͬʱ���޸�Ϊstate0=state1��
    ���յ�����֪ͨʱ��state0���䣬state1++ */
    int     User_State0;    //�û�״̬0
    int     User_State1;    //�û�״̬1
}ListData;

struct ListCtrl_tag;
//����Ԫ��
typedef struct List_tag
{
    struct List_tag*        pLast;      //ָ����һ��Ԫ��
    struct List_tag*        pNext;      //ָ����һ��Ԫ��
    struct ListCtrl_tag*    pListCtrl;  //����������ĸ������������
    struct ListData_tag     Data;       //����
}List;

//���������
typedef struct ListCtrl_tag
{
    List* pHead;
    List* pTail;
    int   nCnt;   //nCnt��Ϊһ���ο����� 
}ListCtrl;


ListCtrl*   Create_List (void                       );  //��������
void        AddNode_List(ListCtrl* pCtrl,List* toAdd);  //���ӽڵ�
List*       FindNode_List(ListCtrl* pCtrl, char option, ... );//���ҽڵ�
void        DelNode_List(ListCtrl* pCtrl,List* toDel);  //ɾ���ڵ�

#endif
