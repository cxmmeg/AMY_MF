#include"IAP.h"


IAP iap_data = {0};

/************************��д����**********************************************
* ������   	     : goto_iap
* ˵��   	     : ��Ȩ�޽���iapģ�飬�����û�����
*******************************************************************************/
void goto_iap(void)
{
	__disable_fault_irq();

	iap_data.writeflag=0x00;  //�������Ӧ�ó���

	Save_Iap_Config();

	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
	NVIC_SystemReset();
}

//������������������������������������������������������������������������������
// ��������  : Save_Iap_Config
// ��������  : ���浱ǰ�����û��������
// �������  : None.
// �������  : None
// ���ز���  : �ɹ� 0  ʧ�� -1
//������������������������������������������������������������������������������
int Save_Iap_Config(void)
{
	int i;
	char *ps = (char*)&iap_data;

	iap_data.CheckSum = 0;

	for(i = 0; i<sizeof(IAP)-2; i++)
	{
		iap_data.CheckSum += *ps;
		ps++;	
	}

	
	FLASH_Unlock();

	FLASH_ErasePage(iap_data_add);

	for(i=0;i<iap_constlong;i++)
	{
		FLASH_ProgramHalfWord( (iap_data_add+(i<<1)), *(u16*)((u32)&iap_data+(i<<1)));
	}

	FLASH_Lock();

	return 0;
}

//������������������������������������������������������������������������������
// ��������  : Get_Iap_Config
// ��������  : ��ȡ��ǰ�����û��������
// �������  : None.
// �������  : None
// ���ز���  : �ɹ� 0  ʧ�� -1
//������������������������������������������������������������������������������
int Get_Iap_Config(void)
{
	int i;
	uint16 CheckSum = 0;
	char *ps = (char*)&iap_data;

	memcpy(&iap_data,(IAP*)iap_data_add,sizeof(IAP));

	for(i = 0; i<sizeof(IAP)-2; i++)
	{
		CheckSum += *ps;
		ps++;	
	}
	if(CheckSum != iap_data.CheckSum)
	{
		return -1;
	}
	return 0;
}

//������������������������������������������������������������������������������
//                          �������(END)
//������������������������������������������������������������������������������


