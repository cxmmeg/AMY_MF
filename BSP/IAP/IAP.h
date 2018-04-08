#ifndef _IAP_H
#define _IAP_H
#ifdef __cplusplus
extern "C" {
#endif

#include"config.h"

#define iap_data_add  0x08007800    /*256���ֽ�,128����*/

typedef __packed struct
{
  	char md5[8];
	DWORD length;	//����
}tagWCDMAUpdateArg;

/*iap�õ�������*/
typedef __packed struct
{
	/*����ͨ�Ų���*/
	uint16_t USART_num;   					/*���ں�			   */
	uint16_t remap;						    /*�Ƿ���ӳ��		   */
	uint32_t bps;         					/*������			   */
	uint16_t writecompleteflag;             /*�û�����������	   */
	uint16_t writeflag;					    /*�û������̱�־	   */
	uint16_t InitUserParFlag;				/*��ʼ���û�������־   */
	/*����ͨ�Ų���*/ 
	uint8_t rip[4];
	uint16_t rport; 
	uint8_t lip[12];
	uint8_t lown_hw_adr[6];
	/*������*/
	u32 copyflag;							/*���Ʊ�־λ 0��Ч 	   */
	u32 src_add;							/*Դ��ַ			   */
	u32 dest_add;							/*Ŀ���ַ			   */
	u32 copy_long; 							/*���ȣ�19+8=27������  */
	uint16_t Address;						/*�豸��ַ*/
	tagWCDMAUpdateArg WCDMAUpdateArg;		/*Ӧ�ó�������,�ϵ���������*/
	uint16 CheckSum;						/*У���,�˲������ڽṹ��β��*/
}IAP;


#define iap_constlong  sizeof(IAP)/2        /*iap����, ���֣�16λ������*/

extern IAP iap_data ;

void goto_iap(void) ;
int Save_Iap_Config(void);
int Get_Iap_Config(void);

#ifdef __cplusplus
}
#endif

#endif

