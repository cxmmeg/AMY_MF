#ifndef _TM1618_H
#define _TM1618_H
#include "stm32f10x.h"
//�#include "systick.h"
//�����ֲ����ʱֻҪ��һ�������ط�������
/* ��ʱʹ�õ�IO�� */
#define TM1618_STB GPIO_Pin_6	  //PC6
#define TM1618_SCL GPIO_Pin_7	  //PC7
#define TM1618_SDA GPIO_Pin_8	  //PC8
#define GPIO_TM1618 GPIOC

#define TM1618_SCL_H GPIO_SetBits(GPIO_TM1618,TM1618_SCL)
#define TM1618_SCL_L GPIO_ResetBits(GPIO_TM1618,TM1618_SCL)

#define TM1618_SDA_H GPIO_SetBits(GPIO_TM1618,TM1618_SDA)
#define TM1618_SDA_L GPIO_ResetBits(GPIO_TM1618,TM1618_SDA)

#define TM1618_STB_H GPIO_SetBits(GPIO_TM1618,TM1618_STB)
#define TM1618_STB_L GPIO_ResetBits(GPIO_TM1618,TM1618_STB)
/* TM1618�������� */
#define TM1618_DISPLAY_EN 0x8a;//������ʾʹ��
#define TM1618_WRITE_EN 0x40;//д����ʹ��
/* ����ȫ�ֺ��� */
void Tm618_IO_Init(void);
void Tm618_Smg_No(u8 no_data,u8 no_adr);
void Write_byte(u8 Byte);
void Write_Cmd(u8 cmd);
void TM1618_SDA_OUT(void);
void TM1618_SDA_IN(void);
void Tm618_Display(void);
u8 read_key(void);
void Tm618_Smg_clear(void);
u8 rec_key(void);
void Tm618_Smg_On(void);
void Tm618_Smg_Line(u8 no_adr);
void Tm618_Smg_8881(void);
void Tm618_Smg_8882(void);
/* ����ȫ�ֱ��� */
extern u8 TM1618_DISPLAY_NO[3];
extern u8 change_led_flag;
extern u8 key_delay_out;//������ʱ��δ�ж������Զ��˳����ý���
#endif
