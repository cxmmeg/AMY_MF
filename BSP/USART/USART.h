#ifndef USART_H
#define USART_H

#include "protocol.h"

/*����ͨ������EEPROM�洢��ַ,����0x0091�ֽ�*/
#define	EEP_ADDR_COMMBPS    _EEP_ADDR_COMMBPS

/*********************************************************************************************/
 /*����ѡ��*/
typedef enum
{
	USART_TYPR_COM1,		  	//����1		 
	USART_TYPR_COM2,	   		//����2
	USART_TYPR_COM3,		   	//����3
	USART_TYPR_COM4,		   	//����4
	USART_TYPR_COM5,		   	//����5
   	USART_TYPR_MAX
}USART_TYPE;

/*����ģʽ*/
typedef enum
{
	USART_MODE_TYPE_IRQ,
	USART_MODE_TYPE_DMA
}USART_MODE_TYPE;
/*********************************************************************************************/
typedef int (*UartFun)(void);			//���ڷ��ͺͽ��պ���
typedef void (*USARTx_Init)(void);		//���ڳ�ʼ����������
typedef void (*USARTx_IRQHandler)(void);//���ڴ�������
/*********************************************************************************************/
/*���ڲ������ƽṹ��*/
typedef __packed struct {
	uint32_t baudRate;	 				//������
	uint16_t parity;					//У��λ
	uint16_t wordLength;				//���ݳ���
	uint16_t stopBits;					//ֹͣλ
	uint16_t hardwareFlowControl;	   	//Ӳ��������
	uint16_t mode;						//ģʽ
}tagUartCommAag;

/*���ڲ������ƽṹ��*/
typedef __packed struct {
	USARTx_Init init;			 		//��ʼ������
	USARTx_IRQHandler irqHandler;	   	//��������
	USART_TypeDef* USARTx;				//��Ӧ�Ĵ��ڵ�ַ
	tagUartCommAag UartCommAag;			//���ڲ���
}UartCom;

/*********************************************************************************************/
/*�����빦��ӳ��*/
typedef __packed struct {
	USART_TYPE usartType;				//�������ͣ�Com1- Com5
	USART_MODE_TYPE mode;				//����ģʽ IRQ DMA
	USART_FUNCTION 	usartFun;			//���ܺ�����WCDMA��
}UsartTypeToFunctionStruct;

/*********************************************************************************************/
/*����ӳ�书�ܽṹ��*/
typedef __packed struct 
{
   	UsartTypeToFunctionStruct* uTypeToFun;		//���ڹ��ܵ�ӳ��	
	UartCom *uCom;								//ֻ��ĳ������������صĲ���
	BOOL ready;									//�Ƿ���� 
}UartStruct;
/*********************************************************************************************/
void Uart_Init(void);
int DMA_Usart_Init(USART_FUNCTION fun);
UartStruct* GetUartStructBaseFunction(USART_FUNCTION fun);	//����Э�����ͻ�ȡ��صĴ���������Ϣ
U8 ReceiveDataFromUartStruct(UartStruct* us);				//�Ӵ��ڽ���һ������
int SendDataFromUartStruct(UartStruct* us, u8 data);  		//�򴮿ڷ���һ������
int EnableCom(UartCom* com,BOOL enable);  					//ʹ�ܻ���ʧЧһ������
int SetupComDir(UartStruct* us,BOOL State);   				//���Ʒ���
void SettingSaveByUSART_FUNCTION(USART_FUNCTION uf);		//���ô��ڲ���
/*********************************************************************************************/
#endif


