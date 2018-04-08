/**********************************************
* File Name          : net.h
* Author             : Ǯ����
* Description        : ��̫��Ӧ�ýӿں�����MSG��
********************************************************************************/
#ifndef __AMY_NET_H__
#define __AMY_NET_H__
#include "stm32f10x.h"

typedef __packed struct
{
			u16 poll_max_time;//��ѯ���ʱ��
			u16 poll_time; //��ѯʱ��
			u8 zd_list[13];			//�ն��б�
			u8 zd_state[25];	//�ն�״̬
			u8 list_please_flag;//�б������־
			u8 state_send_flag;//״̬���ͱ�־
			u8 poll_deal;//��ѯ������
}POLL_ZD_TYPE;
extern POLL_ZD_TYPE poll_zd;

//��Ħ�γ���
void amyack_send_pack(u8 state);//״̬�������
void amystate_send(u8 state);
void eth_rec_deal(u8 ch);
void GATEWAY_send(void);//�������������ʹ������
void amy_state_send(void);//��Ħ��״̬����
void amystate_send_pack(u8 state,u8 addr);//��Ħ��״̬���
void USART3_sendstring(u8 *p);
void USART3_sendchar(u8 ch);
void usart3_init(void);
void amystate_pack_send(u8 state,u8 addr);
void usart5_init(void);
void wg_amy_state(void);//����ֱ����Ħ�β�ѯ״̬



//��Ħ��ȫ�ֺ���
extern u16 GATEWAY_heart_time_flag;   // ����������ʱ��ʱ��ר�����ؼ�ʱ
extern u16 GATEWAY_heart_max_flag;   // ���������ӳ�ʱ�䣬Ĭ��60��
extern u16 GATEWAY_list_time_flag;
extern u16  eth_Len;
extern u8	getwaylist_send_flag;//�б��ͱ�־��������0ʱ��Ҫ���б��ϴ�


extern u16 GATEWAY_off_time_flag;
extern u8	getwayoff_send_flag;//�б��ͱ�־��������0ʱ��Ҫ���б��ϴ�

extern u8 amy_check_ID[16];//��Ħ��Ҫ��ѯ���ն��Ȼ���
extern u8 amy_config_rec_flag;//��Ħ�����ñ�־λ	

//extern u8 deal_process;//������̱�־���ʵ粿Ҫ��һ��һ�����������ôα�־

extern u8 plc_cofig_buf[16];//��Ħ��������ʱ�洢���ڷ���
extern u8 plc_cofig_buf_1[3];//��Ħ������ʱ���ģʽ


extern u16 eth_Len;

extern u8 amy_storflash_flag;//��־��0��flash��1ѭ����һ��Ȼ�����flash��2���ȴ����նˣ�Ȼ�����flash

extern u8 amystate_send_flag[];//��Ħ��״̬���ͱ�־

extern u8 CSQ;
extern u8 CSQ_DELAY;

#endif
