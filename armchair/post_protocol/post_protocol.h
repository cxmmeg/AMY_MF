#ifndef _POST_H
#define _POST_H
#include "stm32f10x.h"
//ȫ�ֺ���
void GATEWAY_heart_send_pack(void);
void GATEWAY_list_send_pack(void);
void GATEWAY_peizhi_pack(void);
void post_group(u8 *post_buf2,u16 post_buf1_len,u8 command);
void poll_list_send_pack(void);
void SIM_ID_send_pack(void);
void amy_off_send_pack(void);
//ȫ�ֱ���
extern u8 post_buf[];   // ����ݴ���
extern u16 post_len;				// �������
extern u8 NO[16];
extern unsigned char CRC8_Table(unsigned char *p, char counter);  

#define POST_VER 0x22	 	//ͨ��Э��
#define POST_ADR 0x00	 	//�豸��ַ
#define POST_CID1 0xd0	 	//���Ʊ�ʶ�룬��Ħ�ι̶�0XD0
#define AMY_HEART 0x1d	 	//��Ħ������
#define AMY_LIST 0x1c	 	//��Ħ������
#define AMY_STATE 0x1b	 	//��Ħ��״̬
#define AMY_SET 0x1e	 	//��Ħ������
#define AMY_PEIZHI 0xfb	 	//��Ħ������
#define AMY_addr_please 0x24	 	//��Ħ��ֱ�ӵ�ַ��ѯ����ַ����ʱ�䣬����ֱ������������ֱ�Ӳ�ѯ��Ħ��״̬
#define AMY_list_state 0x25 //�б�״̬�ϴ�,�������б����һ����������ֻ����ÿ̨�ն�״̬�����ϴ��ն�ID

#endif 
