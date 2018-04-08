#ifndef _CONFIG_H_
#define _CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************************************/
/*ϵͳͷ�ļ�**/
#include "stm32f10x.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************/

	#define DEBUG(format, ...) 


/*********************************************************************************************/
/*--�ײ�����ģ��--*/
/*MCUģ��*/
#include "./MCU/MCU.h"

/*����ģ�鿪��ͷ�ļ�*/
#include "moduleSelect.h"

/*�豸ϵͳȫ����ͷ�ļ�*/
#include "./data/data.h"

/*API����ͷ�ļ�*/
#include "./api.h"

/*GPIO��ʼ������*/
#include "./GPIO/GpioMap.h"	

/*ϵͳ��ʱ��ģ������ͷ�ļ�*/
#include "./LowPrecisionTime/LowPrcisionTime.h"		

/*WatchDogģ������ͷ�ļ�*/
#include "./WatchDog/WatchDog.h"

/*IAPģ������ͷ�ļ�*/
#include "./IAP/IAP.h"

/*��������ͷ�ļ�*/
#include "./basicFunc/basicFunc.h"

/*��������ͷ�ļ�*/
#include "./USART/USART.h"

/*LEDģ������ͷ�ļ�*/
#include "./LED/LED.h"

/*LEDģ������ͷ�ļ�*/
#include "./CESHI/CESHI.h"

/*flashģ������ͷ�ļ�*/
#include "./SpiFlash/Spi_Flash.h"

/*Э���������ͷ�ļ�*/
#include "./Protocol.h"

/*�ʵ粿Э���������ͷ�ļ�*/
#include "./PostProDa/PostProDa.h"

/*�ʵ粿Э�������ϱ���Э��ͷ�ļ�*/
#include "./PostProDa/Uploading_Pro.h"

/*IAP��������Э��ͷ�ļ�*/
#include "./DownLoad/DownLoad.h"

/*����Э�麯��ͷ�ļ�*/
#include "./DownLoad/DownLoad_Pro.h"

/*Э����ջص���������*/
#include "./ProtocolRecCallBack.h"

/*IAP��������ͷ�ļ�*/
#include "./iap_updata/iap_updata.h"

/*����ģ������ͷ�ļ�*/
#include "./qwl_net.h"

/*NBģ������ͷ�ļ�*/
#include "./nblot/nblot.h"

/*CDMAģ������ͷ�ļ�*/
#include "./cdma/cdma.h"

/*Gģ������ͷ�ļ�*/
#include "./gsm/M6312.h"

/*Gģ������ͷ�ļ�*/
#include "./g510/G510.h"

/*Э������������λ���ļ�*/
#include "./Protocol_send.h"

/*��Ħ������ͷ�ļ�*/
#include "./armchair.h"

/*TM1618����ͷ�ļ�*/
#include "./TM1618/TM1618.h"

/*SX1278����ͷ�ļ�*/
#include "./433_2x1278/2x1278.h"

/*amy_net����ͷ�ļ�*/
#include "./amy_net/amy_net.h"

/*flash����ͷ�ļ�*/
#include "./flash/flash.h"

/*�ʵ粿Э������ͷ�ļ�*/
#include "./post_protocol/post_protocol.h"

/*algorithm����ͷ�ļ�*/
#include "./algorithm/algorithm.h"

#include "Plug.h"
#include "Protool.h"
/*********************************************************************************************/
#ifdef __cplusplus
} // extern "C"
#endif

#endif

