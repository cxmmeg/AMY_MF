
#ifndef _cdma_H
#define _cdma_H
#include "stm32f10x.h"

void cdma_init(void);
u8 cdma_send(u8 *p,u16 len);
void cdma_reconnect(void);
u16 strsearch_h(u8 *ptr2,u8 *ptr1_at);//���ַ���*ptr2��*ptr1�е�λ��
void cdma_open_GPRS(void);
void CDMA_IO_Init(void);
void cdma_init_0(void);
void clear_SystemBuf(void);
void GSM_RESET(void);
void Start_GSM(void);	
void TP_GPRS(u8 *p,u8 *q);
extern u8 light_TCP[22];
extern u8 SIM_ID[20];
extern u8 g_flag;//G����־
extern u8 cdma_RESET_flag;
#define CDMA_RESET GPIO_Pin_15	 	//�ܽź궨��PE13
#define CDMA_ON_OFF GPIO_Pin_13	 	//�ܽź궨��PE13
#define CDMA_IO GPIOC	 	//�ܽź궨��

#endif

