#ifndef _armchair_H
#define _armchair_H

#include "config.h"
//#define ac_status_in GPIO_Pin_9	 	//�ܽź궨��PB9 �����������
//#define ac_switch GPIO_Pin_0	 	//�ܽź궨��PE0 ״̬���
#define ac_status_out GPIO_Pin_1	 	//�ܽź궨��PE1 ״̬�����
#define ac_on 0;
#define ac_off 1;

typedef enum {
		ruilaikesi,			//������˹�豸
	oxygen_plant,			//�������豸
	slot_machines,		//Ͷ�һ��豸
	Module_TYPE_MAX 		
}Module_TYPE;

typedef __packed struct
{

			Module_TYPE Module;
}Shared_TYPE;

extern Shared_TYPE Shared_Module;//�豸ѡ��
//ȫ�ֺ���
void Armchair_IO_Init(void);
void Armchair_Init(void);
void ARMCHAIR_TIME(void);
void ARMCHAIR_DEAL(void);
void ARMCHAIR_S_IQR(void);
void Shared_Module_switch(u8 module_state);
void slot_machines_data_send(void);
#define        armchair_status_IN					PINI(PIN_armchair_status_IN)
#define        armchair_switch						PINO(PIN_armchair_switch)
#define        armchair_status_OUT				PINO(PIN_armchair_status_OUT)
#define   	   GPIO_armchair_INIT() 	    _GPIO_armchair_INIT()
u8 armchair_io_state(void);
void Amy_Record_Store(u8 k);
u8 armchair_change_outIO(void);
u8 armchair_io_state_1(void);
//ȫ�ֱ���
extern u8 amy_config;
extern u8 amy_config1;
extern u8 PLC_time_flag;//�ն˶�ʱ���Ͷ�ʱ
extern u8 time_share_process_flag;//��Ħ�η�ʱ�����ʶ
extern u16 zd_overtime_flag;//�ն˳�ʱ��ʶ����ʱû���յ����������б�־��λ
extern u8 AmyID[16];
extern u8 ceshi;
extern u8 PLC_time_tiqian;

extern u8 amy_record_buf[];		//��Ħ�����ݻ�����
extern u8 amy_record_buf_1[];		//��Ħ�����ݻ���������ʱ��ŵ�ַ
extern u8 amy_rec_flag;				//��Ħ�ν������ݱ�־λ
extern u8 amy_rec_flag_1;//��Ħ�ν��ܱ�־λ��������	
extern u8 amy_total;//��Ħ������
extern u8 armchair_time_flag;//��Ħ��״̬��ȡ��ֹ��ʱ��־
extern u8 amy_CHKSUM;//��Ħ��У���룬�����ն�ID��Ӻ�
extern u8 getwaylist_send_times;
extern u16 amy_cotrol_time;//��Ħ�ο���ʱ��
extern u16 amy_cotrol_time_flag;//��ʱ���м�ʱ
extern u8 amy_cotrol_flag;//��Ħ�ο��Ʊ�־


extern u8 wg_ack_buf[200];//�����յ����ݺ󣬽��з������棬�����������ڴ���
extern u8 wg_stor_pointer;//���ش洢ָ��
extern u8 wg_deal_pointer;//���ش���ָ�룬��ǰ������λ��
extern u8 wg_ack_times;//��Ҫ����������

extern u8 wg_state_flag;//��ѯ����������Ħ��״̬��־
extern u8 wg_set_flag;//����ֱ��������Ħ�α�־

extern u8 zd_update_flag;//�ն�������־

extern u8 SIM_GET_FLAG;//��ȡSIM�����
extern u8 shebei_flag;//�豸״̬����
#endif 
