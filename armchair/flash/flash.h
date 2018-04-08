#ifndef _flash_H
#define _flash_H
#include "stm32f10x.h"

	#define     HARDWARE_VER		  		1.00
	#define     SOFTWARE_VER		  		3.77
		#define		COMPANY_NAME				"ModouTech           "	 		/*��˾����*/
		#define		FACTNAME					"armchair gateway    "	 	                /*�豸�ͺ�����*/	


void Write_Flash(u8 *flash_data,u32 flash_adr,u16 flash_len);

void Write_Flash_ALLAMY(void);


void Write_light_idip(void);

void Init_Amy_Id(void);


extern u8 software_version[2];//����汾
extern u8 Hardware_version[2];//Ӳ���汾
extern u8 Upgrade_ip[22];//����IP
extern u8 carrieroperator;//��Ӫ��
extern u8	module_name;//ģ���ͺ�
extern u8 amy_max_total;//��Ħ��������������
//extern u8 SIM_ID[20];//SIM��ID
extern u8 zd_sl_flag;//�ն�����ˢ���־
//����ȫ�ֱ���
//extern u32 flash_data;

#define flash_AMY_adr1 0x0803D000	//�������洢���ı������ݵ���ʼ��ַ,0x0803D000~0x0803E800��һ��6K���洢��Ħ��ZHON����

#define  AMYIDIP_adr 0x0803e800//��Ħ�δ洢IDIP��ַ



#endif
