#ifndef MODULE_SELECT_H
#define MODULE_SELECT_H
#define 	BOOT_CL 		0
#define  	BOOT_HD 		1
//#if BUILD_PROJECT == BOOT_HD
#if 1
	/*--ϵͳ����Ԥ�������--*/
	#define 	CONF_WATCHDOG_EN			  1			/*ʹ������Ź�*/
	#define		CONF_UART1_EN						1			/*�Ƿ�ʹ�ô���1����*/
	#define		CONF_UART2_EN						1			/*�Ƿ�ʹ�ô���2����*/
	#define		CONF_UART3_EN						1			/*�Ƿ�ʹ�ô���3����*/
	#define		CONF_UART4_EN						0			/*�Ƿ�ʹ�ô���4����*/
	#define		CONF_UART5_EN						1			/*�Ƿ�ʹ�ô���5����*/
	#define		CONF_LED_EN							1			/*ʹ��Э��*/
	#define		CONF_PROTOCOL_EN				1			/*ʹ��Э��*/
	#define		CONF_TIMER_EN  					1			/*ʹ����ʱ��*/
	#define		CONF_IAP_EN						  1			/*�������ʹ��*/
	#define		CONF_CESHI_EN						1			/*���Դ���궨��*/
	#define		CONF_QWL_NET_EN					1			/*���Դ���궨��*/
	#define		CONF_SPI_FLASH_EN				1			/*=1�ⲿflash����,=0�ڲ�flash����*/
	#define		CONF_ARMCHAIR_EN				1			/*��Ħ��ʹ��*/	
/*********************************************************************************************/
#else
	/*--ϵͳ����Ԥ�������--*/
	#define 	CONF_WATCHDOG_EN			        1			/*ʹ������Ź�*/
	#define		CONF_UART1_EN						1			/*�Ƿ�ʹ�ô���1����*/
	#define		CONF_UART2_EN						0			/*�Ƿ�ʹ�ô���2����*/
	#define		CONF_UART3_EN						0			/*�Ƿ�ʹ�ô���3����*/
	#define		CONF_UART4_EN						0			/*�Ƿ�ʹ�ô���4����*/
	#define		CONF_UART5_EN						0			/*�Ƿ�ʹ�ô���5����*/
	#define		CONF_PROTOCOL_EN					1			/*ʹ��Э��*/
	#define		CONF_TIMER_EN  						1			/*ʹ����ʱ��*/
	#define		CONF_IAP_EN						    1			/*�������ʹ��*/
#endif


#endif
