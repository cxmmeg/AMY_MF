#ifndef _qwl_net_H
#define _qwl_net_H
#include "stm32f10x.h"
#include "config.h"

typedef enum {
	PLATFORM_CHOICE_PUBLIC,//����
	PLATFORM_CHOICE_UPDATE	//���� 		
}PLATFORM_CHOICE;

typedef __packed struct
{
	u8 UPDATE_overtime_readnb;//��ʱ��NBʱ���־��ÿ��ʮ���һ��
	u16 UPDATE_link_overtime;//��ʱʱ�� //û���յ�����ָ��
	u16 MAX_UPDATE_link_overtime;//���ʱʱ�� //û���յ�����ָ��
	u8 UPDATE_link_times;//���Ӵ���
	u8 MAX_UPDATE_link_times;//������Ӵ������������л�������ƽ̨
	PLATFORM_CHOICE Platform_Choice;// ƽ̨ѡ��������������ʹ��ƽ̨	
}EXCHANGE_PLATFORM;

extern EXCHANGE_PLATFORM exchange_platform;

typedef enum {

	LINK_OK,			//����OK
	LINK_IP,			//����IP
	OPEN_GPRS,		//������
	MODE_INIT,	//ģ���ʼ��
	MODE_CHOICE,//ģ��ѡ��
	SETP_TYPE_MAX 		
}SETP_TYPE;

typedef __packed struct
{
			
			u8 delay_times; //��ʱ����
			u8 delay;			//�����еĳ�ʱ����
			u8 setp_setp;	//ÿ�����������еĲ���
			SETP_TYPE reconnect_setp;//��������
			MODE_TYPE mode_type;
}CONNECT_TYPE;

extern CONNECT_TYPE net;

u8 net_state(void);
void net_rec(u8 k);
u8 net_send(u8 *p,u16 len);
void qwl_net_init(void);
void cdma_or_nblot(void);
void qwl_net_sendstring1(u8 *p);
void qwl_net_sendstring(u8 *p);
void qwl_net_sendchar(u8 ch);



extern u8 cdma_or_nblot_flag;
extern u8 connect_flag;

extern u16 Rx;
#define  RxIn 1024
extern u8 SystemBuf[];  //������ڽ�������

const extern	u8 ASCII_hex[24];
extern u8 net_error_flag;//��������־
extern u8 deal_process;
extern u8 update_ip[22];

extern u8  getwayheart_sendok_flag;//�����з��ͳɹ���ȷ�ϱ�־
extern u8  getwaylist_sendok_flag;//�б��з��ͳɹ���ȷ�ϱ�־
extern u8  getwayoff_sendok_flag;//�б��з��ͳɹ���ȷ�ϱ�־
extern u8 suipian_delay;
#endif 
