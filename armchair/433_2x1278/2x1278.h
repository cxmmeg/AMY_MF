#ifndef _2X1278_H
#define _2X1278_H
#include "stm32f10x.h"

#define q2x1278_M0 GPIO_Pin_5 //PE5
#define q2x1278_M1 GPIO_Pin_6 //PE6

#define   MAX_RX_BUF  200			//����
extern u8 Uart_RecData_buf[];
extern u8 rx_pointer;

#define   MAX_RX2_BUF  200			//����
extern u8 Uart2_RecData_buf[];
extern u8 rx2_pointer;	

extern u8 q2x1278_config_buf[];//433���ò����洢
extern u8 q2x1278_config_buf1;//���ڰ����ڵ���ģ���ַʱ��ʱ�洢�����㰴����ʱ�˳�ʹ��
extern u8 config_recwait_buf[];//���÷����ȴ�����
extern u16 config_recwait_time;//���÷����ȴ���ʱ
extern u8 sx1278_fb_flag;
void q2x1278_init(void);
void q2x1278_plc_send(void);
u8 q2x1278_check_send(void);
u8 q2x1278_rec_cofig(void);
u8 q2x1278_set(void);
u8 amy_config_send(void);
u8 amy_config_send_1(u8 addr);
u8 amy_ack_send(void);
u8 amy_config_please(u8 *addr);

void q2x1278_fb(void);
u8 amy_amy_please(u8 *send_buf,u8 len,u8 command,u8 *addr);
void USART2_sendchar(u8 ch);
#define plc_state 0xa0;



#endif

